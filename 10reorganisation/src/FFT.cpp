#include "../include/FFT.hpp"

#include "../include/Utility.hpp"
void FFT1D(int dir, int m, double* x, double* y) {
  long nn, i, i1, j, k, i2, l, l1, l2;
  float c1, c2, tx, ty, t1, t2, u1, u2, z;
  nn = 1;
  for (i = 0; i < m; i++) nn *= 2;
  i2 = nn >> 1;
  j = 0;
  for (i = 0; i < nn - 1; i++) {
    if (i < j) {
      tx = x[i];
      ty = y[i];
      x[i] = x[j];
      y[i] = y[j];
      x[j] = tx;
      y[j] = ty;
    }
    k = i2;
    while (k <= j) {
      j -= k;
      k >>= 1;
    }
    j += k;
  }
  c1 = -1.0;
  c2 = 0.0;
  l2 = 1;
  for (l = 0; l < m; l++) {
    l1 = l2;
    l2 <<= 1;
    u1 = 1.0;
    u2 = 0.0;
    for (j = 0; j < l1; j++) {
      for (i = j; i < nn; i += l2) {
        i1 = i + l1;
        t1 = u1 * x[i1] - u2 * y[i1];
        t2 = u1 * y[i1] + u2 * x[i1];
        x[i1] = x[i] - t1;
        y[i1] = y[i] - t2;
        x[i] += t1;
        y[i] += t2;
      }
      z = u1 * c1 - u2 * c2;
      u2 = u1 * c2 + u2 * c1;
      u1 = z;
    }
    c2 = sqrt((1.0 - c1) / 2.0);
    if (dir == 1) c2 = -c2;

    c1 = sqrt((1.0 + c1) / 2.0);
  }
  if (dir == 1) {
    for (i = 0; i < nn; i++) {
      x[i] /= (float)nn;
      y[i] /= (float)nn;
    }
  }
}
#if 0
void DFT2D(double* data_real, double* data_imag, double* output_real,
           double* output_imag, int width, int height) {
  (void)data_imag;
#pragma omp parallel for collapse(2)
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      output_real[width * j + i] = 0.0;
      output_imag[width * j + i] = 0.0;

      for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
          double angle = 2.0 * PI *
                         ((double)i * (double)x / (double)width +
                          (double)j * (double)y / (double)height);
          // DFT 正向：cos(a) - i*sin(a)
          output_real[width * j + i] += data_real[width * y + x] * cos(angle);
          output_imag[width * j + i] -= data_real[width * y + x] * sin(angle);
        }
      }
      // output_real[width * j + i] /= (double)(width * height);
      // output_imag[width * j + i] /= (double)(width * height);
    }
  }
}
void IDFT2D(double* data_real, double* data_imag, double* output_real,
            double* output_imag, int width, int height) {
  (void)output_imag;
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      double real_sum = 0.0;

      for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
          double angle =
              2.0 * PI * ((double)i * x / width + (double)j * y / height);
          real_sum += data_real[width * j + i] * cos(angle) -
                      data_imag[width * j + i] * sin(angle);
        }
      }

      output_real[width * y + x] = real_sum / (width * height);
    }
  }
}
#else
// 1D DFT 核心邏輯
void DFT1D(double* in_real, double* in_imag, double* out_real, double* out_imag,
           int N, bool inverse) {
  double angle_sign = inverse ? 1.0 : -1.0;
  double inv_N = inverse ? 1.0 / N : 1.0;

  for (int k = 0; k < N; k++) {
    double sum_r = 0, sum_i = 0;
    for (int n = 0; n < N; n++) {
      double angle = 2.0 * PI * k * n / N;
      double cos_a = cos(angle);
      double sin_a = angle_sign * sin(angle);

      // 複數乘法: (in_r + i*in_i) * (cos_a + i*sin_a)
      sum_r += in_real[n] * cos_a - in_imag[n] * sin_a;
      sum_i += in_real[n] * sin_a + in_imag[n] * cos_a;
    }
    out_real[k] = sum_r * inv_N;
    out_imag[k] = sum_i * inv_N;
  }
}
void DFT2D_Separable(double* data_real, double* data_imag, double* out_real,
                     double* out_imag, int width, int height, bool inverse) {
  // 暫存矩陣，用來存放「行變換」後的結果
  std::vector<double> temp_real(width * height, 0.0);
  std::vector<double> temp_imag(width * height, 0.0);

  // Step 1: 對每一列 (Row) 做 1D DFT
  for (int y = 0; y < height; y++) {
    DFT1D(&data_real[y * width], &data_imag[y * width], &temp_real[y * width],
          &temp_imag[y * width], width, inverse);
  }

  // Step 2: 對每一行 (Column) 做 1D DFT
  // 注意：行不連續，需要先提取到臨時陣列
  for (int x = 0; x < width; x++) {
    std::vector<double> col_in_r(height), col_in_i(height);
    std::vector<double> col_out_r(height), col_out_i(height);

    for (int y = 0; y < height; y++) {
      col_in_r[y] = temp_real[y * width + x];
      col_in_i[y] = temp_imag[y * width + x];
    }

    DFT1D(col_in_r.data(), col_in_i.data(), col_out_r.data(), col_out_i.data(),
          height, inverse);

    for (int y = 0; y < height; y++) {
      out_real[y * width + x] = col_out_r[y];
      out_imag[y * width + x] = col_out_i[y];
    }
  }
}
void DFT2D(double* data_real, double* data_imag, double* output_real,
           double* output_imag, int width, int height) {
  DFT2D_Separable(data_real, data_imag, output_real, output_imag, width, height,
                  false);
}
void IDFT2D(double* data_real, double* data_imag, double* output_real,
            double* output_imag, int width, int height) {
  DFT2D_Separable(data_real, data_imag, output_real, output_imag, width, height,
                  true);
}
#endif
void FFT2D(double* data_real, double* data_imag, double* output_real,
           double* output_imag, int nx, int ny, int dir, int width,
           int height) {
  int i, j;
  int m;

  assert(width % 2 == 0 && height % 2 == 0);

  double* real;
  double* imag;

  real = new double[nx];
  imag = new double[nx];

  for (i = 0; i < nx; i++) {
    real[i] = 0.0;
    imag[i] = 0.0;
  }

  for (j = 0; j < ny; j++) {
    for (i = 0; i < nx; i++) {
      real[i] = data_real[width * j + i];
      imag[i] = data_imag[width * j + i];
    }

    m = (int)log2((double)nx);
    FFT1D(dir, m, real, imag);

    for (i = 0; i < nx; i++) {
      output_real[width * j + i] = real[i];
      output_imag[width * j + i] = imag[i];
    }
  }

  delete[] real;
  delete[] imag;
  real = nullptr;
  imag = nullptr;

  real = new double[ny];
  imag = new double[ny];

  for (i = 0; i < ny; i++) {
    real[i] = 0.0;
    imag[i] = 0.0;
  }

  for (i = 0; i < nx; i++) {
    for (j = 0; j < ny; j++) {
      real[j] = output_real[width * j + i];
      imag[j] = output_imag[width * j + i];
    }

    m = (int)log2((double)ny);

    FFT1D(dir, m, real, imag);

    for (j = 0; j < ny; j++) {
      output_real[width * j + i] = real[j];
      output_imag[width * j + i] = imag[j];
    }
  }

  delete[] real;
  delete[] imag;
  real = nullptr;
  imag = nullptr;
}
void FFTamplitude(double* data_real, double* data_imag, double* amplitude,
                  int width, int height) {
  int i, j;
  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      amplitude[width * j + i] =
          sqrt(data_real[width * j + i] * data_real[width * j + i] +
               data_imag[width * j + i] * data_imag[width * j + i]);
    }
  }
}
void FFTpow2(double* in, double* out, int width, int height) {
  int i, j;
  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      out[width * j + i] = in[width * j + i] * in[width * j + i];
    }
  }
}
void FFTphase(double* data_real, double* data_imag, double* phase, int width,
              int height) {
  int i, j;
  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      phase[width * j + i] =
          atan2(data_imag[width * j + i], data_real[width * j + i]);
    }
  }
}
void FFTcos(double* in, double* out, int width, int height) {
  int i, j;
  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      out[width * j + i] = cos(in[width * j + i]);
    }
  }
}
void FFTsin(double* in, double* out, int width, int height) {
  int i, j;
  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      out[width * j + i] = sin(in[width * j + i]);
    }
  }
}
void FFTexp(double* in, double* out, int width, int height) {
  int i, j;
  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      out[width * j + i] = exp(in[width * j + i]);
    }
  }
}
void matrixminus(double* in1, double* in2, double* out, int width, int height) {
  int i, j;
  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      out[width * j + i] = in1[width * j + i] - in2[width * j + i];
    }
  }
}
void matrixmutiply(double* in1, double* in2, double* out, int width,
                   int height) {
  int i, j;
  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      out[width * j + i] = in1[width * j + i] * in2[width * j + i];
    }
  }
}
void FFTlog(double* in, double* fftlog, int width, int height) {
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      fftlog[width * y + x] = log(in[width * y + x]);
    }
  }
}
void FFTnormalize(double* in, double* fftnor, int width, int height) {
  double curmax = 0;
  double curmin = DBL_MAX;
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (in[width * y + x] > curmax) curmax = in[width * y + x];
      if (in[width * y + x] < curmin) curmin = in[width * y + x];
    }
  }
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      fftnor[width * y + x] =
          ((in[width * y + x] - curmin) / (curmax - curmin)) * 255.0;
    }
  }
}
void FFT255(double* in, double* fft255, int width, int height) {
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      fft255[width * y + x] = 255.0 * in[width * y + x];
    }
  }
}
void FFTshift(double* in, double* out, int width, int height) {
  int i, j;
  int halfW = width / 2;
  int halfH = height / 2;
  double* result = new double[width * height];
  for (i = 0; i < (halfW); i++) {
    for (j = 0; j < (halfH); j++) {
      int topLeft = j * width + i;
      int topRight = j * width + (i + halfW);
      int bottomLeft = (j + halfH) * width + i;
      int bottomRight = (j + halfH) * width + (i + halfW);

      // Swap Top-Left with Bottom-Right
      result[bottomRight] = in[topLeft];
      result[topLeft] = in[bottomRight];

      // Swap Top-Right with Bottom-Left
      result[bottomLeft] = in[topRight];
      result[topRight] = in[bottomLeft];
    }
  }
  for (i = 0; i < width * height; i++) {
    out[i] = result[i];
  }
  delete[] result;
}
void FFTShiftMagnitude(double* data_real, double* data_imag, double* FFTShifted,
                       int width, int height) {
  int i, j;
  for (i = 0; i <= (width / 2) - 1; i++) {
    for (j = 0; j <= (height / 2) - 1; j++) {
      FFTShifted[width * (j + (height / 2)) + (i + width / 2)] =
          sqrt(data_real[width * j + i] * data_real[width * j + i] +
               data_imag[width * j + i] * data_imag[width * j + i]);
      FFTShifted[width * j + i] =
          sqrt(data_real[width * (j + (height / 2)) + (i + width / 2)] *
                   data_real[width * (j + (height / 2)) + (i + width / 2)] +
               data_imag[width * (j + (height / 2)) + (i + width / 2)] *
                   data_imag[width * (j + (height / 2)) + (i + width / 2)]);
      FFTShifted[width * j + (i + (width / 2))] =
          sqrt(data_real[width * (j + (height / 2)) + i] *
                   data_real[width * (j + (height / 2)) + i] +
               data_imag[width * (j + (height / 2)) + i] *
                   data_imag[width * (j + (height / 2)) + i]);
      FFTShifted[width * (j + (width / 2)) + i] =
          sqrt(data_real[width * j + (i + (width / 2))] *
                   data_real[width * j + (i + (width / 2))] +
               data_imag[width * j + (i + (width / 2))] *
                   data_imag[width * j + (i + (width / 2))]);
    }
  }
}
void FFTShow(double* FFTMagnitude, double* FFTlog, int width, int height) {
  double logmax = 0;
  double logmmin = 0x7FFF;
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      FFTlog[width * y + x] = log(1.0 + FFTMagnitude[width * y + x]);
      if (FFTlog[width * y + x] > logmax) logmax = FFTlog[width * y + x];
      if (FFTlog[width * y + x] < logmmin) logmmin = FFTlog[width * y + x];
    }
  }
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      FFTlog[width * y + x] = 2000 * (FFTlog[width * y + x] - logmmin) / logmax;
      if (FFTlog[width * y + x] > 255) FFTlog[width * y + x] = 255;
    }
  }
}
