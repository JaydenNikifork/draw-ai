#include <iostream>
#include <cassert>
#include <functional>
#include "../matrix.hpp"

using namespace std;

int main() {
  Matrix<int> m1 = Matrix<int>(2, 2);
  m1(0, 0) = 1;
  m1(0, 1) = 2;
  m1(1, 0) = 3;
  m1(1, 1) = 4;
  Matrix<int> m2 = m1 * m1;
  cout << "Test 1: Multiplication" << endl;
  assert(m2(0, 0) == 7);
  assert(m2(1, 1) == 22);


  cout << "Test 2: Addition" << endl;
  m2 = m1 + m1;
  assert(m2(0, 0) == 2);
  assert(m2(1, 1) == 8);


  cout << "Test 3: Subtraction" << endl;
  m2 = m2 - m1;
  assert(m2(0, 0) == m1(0, 0));
  assert(m2(1, 1) == m1(1, 1));


  cout << "Test 4: Transpose" << endl;
  m2 = m1.transpose();
  assert(m2(1, 0) == m1(0, 1));
  assert(m2(1, 1) == m1(1, 1));


  cout << "Test 5: Map" << endl;
  function<int (int)> inc = [](int entry){ return entry + 1; };
  m2 = m1.map(inc);
  assert(m2(0, 0) == m1(0, 0) + 1);
  assert(m2(1, 1) == m1(1, 1) + 1);


  cout << "All tests passed" << endl;
}