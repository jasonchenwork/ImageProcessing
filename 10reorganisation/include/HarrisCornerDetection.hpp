#ifndef HARRISCORNERDETECTION_HPP
#define HARRISCORNERDETECTION_HPP
#include <bits/stdc++.h>
using namespace std;
struct cornerpoint {
  uint16_t x;
  uint16_t y;
};
struct HarrisCornerArg {
  double alpha;
  double threshold;
};
void HarrisCornerDetection(const uint8_t *imageSource, uint16_t width,
                           uint16_t height, list<cornerpoint> &coners,
                           HarrisCornerArg harrisarg);
#endif