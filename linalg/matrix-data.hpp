#ifndef MATRIX_DATA_HPP
#define MATRIX_DATA_HPP

template<typename T>
class matrix_data {
protected:
  T **theMatrix, **theTranspose;

public:
  matrix_data(int m, int n);
  matrix_data(int m, int n, T **otherMatrix);
  matrix_data(int m, int n, T **otherMatrix, T **otherTranspose);
};

template<typename T>
matrix_data<T>::matrix_data(int m, int n):
  theMatrix{new T*[m]},
  theTranspose{new T*[n]}
{
  for (int i = 0; i < m; ++i) {
    theMatrix[i] = new T[n];
  }
  for (int i = 0; i < n; ++i) {
    theTranspose[i] = new T[m];
  }
}

template<typename T>
matrix_data<T>::matrix_data(int m, int n, T **otherMatrix, T **otherTranspose):
  theMatrix{new T*[m]},
  theTranspose{new T*[n]}
{
  for (int i = 0; i < m; ++i) {
    theMatrix[i] = new T[n];
    for (int j = 0; j < n; ++j) {
      theMatrix[i][j] = otherMatrix[i][j];
    }
  }
  for (int i = 0; i < n; ++i) {
    theTranspose[i] = new T[m];
    for (int j = 0; j < m; ++j) {
      theTranspose[i][j] = otherTranspose[i][j];
    }
  }
}

template<typename T>
matrix_data<T>::matrix_data(int m, int n, T **otherMatrix):
  theMatrix{new T*[m]},
  theTranspose{new T*[n]}
{
  for (int i = 0; i < m; ++i) {
    theMatrix[i] = new T[n];
    for (int j = 0; j < n; ++j) {
      theMatrix[i][j] = otherMatrix[i][j];
    }
  }
  for (int i = 0; i < n; ++i) {
    theTranspose[i] = new T[m];
    for (int j = 0; j < m; ++j) {
      theTranspose[i][j] = otherMatrix[j][i];
    }
  }
}

#endif