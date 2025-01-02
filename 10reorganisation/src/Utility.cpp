#include "../include/Utility.hpp"

int AddGaussianNoise(const uint8_t *imageSource, uint8_t *imageTarget,
                     uint16_t width, uint16_t height, double mean,
                     double stddev) {
  random_device rd;
  mt19937 gen(rd());
  normal_distribution<> dist(mean, stddev);
  uint16_t x, y;
  double r, g, b;

  for (y = 0; y != height; ++y) {
    for (x = 0; x != width; ++x) {
      double noise = static_cast<int>(dist(gen));
      r = *(imageSource + 3 * (width * y + x) + 2) + noise;
      g = *(imageSource + 3 * (width * y + x) + 1) + noise;
      b = *(imageSource + 3 * (width * y + x) + 0) + noise;

      r = (r > 255) ? 255 : r;
      g = (g > 255) ? 255 : g;
      b = (b > 255) ? 255 : b;

      r = (r < 0) ? 0 : r;
      g = (g < 0) ? 0 : g;
      b = (b < 0) ? 0 : b;

      *(imageTarget + 3 * (width * y + x) + 2) = (uint8_t)r;
      *(imageTarget + 3 * (width * y + x) + 1) = (uint8_t)g;
      *(imageTarget + 3 * (width * y + x) + 0) = (uint8_t)b;
    }
  }

  return 1;
}
void CompareImage(const uint8_t *in1, const uint8_t *in2, uint8_t *out,
                  uint16_t width, uint16_t height) {
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      int curIndex = 3 * (width * y + x);

      int curR = *(in1 + curIndex + 2);
      int curG = *(in1 + curIndex + 1);
      int curB = *(in1 + curIndex + 0);

      double in1_gray = (curR + curG + curB) / 3.0;

      int curR2 = *(in2 + curIndex + 2);
      int curG2 = *(in2 + curIndex + 1);
      int curB2 = *(in2 + curIndex + 0);

      double in2_gray = (curR2 + curG2 + curB2) / 3.0;

      uint8_t differ = (uint8_t)abs(in1_gray - in2_gray);

      if (differ > 0) cout << differ << endl;

      *(out + curIndex + 2) = differ;
      *(out + curIndex + 1) = differ;
      *(out + curIndex + 0) = differ;
    }
  }
}

void colorimage2grayimage(const uint8_t *imageSource, uint8_t *imageTarget,
                          uint16_t width, uint16_t height) {
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      int curIndex = 3 * (width * y + x);

      int curR = imageSource[curIndex + 2];
      int curG = imageSource[curIndex + 1];
      int curB = imageSource[curIndex + 0];

      int grayvalue = (curR + curG + curB) / 3;

      imageTarget[width * y + x] = grayvalue;
    }
  }
}
void grayimage2colorimage(const uint8_t *imageSource, uint8_t *imageTarget,
                          uint16_t width, uint16_t height) {
  // gray image to color image
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      int curIndex = 3 * (width * y + x);

      int grayvalue = imageSource[width * y + x];

      imageTarget[curIndex + 2] = grayvalue;
      imageTarget[curIndex + 1] = grayvalue;
      imageTarget[curIndex + 0] = grayvalue;
    }
  }
}

