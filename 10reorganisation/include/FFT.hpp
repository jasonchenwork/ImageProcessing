#ifndef FFT_HPP
#define FFT_HPP
#include <bits/stdc++.h>

#include "../include/SimpleMat.hpp"
using namespace std;
void FFT2D(double* data_real, double* data_imag, double* output_real,
           double* output_imag, int nx, int ny, int dir, int width, int height);
void FFTShiftMagnitude(double* data_real, double* data_imag, double* FFTShifted,
                       int width, int height);
void FFTShow(double* FFTMagnitude, double* FFTlog, int width, int height);

void FFTamplitude(double* data_real, double* data_imag, double* amplitude,
                  int width, int height);
void FFTphase(double* data_real, double* data_imag, double* phase, int width,
              int height);

void FFTcos(double* in, double* out, int width, int height);
void FFTsin(double* in, double* out, int width, int height);
void FFTexp(double* in, double* out, int width, int height);

void FFTnormalize(double* in, double* fftnor, int width, int height);
void FFT255(double* in, double* fft255, int width, int height);
void FFTpow2(double* in, double* out, int width, int height);
void FFTlog(double* in, double* fftlog, int width, int height);

void matrixminus(double* in1, double* in2, double* out, int width, int height);
void matrixmutiply(double* in1, double* in2, double* out, int width,
                   int height);
void FFTshift(double* in, double* out, int width, int height);

void DFT2D(double* data_real, double* data_imag, double* output_real,
           double* output_imag, int width, int height);
void IDFT2D(double* data_real, double* data_imag, double* output_real,
            double* output_imag, int width, int height);

class ImageDFT {
 public:
  SimpleMat<double> real;
  SimpleMat<double> imag;
  int width;
  int height;

  ImageDFT(int w, int h) : real(w, h), imag(w, h), width(w), height(h) {};
  template <typename T>
  ImageDFT(T* image, int w, int h)
      : real(w, h), imag(w, h), width(w), height(h) {
    // Constructor implementation (if needed)
    SimpleMat<double> input_real;
    SimpleMat<double> input_imag(w, h);
    double* image_double = new double[w * h];
    TypeConver(image, image_double, (uint16_t)w, (uint16_t)h);
    input_real = SimpleMat<double>(w, h, image_double);

    DFT2D(input_real.data, input_imag.data, real.data, imag.data, width,
          height);

    delete[] image_double;
    image_double = nullptr;
  }
#if 1
  template <typename T>
  ImageDFT(SimpleMat<T> image)
      : real(image.width, image.height),
        imag(image.width, image.height),
        width(image.width),
        height(image.height) {
    int w = image.width;
    int h = image.height;

    SimpleMat<double> input_real;
    SimpleMat<double> input_imag(w, h);
    // double* image_double = new double[w * h];
    // TypeConver(image.data, image_double, (uint16_t)w, (uint16_t)h);
    input_real = SimpleMat<double>(w, h, image.data);

    DFT2D(input_real.data, input_imag.data, real.data, imag.data, width,
          height);

    // delete[] image_double;
    // image_double = nullptr;
  }
#endif
  ~ImageDFT() {
    // Destructor implementation (if needed)
  }
  friend ImageDFT operator*(const ImageDFT& dft1, const ImageDFT& dft2) {
    assert(dft1.width == dft2.width && dft1.height == dft2.height);
    ImageDFT result(dft1.width, dft1.height);
    for (int i = 0; i < dft1.width * dft1.height; i++) {
      double r1 = dft1.real.data[i];
      double i1 = dft1.imag.data[i];
      double r2 = dft2.real.data[i];
      double i2 = dft2.imag.data[i];

      result.real.data[i] = r1 * r2 - i1 * i2;
      result.imag.data[i] = r1 * i2 + i1 * r2;
    }
    return result;
  }
  friend ImageDFT operator/(const ImageDFT& dft1, const double scalar) {
    assert(scalar != 0);
    ImageDFT result(dft1.width, dft1.height);
    for (int i = 0; i < dft1.width * dft1.height; i++) {
      result.real.data[i] = dft1.real.data[i] / scalar;
      result.imag.data[i] = dft1.imag.data[i] / scalar;
    }
    return result;
  }
  ImageDFT div(SimpleMat<double>& mat) {
    ImageDFT result(width, height);
    for (int i = 0; i < width * height; i++) {
      if (mat.data[i] != 0) {
        result.real.data[i] = real.data[i] / mat.data[i];
        result.imag.data[i] = imag.data[i] / mat.data[i];
      } else {
        result.real.data[i] = 0;
        result.imag.data[i] = 0;
      }
    }
    return result;
  }
  // SimpleMat<double> getIDFTImage();
  SimpleMat<double> getMagnitude() {
    SimpleMat<double> magnitude(width, height);
    for (int i = 0; i < width * height; i++) {
      magnitude.data[i] =
          sqrt(real.data[i] * real.data[i] + imag.data[i] * imag.data[i]);
    }
    return magnitude;
  }
  SimpleMat<double> getConjugate() {
    SimpleMat<double> conjugate(width, height);
    for (int i = 0; i < width * height; i++) {
      conjugate.data[i] = imag.data[i] * -1;
    }
    return conjugate;
  }
  SimpleMat<double> getIDFTImage() {
    SimpleMat<double> output_real(width, height);
    SimpleMat<double> output_imag(width, height);

    IDFT2D(real.data, imag.data, output_real.data, output_imag.data, width,
           height);

    return output_real;
  }
};

#endif