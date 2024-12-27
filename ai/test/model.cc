#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "../model.hpp"

using namespace std;

int main() {
  std::srand(std::time(0));

  cout << "Test 1: Randomize" << endl;
  Model m1 = Model(1, 1, 0, 1);
  Model m2 = Model(1, 1, 0, 1);
  m1.randomize();
  m2.randomize();
  assert(m1.layer0(0, 0) != m2.layer0(0, 0));


  cout << "Test 2: Mutate" << endl;
  double maxDelta = 0.5;
  m2 = m1.mutate(maxDelta);
  double diff = m1.layer0(0, 0) - m2.layer0(0, 0);
  assert(-maxDelta < diff && diff < maxDelta);
  m2 = m1.mutate(maxDelta);
  diff = m1.layer0(0, 0) - m2.layer0(0, 0);
  assert(-maxDelta < diff && diff < maxDelta);
  m2 = m1.mutate(maxDelta);
  diff = m1.layer0(0, 0) - m2.layer0(0, 0);
  assert(-maxDelta < diff && diff < maxDelta);
  m2 = m1.mutate(maxDelta);
  diff = m1.layer0(0, 0) - m2.layer0(0, 0);
  assert(-maxDelta < diff && diff < maxDelta);
  m2 = m1.mutate(maxDelta);
  diff = m1.layer0(0, 0) - m2.layer0(0, 0);
  assert(-maxDelta < diff && diff < maxDelta);
  m2 = m1.mutate(maxDelta);
  diff = m1.layer0(0, 0) - m2.layer0(0, 0);
  assert(-maxDelta < diff && diff < maxDelta);

  Model a = Model(4, 5, 6, 7);
  vector<double> inputs = { 0.1, 0.2, 0.3, 0.4}, o;
  a.randomize();
  o = a.run(inputs);
  for (auto x : o) cout << x << ',';
  cout << endl << endl;
  a = a.mutate(0.2);
  o = a.run(inputs);
  for (auto x : o) cout << x << ',';
  cout << endl << endl;
  a = a.mutate(0.2);
  o = a.run(inputs);
  for (auto x : o) cout << x << ',';
  cout << endl << endl;
  a = a.mutate(0.2);
  o = a.run(inputs);
  for (auto x : o) cout << x << ',';
  cout << endl << endl;
  a = a.mutate(0.2);
  o = a.run(inputs);
  for (auto x : o) cout << x << ',';
  cout << endl << endl;
  a = a.mutate(0.2);
  o = a.run(inputs);
  for (auto x : o) cout << x << ',';
  cout << endl << endl;
  a = a.mutate(0.2);
  o = a.run(inputs);
  for (auto x : o) cout << x << ',';
  cout << endl << endl;
  a = a.mutate(0.2);
  o = a.run(inputs);
  for (auto x : o) cout << x << ',';
  cout << endl << endl;
  a = a.mutate(0.2);
  o = a.run(inputs);
  for (auto x : o) cout << x << ',';
  cout << endl << endl;
  a = a.mutate(0.2);

  cout << "Test 3: Run" << endl;
  inputs = {1};
  vector<double> outputs = m1.run(inputs);
  assert(outputs.size() == inputs.size());
  assert(-1 < outputs[0] && outputs[0] < 1);


  cout << "All tests passed" << endl;
}