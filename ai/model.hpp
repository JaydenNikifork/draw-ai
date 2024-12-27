#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <functional>
#include <stdlib.h>
#include <cstdlib>
#include "../linalg/matrix.hpp"
#include "utils.hpp"

class Model {
  unsigned int numInputs, hiddenLayerSize, numOutputs;

public:
  Matrix<double> layer0, layerNM1;
  std::vector<Matrix<double>> hiddenLayers;

  Model(unsigned int numInputs, unsigned int hiddenLayerSize, unsigned int numHiddenLayers, unsigned int numOutputs):
    numInputs{numInputs},
    hiddenLayerSize{hiddenLayerSize},
    numOutputs{numOutputs},
    layer0{Matrix<double>(numInputs, hiddenLayerSize)},
    layerNM1{Matrix<double>(hiddenLayerSize, numOutputs)},
    hiddenLayers{std::vector<Matrix<double>>(numHiddenLayers, Matrix<double>(hiddenLayerSize, hiddenLayerSize))}
  {
    randomize();
  }

  Model() {}

  Model(const Model &other):
    layer0{other.layer0},
    layerNM1{other.layerNM1},
    hiddenLayers{other.hiddenLayers},
    numInputs{other.numInputs},
    hiddenLayerSize{other.hiddenLayerSize},
    numOutputs{other.numOutputs}
  {}

  Model(const Model &&other):
    layer0{std::move(other.layer0)},
    layerNM1{std::move(other.layerNM1)},
    hiddenLayers{std::move(other.hiddenLayers)},
    numInputs{std::move(other.numInputs)},
    hiddenLayerSize{std::move(other.hiddenLayerSize)},
    numOutputs{std::move(other.numOutputs)}
  {}

  Model &operator=(const Model &other) {
    layer0 = other.layer0;
    layerNM1 = other.layerNM1;
    hiddenLayers = other.hiddenLayers;
    numInputs = other.numInputs;
    hiddenLayerSize = other.hiddenLayerSize;
    numOutputs = other.numOutputs;
    return *this;
  }

  Model &operator=(const Model &&other) {
    layer0 = std::move(other.layer0);
    layerNM1 = std::move(other.layerNM1);
    hiddenLayers = std::move(other.hiddenLayers);
    numInputs = std::move(other.numInputs);
    hiddenLayerSize = std::move(other.hiddenLayerSize);
    numOutputs = std::move(other.numOutputs);
    return *this;
  }

  void randomize() {
    for (int y = 0; y < numInputs; ++y) {
      for (int x = 0; x < hiddenLayerSize; ++x) {
        layer0(y, x) = (std::rand() % 1000) / 1000.0 - 0.5;
      }
    }
    for (int i = 0; i < hiddenLayers.size(); ++i) {
      for (int y = 0; y < hiddenLayerSize; ++y) {
        for (int x = 0; x < hiddenLayerSize; ++x) {
          hiddenLayers[i](y, x) = (std::rand() % 1000) / 1000.0 - 0.5;
        }
      }
    }
    for (int y = 0; y < hiddenLayerSize; ++y) {
      for (int x = 0; x < numOutputs; ++x) {
        layerNM1(y, x) = (std::rand() % 1000) / 1000.0 - 0.5;
      }
    }
  }

  Model mutate(double deltaMax) {
    std::function<double (double)> randMap = [deltaMax](double entry){
      double delta = (static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) * deltaMax; 
      delta = 2 * delta - deltaMax;
      return entry + delta;
    };

    Model mutated = *this;
    mutated.layer0 = mutated.layer0.map(randMap);
    mutated.layerNM1 = mutated.layerNM1.map(randMap);
    for (int i = 0; i < mutated.hiddenLayers.size(); ++i) {
      mutated.hiddenLayers[i] = mutated.hiddenLayers[i].map(randMap);
    }

    return mutated;
  }

  std::vector<double> run(const std::vector<double> &inputs) {
    struct invalid_inputs {};
    if (inputs.size() != layer0.m) throw invalid_inputs{};

    Matrix<double> nodeLayer = Matrix<double>(inputs).transpose();

    nodeLayer = nodeLayer * layer0;
    nodeLayer = nodeLayer.map(sigmoid);
    for (int i = 0; i < hiddenLayers.size(); ++i) {
      nodeLayer = nodeLayer * hiddenLayers[i];
      nodeLayer = nodeLayer.map(sigmoid);
    }
    nodeLayer = nodeLayer * layerNM1;
    nodeLayer = nodeLayer.map(sigmoid);

    int m = layerNM1.n;
    std::vector<double> outputs(m);
    for (int i = 0; i < m; ++i) {
      outputs[i] = nodeLayer(0, i);
    }

    return outputs;
  }
};

#endif