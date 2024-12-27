#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <math.h>

class Image {
  std::vector<double> im2vec(cv::Mat &image) {
    std::vector<double> imageVec;
    imageVec.resize(image.rows * image.cols);
    for (int i = 0; i < image.rows; ++i) {
      for (int j = 0; j < image.cols; ++j) {
        int idx = i * image.cols + j;
        imageVec[idx] = image.at<uint8_t>(i, j) / 255.0;
      }
    }
    return imageVec;
  }

  cv::Mat vec2im(std::vector<double> &imageVec, int rows, int cols) {
    cv::Mat image(rows, cols, CV_64F);
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        int idx = i * image.cols + j;
        image.at<uint8_t>(i, j) = static_cast<uint8_t>(std::clamp(static_cast<int>(std::round(imageVec[idx] * 255)), 0, 255));
      }
    }
    return image;
  }

public:
  cv::Mat image;
  std::vector<double> imageVec;

  Image(std::string path):
    image{cv::imread(path, cv::IMREAD_GRAYSCALE)}
  {
    if (image.empty()) {
      std::cerr << "Error: Could not load image!" << std::endl;
      throw 2;
    }
    if (image.type() != CV_8UC1) {
      std::cerr << "Image is not 8-bit single-channel. Converting..." << std::endl;
      image.convertTo(image, CV_8UC1);
    }
    imageVec = im2vec(image);
  }

  Image(int rows, int cols):
    image{cv::Mat(rows, cols, CV_8UC1)}
  {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        image.at<uint8_t>(i, j) = 255;
      }
    }
    imageVec = im2vec(image);
  }

  Image(const Image &other):
    image{other.image.clone()},
    imageVec{other.imageVec}
  {}

  Image(const Image &&other):
    image{std::move(other.image.clone())},
    imageVec{std::move(other.imageVec)}
  {}

  Image &operator=(const Image &other) {
    image = other.image.clone();
    imageVec = other.imageVec;
    return *this;
  }

  Image &operator=(const Image &&other) {
    image = std::move(other.image.clone());
    imageVec = std::move(other.imageVec);
    return *this;
  }

  void drawLine(double x1, double y1, double x2, double y2, double color, double thickness, double alpha = 0.25) {
    int pixX1 = std::round(image.cols * x1);
    int pixY1 = std::round(image.rows * y1);
    int pixX2 = std::round(image.cols * x2);
    int pixY2 = std::round(image.rows * y2);
    int colorInt = std::round(color * 255);
    int thicknessInt = std::max(2.0, std::round(thickness * std::max(image.cols, image.rows)));

    cv::Mat overlay;
    image.copyTo(overlay);

    cv::line(overlay, cv::Point(pixX1, pixY1), cv::Point(pixX2, pixY2), cv::Scalar(colorInt, colorInt, colorInt), thicknessInt);

    cv::addWeighted(overlay, alpha, image, 1.0 - alpha, 0, image);

    imageVec = im2vec(image);
  }

  void saveImage(std::string path) {
    cv::imwrite(path, image);
  }

  void showImage() {
    cv::imshow("Image", image);
  }

  void printImage() {
    for (int i = 0; i < image.rows; ++i) {
      for (int j = 0; j < image.cols; ++j) {
        std::cout << static_cast<int>(image.at<uint8_t>(i, j)) << ',';
      }
      std::cout << std::endl;
    }
  }
};
