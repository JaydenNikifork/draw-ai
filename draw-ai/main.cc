#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cstdlib>
#include <ctime>
#include "../ai/evolution.hpp"
#include "../image/image.hpp"

int main() {
  std::cout << "main start\n";

  Image refImage("../image2.png");
  Image curImage(refImage.image.rows, refImage.image.cols);
  Image workingImage(refImage.image.rows, refImage.image.cols);
  std::cout << "loaded images\n";

  std::srand(std::time(0));

  double maxScore = 0, prevScore = 0;

  unsigned int concurrent = 20,
               threshold = 4,
               numInputs = 2 * refImage.imageVec.size(),
               hiddenLayerSize = 25,
               numHiddenLayers = 2,
               numOutputs = 6;

  std::cout << concurrent << ',' << threshold << ',' << numInputs << ',' << hiddenLayerSize << ','<< numHiddenLayers << ',' << numOutputs << std::endl;

  Evolution ev(concurrent, threshold, numInputs, hiddenLayerSize, numHiddenLayers, numOutputs);

  std::cout << "created ev\n";

  auto getScore = [&maxScore, refImage, &workingImage, &curImage](std::vector<double> outputs) {
    // assume outputs is size 4
    Image tmpImg = curImage;
    tmpImg.drawLine(outputs[0], outputs[1], outputs[2], outputs[3], outputs[4], outputs[5]);
    double score = 0;
    for (int i = 0; i < tmpImg.imageVec.size(); ++i) {
      score += 1.0 - std::pow(std::abs(tmpImg.imageVec[i] - refImage.imageVec[i]), 0.5);
    }
    if (score > maxScore) {
      workingImage = tmpImg;
      maxScore = score;
    }
    return score;
  };

  std::cout << "starting loop\n";

  char input;
  int genNum = 1;
  while (true) {
    std::cout << "Generation " << genNum << std::endl;
    std::vector<double> inputs = refImage.imageVec;
    for (double &pix : workingImage.imageVec) {
      inputs.push_back(pix);
    }
    maxScore = 0;
    ev.stepGeneration(inputs, getScore, 0.2, prevScore);
    curImage = workingImage;
    std::string savePath = "../images/gen.png";
    curImage.saveImage(savePath);
    ++genNum;
  }
}