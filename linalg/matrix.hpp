#ifndef MATRIX_H
#define MATRIX_H

#include <cuda_runtime.h>
#include "matrix-data.hpp"

#define CUDA_MIN 256

template<typename T>
class matrix: public matrix_data<T> {
  int m, n;

  matrix(int m, int n, T **theMatrix, T **theTranspose);

  int size() { return m * n; }

  void updateTranspose();

public:
  struct invalid_size_error {};
  struct index_out_of_bound_error{};

  class vector {
    int row;
    matrix &theMatrix;
  public:
    vector(int row, matrix &theMatrix): row{row}, theMatrix{theMatrix} {}

    const T operator[](int index);
  };

  matrix(int m, int n);

  matrix(matrix<T> &other);

  matrix<T> transpose();

  matrix<T> operator+(matrix<T> &other);
  matrix<T> operator*(matrix<T> &other);
  matrix<T> operator*(T c);
  matrix<T> operator-();
  friend matrix<T> operator*(T c, matrix<T> &us) { return us * c; }

  vector operator[](int rowIndex);

  void set(int m, int n, T val);
};

template<typename T>
matrix<T> matrixAverage(matrix<T> &m1, matrix<T> &m2, float w1=0.5, float w2=0.5) {
  matrix<T> newM1 = w1 * m1, newM2 = w2 * m2;
  return newM1 + newM2;
}







template<typename T>
matrix<T>::matrix(int m, int n): matrix_data<T>(m, n), m{m}, n{n} {}

template<typename T>
matrix<T>::matrix(int m, int n, T **theMatrix, T **theTranspose):
  matrix_data<T>(m, n, theMatrix, theTranspose),
  m{m},
  n{n}
{}

template<typename T>
matrix<T>::matrix(matrix &other):
  matrix_data<T>(other.m, other.n, other.theMatrix, other.theTranspose),
  m{other.m},
  n{other.n}
{}

template<typename T>
void matrix<T>::updateTranspose() {
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      this->theTranspose[j][i] = this->theMatrix[i][j];
    }
  }
}

template<typename T>
matrix<T> matrix<T>::transpose() {
  return matrix(n, m, this->theTranspose, this->theMatrix);
}

template<typename T>
void matrix<T>::set(int i, int j, T val) {
  this->theMatrix[i][j] = val;
  this->theTranspose[j][i] = val;
}

template<typename T>
matrix<T>::vector matrix<T>::operator[](int rowIndex) {
  if (rowIndex >= m) {
    throw index_out_of_bound_error{};
  }

  return vector{rowIndex, *this};
}

template<typename T>
const T matrix<T>::vector::operator[](int index) {
  return theMatrix.theMatrix[row][index];
}

template<typename T>
T *copyMatrixToCuda(int m, int n, T **matrix) {
  int size = m * n;
  T *cudaMatrix;
  cudaMalloc(&cudaMatrix, size * sizeof(T));
  for (size_t i = 0; i < m; ++i) {
    cudaMemcpy(cudaMatrix + (i * n), matrix[i], n * sizeof(T), cudaMemcpyHostToDevice);
  }
  return cudaMatrix;
}

template<typename T>
void moveCudaToMatrix(int m, int n, T *cudaMatrix, T **matrix) {
  for (size_t i = 0; i < m; ++i) {
    cudaMemcpy(matrix[i], cudaMatrix + (i * n), n * sizeof(T), cudaMemcpyDeviceToHost);
  }
}

template<typename T>
__global__ void cudaMatrixAdd(T *m1, T *m2, T *sum, int m, int n) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  int j = blockIdx.y * blockDim.y + threadIdx.y;
  int idx = i * n + j;

  if (i < m && j < n) {
    sum[idx] = m1[idx] + m2[idx];
  }
}

