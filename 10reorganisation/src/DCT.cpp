#include "../include/DCT.hpp"

#include "../include/Utility.hpp"

void DCT(double* in, double* out, int width, int height) {
#pragma omp for simd
  for (int u = 0; u < width; u++) {
    for (int v = 0; v < height; v++) {
      double sum = 0.0;
      double Cu = (u == 0) ? 1.0 / sqrt(2.0) : 1.0;
      double Cv = (v == 0) ? 1.0 / sqrt(2.0) : 1.0;

      for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
          sum += in[x * width + y] *
                 cos((2.0 * x + 1.0) * u * PI / (2.0 * width)) *
                 cos((2.0 * y + 1.0) * v * PI / (2.0 * height));
        }
      }
      out[u * width + v] = 0.25 * Cu * Cv * sum;
    }
  }
}

void IDCT(double* in, double* out, int width, int height) {
#pragma omp for simd
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      double sum = 0.0;
      for (int u = 0; u < width; u++) {
        for (int v = 0; v < height; v++) {
          double Cu = (u == 0) ? 1.0 / sqrt(2.0) : 1.0;
          double Cv = (v == 0) ? 1.0 / sqrt(2.0) : 1.0;
          sum += Cu * Cv * in[u * width + v] *
                 cos((2.0 * x + 1.0) * u * PI / (2.0 * width)) *
                 cos((2.0 * y + 1.0) * v * PI / (2.0 * height));
        }
      }
      out[x * width + y] = sum * 0.25;
    }
  }
}

void DCTdenoise(double* in, double* out, int width, int height, int ch) {
  for (int rgb = 0; rgb < ch; rgb++) {
#pragma omp parallel for num_threads(8)
    for (int i = 0; i < width - 8; i += 8) {
      for (int j = 0; j < height - 8; j += 8) {
        double dctmap[64];
        double idctmap[64];
        double block[64];
        for (int x = 0; x < 8; x++) {
          for (int y = 0; y < 8; y++) {
            block[y * 8 + x] = in[(((j + y) * width + (i + x)) * ch) + rgb];
          }
        }
        DCT(block, dctmap, 8, 8);
#if 1  // denoise
        for (int u = 1; u < 8; u++) {
          for (int v = 1; v < 8; v++) {
            if (fabs(dctmap[u * 8 + v]) < 90) {
              dctmap[u * 8 + v] = 0.0;
            }
          }
        }
#endif
        IDCT(dctmap, idctmap, 8, 8);
        for (int x = 0; x < 8; x++) {
          for (int y = 0; y < 8; y++) {
            out[(((j + y) * width + (i + x)) * ch) + rgb] = idctmap[y * 8 + x];
          }
        }
      }
    }
  }
}