void filter2D(const uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
              uint16_t height, struct STfilter2D filters) {
  int halfkernelsize = filters.kernels[0].size() / 3;
  vector<int> filterd(filters.kernels.size(), 0);
  if (halfkernelsize < 1) {
    return;  // do nothing
  }

  for (int x = 1; x < width - 1; ++x) {
    for (int y = 1; y < height - 1; ++y) {
      for (int i = 0; i < (int)filters.kernels.size(); i++) {
        filterd[i] = 0;
      }
      int maskres = 0;
      for (int my = y - halfkernelsize; my <= y + halfkernelsize; ++my) {
        for (int mx = x - halfkernelsize; mx <= x + halfkernelsize; ++mx) {
          for (int i = 0; i < (int)filters.kernels.size(); i++) {
            filterd[i] += filters.kernels[i][mx - x + halfkernelsize]
                                         [my - y + halfkernelsize] *
                          imageSource[width * my + mx];
          }
        }
      }
      if (!filters.threshold_bypass) {
        maskres = filters.filterfunc(filterd);
      } else {
        maskres = filterd[0];
      }
      if (maskres > filters.threshold) {
        imageTarget[width * y + x] = 255;
      } else {
        imageTarget[width * y + x] = 0;
      }
    }
  }
}
void imagescale(const unsigned char *imageSource, unsigned char *imageTarget,
                int Swidth, int Sheight, int Twidth, int Theight) {
  double zx = (double)Twidth / (double)Swidth;
  double zy = (double)Theight / (double)Sheight;

  int i, j, m, n;

  double p, q;

  int dR = 0;
  int dG = 0;
  int dB = 0;

  // unsigned char R, G, B;  // color of R, G, B

  for (i = 0; i < Theight; i++) {
    for (j = 0; j < Twidth; j++) {
      double x, y;
      y = (i / zy);
      x = (j / zx);

      if (y > 0) {
        m = (int)(y);
      } else {
        m = (int)(y - 1);
      }

      if (x > 0) {
        n = (int)(x);
      } else {
        n = (int)(x - 1);
      }

      q = y - m;
      p = x - n;

      if (q == 1) {
        q = 0;
        m += 1;
      }
      if (p == 1) {
        p = 0;
        n += 1;
      }
      if ((m >= 0) && (m < Sheight - 1) && (n >= 0) && (n < Swidth - 1)) {
        dR =
            ((1.0 - q) *
                 ((1.0 - p) * (*(imageSource + 3 * (Swidth * m + n) + 2)) +
                  p * (*(imageSource + 3 * (Swidth * (m + 1) + n) + 2))) +
             q * ((1.0 - p) *
                      (*(imageSource + 3 * (Swidth * m + (n + 1)) + 2)) +
                  p * (*(imageSource + 3 * (Swidth * (m + 1) + (n + 1)) + 2))));
        dG =
            ((1.0 - q) *
                 ((1.0 - p) * (*(imageSource + 3 * (Swidth * m + n) + 1)) +
                  p * (*(imageSource + 3 * (Swidth * (m + 1) + n) + 1))) +
             q * ((1.0 - p) *
                      (*(imageSource + 3 * (Swidth * m + (n + 1)) + 1)) +
                  p * (*(imageSource + 3 * (Swidth * (m + 1) + (n + 1)) + 1))));
        dB =
            ((1.0 - q) *
                 ((1.0 - p) * (*(imageSource + 3 * (Swidth * m + n) + 0)) +
                  p * (*(imageSource + 3 * (Swidth * (m + 1) + n) + 0))) +
             q * ((1.0 - p) *
                      (*(imageSource + 3 * (Swidth * m + (n + 1)) + 0)) +
                  p * (*(imageSource + 3 * (Swidth * (m + 1) + (n + 1)) + 0))));
      } else {
        dG = dB = dR = 0;
      }

      if (dG < 0) {
        dG = 0;
      }
      if (dR < 0) {
        dR = 0;
      }
      if (dB < 0) {
        dB = 0;
      }
      if (dG > 255) {
        dG = 255;
      }
      if (dR > 255) {
        dR = 255;
      }
      if (dB > 255) {
        dB = 255;
      }

      *(imageTarget + 3 * (Twidth * i + j) + 2) = (char)dR;
      *(imageTarget + 3 * (Twidth * i + j) + 1) = (char)dG;
      *(imageTarget + 3 * (Twidth * i + j) + 0) = (char)dB;
    }
  }
}
void GaussianFilter(double *gaussianfilter, uint16_t size, double std) {
  int windowbase = (int)size / 2;
  for (int x = -windowbase; x <= windowbase; x++) {
    for (int y = -windowbase; y <= windowbase; y++) {
      gaussianfilter[(x + windowbase) * size + (y + windowbase)] =
          (1.0 / (2.0 * PI * std * std)) *
          exp(-1.0 * ((x) * (x) + (y) * (y)) / (2.0 * std * std));
    }
  }
}
void MedianFilter(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
                  uint16_t height, uint16_t filtersize, uint8_t dim) {
  int halfkernelsize = filtersize / 2;

  if (halfkernelsize < 1) {
    return;  // do nothing
  }

  for (int x = 1; x < width - 1; ++x) {
    for (int y = 1; y < height - 1; ++y) {
      vector<vector<int>> filter(dim, vector<int>());

      for (int my = y - halfkernelsize; my <= y + halfkernelsize; ++my) {
        if (my < 0 || my >= height) continue;
        for (int mx = x - halfkernelsize; mx <= x + halfkernelsize; ++mx) {
          if (mx < 0 || mx >= width) continue;
          for (int i = 0; i < dim; i++) {
            filter[i].push_back(imageSource[dim * (width * my + mx) + i]);
          }
        }
      }

      for (int i = 0; i < dim; i++) {
        sort(filter[i].begin(), filter[i].end());
#if 0
        for (auto aa : filter[i]) {
          cout << aa << " ";
        }
        cout << endl;
#endif
        int median = filter[i].size() / 2;
        imageTarget[dim * (width * y + x) + i] = filter[i][median];
      }
    }
  }
}
template <typename T1, typename T2>
void conv2D(T1 *imageSource, T1 *imageTarget, uint16_t width, uint16_t height,
            T2 *filter, uint16_t filtersize, uint8_t dim) {
  int halfkernelsize = filtersize / 2;

  if (halfkernelsize < 1) {
    return;  // do nothing
  }

  for (int x = 1; x < width - 1; ++x) {
    for (int y = 1; y < height - 1; ++y) {
      double filtersum[dim] = {0};

      for (int my = y - halfkernelsize; my <= y + halfkernelsize; ++my) {
        if (my < 0 || my >= height) continue;
        for (int mx = x - halfkernelsize; mx <= x + halfkernelsize; ++mx) {
          if (mx < 0 || mx >= width) continue;
          double coef = filter[(my - y + halfkernelsize) * halfkernelsize +
                               (mx - x + halfkernelsize)];

          for (int i = 0; i < dim; i++) {
            filtersum[i] +=
                coef * (double)imageSource[dim * (width * my + mx) + i];
          }
        }
      }
      for (int i = 0; i < dim; i++) {
        if (filtersum[i] < 0) filtersum[i] = 0;
        if (filtersum[i] > 255) filtersum[i] = 255;
        imageTarget[dim * (width * y + x) + i] = (T1)filtersum[i];
      }
    }
  }
}
// Explicit Instantiation
template void conv2D<uint8_t, double>(uint8_t *imageSource,
                                      uint8_t *imageTarget, uint16_t width,
                                      uint16_t height, double *filter,
                                      uint16_t filtersize, uint8_t dim);