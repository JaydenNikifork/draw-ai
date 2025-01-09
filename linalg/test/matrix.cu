#include <iostream>
#include <cassert>
#include <functional>
#include "../matrix.hpp"

using namespace std;

int main() {
  matrix<int> m1 = matrix<int>(2, 2);
  m1.set(0, 0, 1);
  m1.set(0, 1, 2);
  m1.set(1, 0, 3);
  m1.set(1, 1, 4);
  matrix<int> m2 = m1 * m1;
  cout << "Test 1: Multiplication" << endl;
  assert(m2[0][0] == 7);
  assert(m2[1][1] == 22);


  cout << "Test 2: Addition" << endl;
  m2 = m1 + m1;
  assert(m2[0][0] == 2);
  assert(m2[1][1] == 8);


  cout << "Test 3: Subtraction" << endl;
  matrix<int> m1Neg = -m1;
  m2 = m2 + m1Neg;
  assert(m2[0][0] == m1[0][0]);
  assert(m2[1][1] == m1[1][1]);


  cout << "Test 4: Transpose" << endl;
  matrix<float> m8(10, 20);
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 20; ++j) {
      m8.set(i, j, i * 20 + j);
    }
  }
  matrix<float> m8trans = m8.transpose();
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 20; ++j) {
      assert(m8[i][j] == m8trans[j][i]);
    }
  }
  m2 = m1.transpose();
  assert(m2[1][0] == m1[0][1]);
  assert(m2[1][1] == m1[1][1]);
 
  cout << "Test 5: CUDA Multiplication" << endl;
  matrix<int> m3(20, 30);
  for (int i = 0; i < 20; ++i) {
    for (int j = 0; j < 30; ++j) {
      m3.set(i, j, 1);
    }
  }
  matrix<int> m4(30, 40);
  for (int i = 0; i < 30; ++i) {
    for (int j = 0; j < 40; ++j) {
      m4.set(i, j, 1);
    }
  }
  matrix<int> m5 = m3 * m4;
  assert(m5[0][0] == 30 && m5[19][39] == 30);

  cout << "Test 6: CUDA Addition" << endl;
  matrix<int> m6 = m4 + m4;
  assert(m6[29][39] == 2);

  cout << "Test 7: CUDA Scaling" << endl;
  matrix<int> m7 = m4 * 2;
  assert(m7[29][39] == 2);

  cout << "All tests passed" << endl;
}