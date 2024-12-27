#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <vector>
#include <queue>
#include <utility>
#include "model.hpp"

class Evolution {
  unsigned int concurrent, threshold, numInputs, hiddenLayerSize, numHiddenLayers, numOutputs;

public:
  std::vector<Model> generation;

  Evolution(
    unsigned int concurrent,
    unsigned int threshold,
    unsigned int numInputs,
    unsigned int hiddenLayerSize,
    unsigned int numHiddenLayers,
    unsigned int numOutputs
  ):
    concurrent{concurrent},
    threshold{threshold},
    numInputs{numInputs},
    hiddenLayerSize{hiddenLayerSize},
    numHiddenLayers{numHiddenLayers},
    numOutputs{numOutputs}
  {
    generation.resize(concurrent);
    for (int i = 0; i < concurrent; ++i) {
      generation[i] = Model(numInputs, hiddenLayerSize, numHiddenLayers, numOutputs);
    }
  }

  void stepGeneration(const std::vector<double> &inputs, std::function<double (std::vector<double>)> getScore, double deltaMax, double &prevScore) {
    std::priority_queue<std::pair<double, int>> scores;

    for (int i = 0; i < concurrent; ++i) {
      auto outputs = generation[i].run(inputs);
      double score = getScore(outputs);
      scores.push({score, i});
    }

    std::vector<Model> nextGeneration(concurrent);
    int added = 0;
    double newScore = prevScore;
    for (int i = 0; i < threshold; ++i) {
      auto [score, idx] = scores.top();
      scores.pop();

      //if (score <= prevScore + 1) break;

      nextGeneration[i] = generation[idx];

      if (!i) {
        newScore = score;
        auto outputs = generation[idx].run(inputs);
        std::cout << idx << std::endl;
        for (auto output : outputs) std::cout << output << ',';
        std::cout << score << std::endl;
      }

      ++added;
    }
    for (int i = added; i < concurrent; ++i) {
      if (added) {
        Model newModel = nextGeneration[i % added].mutate(deltaMax);
        nextGeneration[i] = newModel;
      } else {
        Model newModel = generation[i].mutate(deltaMax);
        nextGeneration[i] = newModel;
      }
    }
    for (int i = concurrent - threshold; i < concurrent; ++i) {
      nextGeneration[i].randomize();
    }

    generation = nextGeneration;
    prevScore = newScore;
  }
};

#endif