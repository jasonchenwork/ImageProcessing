#ifndef FFT_HPP
#define FFT_HPP
#include <bits/stdc++.h>
using namespace std;

void FFT2D(double *data_real, double *data_imag, double *output_real,
           double *output_imag, int nx, int ny, int dir, int width, int height);
void FFTShiftMagnitude(double *data_real, double *data_imag, double *FFTShifted,
                       int width, int height);
void FFTShow(double *FFTMagnitude, double *FFTlog, int width, int height);

void FFTamplitude(double *data_real, double *data_imag, double *amplitude,
                  int width, int height);
void FFTphase(double *data_real, double *data_imag, double *phase, int width,
              int height);

void FFTcos(double *in, double *out, int width, int height);
void FFTsin(double *in, double *out, int width, int height);
void FFTexp(double *in, double *out, int width, int height);

void FFTnormalize(double *in, double *fftnor, int width, int height);
void FFTpow2(double *in, double *out, int width, int height);
void FFTlog(double *in, double *fftlog, int width, int height);

void matrixminus(double *in1, double *in2, double *out, int width, int height);
void matrixmutiply(double *in1, double *in2, double *out, int width,
                   int height);
#endif