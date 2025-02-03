#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <bits/stdc++.h>
using namespace std;
#define PI 3.141592

struct STfilter2D {
  vector<vector<vector<int>>> kernels;
  int threshold;
  bool threshold_bypass;
  int (*filterfunc)(vector<int>);
};
enum {
  conv2D_gray = 1,
  conv2D_color = 3,
};
enum COLORIDX {
  COLOR_RED = 0,
  COLOR_GREEN = 1,
  COLOR_BLUE = 2,
  COLOR_WHITE = 3
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

void GaussianFilter(double *gaussianfilter, uint16_t size, double std);

void MedianFilter(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
                  uint16_t height, uint16_t filtersize, uint8_t dim);

template <typename T1, typename T2>
void conv2D(T1 *imageSource, T1 *imageTarget, uint16_t width, uint16_t height,
            T2 *filter, uint16_t filtersize, uint8_t dim);
template <typename T1, typename T2>
void conv2D(vector<T1> &imageSource, vector<T1> &imageTarget, uint16_t width,
            uint16_t height, T2 *filter, uint16_t filtersize, uint8_t dim);

void uint8Tofloat(uint8_t *src, float *dst, uint16_t width, uint16_t height);
void floatTouint8(float *src, uint8_t *dst, uint16_t width, uint16_t height);

void drawCircle(uint8_t *dst, uint16_t w, uint16_t h, int x, int y, int radius,
                int coloridx);
void draw_red_orientation(uint8_t *dst, uint16_t w, uint16_t h, int x, int y,
                          float ori, int cR);
void setPixelColor(uint8_t *dst, uint16_t w, uint16_t h, int x, int y,
                   int coloridx);
#endif