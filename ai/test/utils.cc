#include <iostream>
#include <cassert>
#include "../utils.hpp"

using namespace std;

int main() {
  cout << "Test 1: Sigmoid" << endl;
  double x1 = 10, x2 = -20;
  double res1 = sigmoid(x1), res2 = sigmoid(x2);
  assert(-1 < res1 && res1 < 1);
  assert(-1 < res2 && res2 < 1);

  cout << "All tests passed" << endl;
}