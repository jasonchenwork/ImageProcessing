#ifndef FFT_HPP
#define FFT_HPP
#include <bits/stdc++.h>
using namespace std;

void FFT2D(double *data_real, double *data_imag, double *output_real,
           double *output_imag, int nx, int ny, int dir, int width, int height);
void FFTShiftMagnitude(double *data_real, double *data_imag, double *FFTShifted,
                       int width, int height);
void FFTShow(double *FFTMagnitude, double *FFTlog, int width, int height);
#endif