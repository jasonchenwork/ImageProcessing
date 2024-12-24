#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <bits/stdc++.h>
using namespace std;
struct STfilter2D {
  vector<vector<vector<int>>> kernels;
  int threshold;
  bool threshold_bypass;
  int (*filterfunc)(vector<int>);
};
int AddGaussianNoise(const uint8_t *imageSource, uint8_t *imageTarget,
                     uint16_t width, uint16_t height, double mean,
                     double stddev);

void CompareImage(const uint8_t *in1, const uint8_t *in2, uint8_t *out,
                  uint16_t width, uint16_t height);
void colorimage2grayimage(const uint8_t *imageSource, uint8_t *imageTarget,
                          uint16_t width, uint16_t height);
void grayimage2colorimage(const uint8_t *imageSource, uint8_t *imageTarget,
                          uint16_t width, uint16_t height);
void filter2D(const uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
              uint16_t height, struct STfilter2D filters);

void imagescale(const unsigned char *imageSource, unsigned char *imageTarget,
                int Swidth, int Sheight, int Twidth, int Theight);
#endif