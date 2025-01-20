#include "../include/HarrisCornerDetection.hpp"

#include "../include/Utility.hpp"

bool isLocalMax(float *Q, uint16_t width, uint16_t height, int x, int y) {
  if (y >= height || y < 0 || x >= width || x < 0) return false;
  return (Q[width * y + x] >= Q[width * (y - 1) + (x - 1)] &&
          Q[width * y + x] >= Q[width * (y - 1) + (x)] &&
          Q[width * y + x] >= Q[width * (y - 1) + (x + 1)] &&
          Q[width * y + x] >= Q[width * (y) + (x - 1)] &&
          Q[width * y + x] >= Q[width * (y) + (x + 1)] &&
          Q[width * y + x] >= Q[width * (y + 1) + (x - 1)] &&
          Q[width * y + x] >= Q[width * (y + 1) + (x)] &&
          Q[width * y + x] >= Q[width * (y + 1) + (x + 1)]);
}
void HarrisCornerDetection(const uint8_t *imageSource, uint16_t width,
                           uint16_t height, list<cornerpoint> &coners,
                           HarrisCornerArg harrisarg) {
  uint8_t *grayimage = new uint8_t[width * height];
  float *grayimagef = new float[width * height];
  float *Dx = new float[width * height];
  float *Dy = new float[width * height];
  float *Dxx = new float[width * height];
  float *Dyy = new float[width * height];
  float *Dxy = new float[width * height];
  float *Q = new float[width * height];

  colorimage2grayimage(imageSource, grayimage, width, height);
  uint8Tofloat(grayimage, grayimagef, width, height);

  uint16_t windowsize = 3;
  double SobelGx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
  double SobelGy[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
  conv2D(grayimagef, Dx, width, height, SobelGx, (uint16_t)windowsize,
         conv2D_gray);
  conv2D(grayimagef, Dy, width, height, SobelGy, (uint16_t)windowsize,
         conv2D_gray);
  int gwindowsize = 5;  // 5
  double std = 1.0;
  double *gaussianfilter = new double[gwindowsize * gwindowsize];
  GaussianFilter(gaussianfilter, (uint16_t)gwindowsize, std);
  int halfkernelsize = gwindowsize / 2;

  for (int x = halfkernelsize; x < width - halfkernelsize; ++x) {
    for (int y = halfkernelsize; y < height - halfkernelsize; ++y) {
      double Dxxsum = 0;
      double Dyysum = 0;
      double Dxysum = 0;
      for (int my = y - halfkernelsize; my <= y + halfkernelsize; ++my) {
        if (my < 0 || my >= height) continue;
        for (int mx = x - halfkernelsize; mx <= x + halfkernelsize; ++mx) {
          if (mx < 0 || mx >= width) continue;
          double coef =
              gaussianfilter[(my - y + halfkernelsize) * halfkernelsize +
                             (mx - x + halfkernelsize)];

          Dxxsum += coef * static_cast<double>(Dx[(width * my + mx)] *
                                               Dx[(width * my + mx)]);
          Dyysum += coef * static_cast<double>(Dy[(width * my + mx)] *
                                               Dy[width * my + mx]);
          Dxysum += coef * static_cast<double>(Dx[width * my + mx] *
                                               Dy[width * my + mx]);
        }
      }

      Dxx[width * y + x] = static_cast<float>(Dxxsum);
      Dyy[width * y + x] = static_cast<float>(Dyysum);
      Dxy[width * y + x] = static_cast<float>(Dxysum);
    }
  }
  double A, B, C, det, trace;
  double alpha = harrisarg.alpha;  // 0.05;
  for (int x = halfkernelsize; x < width - halfkernelsize; ++x) {
    for (int y = halfkernelsize; y < height - halfkernelsize; ++y) {
      A = Dxx[width * y + x];
      B = Dyy[width * y + x];
      C = Dxy[width * y + x];
      det = A * B - C * C;
      trace = A + B;
      Q[width * y + x] = det - alpha * (trace * trace);
    }
  }
  double threshold = harrisarg.threshold;  // 100000

  for (int x = halfkernelsize; x < width - halfkernelsize; ++x) {
    for (int y = halfkernelsize; y < height - halfkernelsize; ++y) {
      if ((Q[width * y + x]) > threshold &&
          isLocalMax(Q, width, height, x, y)) {
        cornerpoint CP;
        CP.x = x;
        CP.y = y;
        coners.push_back(CP);
      }
    }
  }

  delete[] grayimage;
  delete[] grayimagef;
  delete[] Dx;
  delete[] Dy;
  delete[] Dxx;
  delete[] Dyy;
  delete[] Dxy;
  delete[] Q;
}