template<typename T>
matrix<T> matrix<T>::operator+(matrix<T> &other) {
  if (m != other.m || n != other.n) {
    throw invalid_size_error{};
  }

  matrix<T> sum(*this);

  if (size() < CUDA_MIN) {
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        sum.set(i, j, sum[i][j] + other[i][j]);
      }
    }
    sum.updateTranspose();
    return sum;
  }

  // big enough to CUDA
  T *cudaMatrix1 = copyMatrixToCuda(m, n, sum.theMatrix);
  T *cudaMatrix2 = copyMatrixToCuda(m, n, other.theMatrix);

  dim3 blockDim(16, 16);
  dim3 gridDim((m + blockDim.x - 1) / blockDim.x,
               (n + blockDim.y - 1) / blockDim.y);

  cudaMatrixAdd<<<gridDim, blockDim>>>(cudaMatrix1, cudaMatrix2, cudaMatrix1, m, n);
  cudaDeviceSynchronize();

  moveCudaToMatrix(m, n, cudaMatrix1, sum.theMatrix);
  sum.updateTranspose();

  cudaFree(cudaMatrix1);
  cudaFree(cudaMatrix2);

  return sum;
}

template<typename T>
__global__ void cudaMatrixMul(T *m1, T *m2Trans, T *prod, int m, int n, int sharedDim) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  int j = blockIdx.y * blockDim.y + threadIdx.y;

  if (i < m && j < n) {
    for (int k = 0; k < sharedDim; ++k) {
      prod[i * n + j] += m1[i * sharedDim + k] * m2Trans[j * sharedDim + k];
    }
  }
}

template<typename T>
matrix<T> matrix<T>::operator*(matrix<T> &other) {
  if (n != other.m) {
    throw invalid_size_error{};
  }

  matrix<T> prod(m, other.n);

  if (prod.size() < CUDA_MIN * 2) {
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < other.n; ++j) {
        for (int idx = 0; idx < n; ++idx) {
          prod.set(i, j, prod[i][j] + this->theMatrix[i][idx] * other.theTranspose[j][idx]);
        }
      }
    }
    prod.updateTranspose();
    return prod;
  }

  // big enough to CUDA
  T *cudaMatrix1 = copyMatrixToCuda(m, n, this->theMatrix);
  T *cudaMatrix2 = copyMatrixToCuda(other.n, other.m, other.theTranspose);
  T *cudaMatrixProd = copyMatrixToCuda(m, other.n, prod.theMatrix);

  dim3 blockDim(16, 16);
  dim3 gridDim((m + blockDim.x - 1) / blockDim.x,
               (other.n + blockDim.y - 1) / blockDim.y);

  cudaMatrixMul<<<gridDim, blockDim>>>(cudaMatrix1, cudaMatrix2, cudaMatrixProd, m, other.n, n);
  cudaDeviceSynchronize();

  moveCudaToMatrix(m, other.n, cudaMatrixProd, prod.theMatrix);
  prod.updateTranspose();

  cudaFree(cudaMatrix1);
  cudaFree(cudaMatrix2);
  cudaFree(cudaMatrixProd);

  return prod;
}

template<typename T>
__global__ void cudaMatrixScale(T *matrix, int m, int n, T c) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  int j = blockIdx.y * blockDim.y + threadIdx.y;
  int idx = i * n + j;

  if (i < m && j < n) {
    matrix[idx] *= c;
  }
}

template<typename T>
matrix<T> matrix<T>::operator*(T c) {
  matrix<T> prod(*this);

  if (prod.size() < CUDA_MIN) {
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        prod.set(i, j, prod[i][j] * c);
      }
    }
    prod.updateTranspose();
    return prod;
  }

  // big enough to CUDA
  T *cudaMatrix = copyMatrixToCuda(m, n, prod.theMatrix);

  dim3 blockDim(16, 16);
  dim3 gridDim((m + blockDim.x - 1) / blockDim.x,
               (n + blockDim.y - 1) / blockDim.y);

  cudaMatrixScale<<<gridDim, blockDim>>>(cudaMatrix, m, n, c);
  cudaDeviceSynchronize();

  moveCudaToMatrix(m, n, cudaMatrix, prod.theMatrix);
  prod.updateTranspose();

  cudaFree(cudaMatrix);

  return prod;
}

template<typename T>
matrix<T> matrix<T>::operator-() {
  return (*this) * static_cast<T>(-1);
}

#endif