#ifndef MATRIX_H
#define MATRIX_H

#include <utility>
#include <functional>
#include <iostream>

template<typename T>
class Matrix {
  struct invalid_size_error {};
  std::vector<std::vector<T>> vals;

public:
  std::size_t m, n;

  Matrix(unsigned int m, unsigned int n):
    m{m},
    n{n},
    vals{std::vector<std::vector<T>>(m, std::vector<T>(n))}
  {}

  Matrix(const std::vector<T> &entries): m{entries.size()}, n{1}, vals{std::vector<std::vector<T>>(entries.size(), std::vector<T>(1))} {
    for (int i = 0; i < m; ++i) {
      vals[i][0] = entries[i];
    }
  }

  Matrix() {}

  Matrix(const Matrix<T> &other):
    m{other.m},
    n{other.n},
    vals{other.vals}
  {}

  Matrix(const Matrix<T> &&other):
    m{std::move(other.m)},
    n{std::move(other.n)},
    vals{std::move(other.vals)}
  {}

  Matrix<T> &operator=(const Matrix<T> &other) {
    m = other.m;
    n = other.n;
    vals = other.vals;
    return *this;
  }

  Matrix<T> &operator=(const Matrix<T> &&other) {
    m = std::move(other.m);
    n = std::move(other.n);
    vals = std::move(other.vals);
    return *this;
  }

  Matrix<T> operator+(Matrix &other) {
    if (other.m != m || other.n != n) throw invalid_size_error{};

    Matrix<T> sum = Matrix<T>(m, n);

    for (int y = 0; y < m; ++y) {
      for (int x = 0; x < n; ++x) {
        sum(y, x) = (*this)(y, x) + other(y, x);
      }
    }

    return sum;
  }

  Matrix<T> operator-(Matrix &other) {
    if (other.m != m || other.n != n) throw invalid_size_error{};

    Matrix<T> dif = Matrix<T>(m, n);

    for (int y = 0; y < m; ++y) {
      for (int x = 0; x < n; ++x) {
        dif(y, x) = (*this)(y, x) - other(y, x);
      }
    }

    return dif;
  };

  Matrix<T> operator*(Matrix &other) {
    if (n != other.m) throw invalid_size_error{};

    Matrix<T> prod = Matrix<T>(m, other.n);

    for (int y = 0; y < m; ++y) {
      for (int x = 0; x < other.n; ++x) {
        T val{};
        for (int i = 0; i < n; ++i) {
          val += (*this)(y, i) * other(i, x);
        }
        prod(y, x) = val;
      }
    }

    return prod;
  }

  T &operator()(int y, int x) {
    return vals[y][x];
  }

  Matrix<T> transpose() {
    Matrix<T> trans(n, m);

    for (int y = 0; y < m; ++y) {
      for (int x = 0; x < n; ++x) {
        trans(x, y) = (*this)(y, x);
      }
    }

    return trans;
  }

  Matrix<T> map(std::function<T (T)> mapFn) {
    Matrix<T> result(m, n);
    for (int y = 0; y < m; ++y) {
      for (int x = 0; x < n; ++x) {
        result(y, x) = mapFn((*this)(y, x));
      }
    }
    return result;
  }

  void print() {
    for (int y = 0; y < m; ++y) {
      for (int x = 0; x < n; ++x) {
        std::cout << (*this)(y, x) << ',';
      }
      std::cout << std::endl;
    }
  }
};

#endif