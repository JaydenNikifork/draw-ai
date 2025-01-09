#ifndef AI_UTILS_H
#define AI_UTILS_H

#include <math.h>

double sigmoid(double x) {
  return 1.0 / (1.0 + std::exp(-x));
}

#endif