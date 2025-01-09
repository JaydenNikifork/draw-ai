#include <iostream>
#include <cassert>
#include <vector>
#include <functional>
#include "../evolution.hpp"

using namespace std;

int main() {
  cout << "Test 1: Step" << endl;
  Evolution ev(3, 1, 1, 5, 5, 1);
  Model m1 = ev.generation[0], m2 = ev.generation[1], m3 = ev.generation[2];
  std::vector<double> inputs = {1};
  std::function<double (std::vector<double>)> getScore = [](std::vector<double> outputs) {
    return outputs[0];
  };
  Model maxModel = m1;
  if (getScore(m2.run(inputs)) > getScore(maxModel.run(inputs))) maxModel = m2;
  if (getScore(m3.run(inputs)) > getScore(maxModel.run(inputs))) maxModel = m3;
  double deltaMax = 0.5;
  ev.stepGeneration(inputs, getScore, deltaMax);
  assert(ev.generation[0].layer0(0, 0) == maxModel.layer0(0, 0));


  cout << "All tests passed" << endl;
}