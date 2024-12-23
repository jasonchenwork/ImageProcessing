#ifndef NONLOCALMEANS_HPP
#define NONLOCALMEANS_HPP
#include <bits/stdc++.h>
using namespace std;
void NLmeans(const uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
             uint16_t height, double h, uint8_t halfkernelsize,
             uint8_t halfsearch);
void NLmeanswithIntegralImage(const uint8_t *imageSource, uint8_t *imageTarget,
                              uint16_t width, uint16_t height, double h,
                              uint8_t halfkernelsize, uint8_t halfsearch);
void NLmeanswiththreadmain(uint8_t *imageSource, uint8_t *imageTarget,
                           uint16_t width, uint16_t height, double h,
                           uint8_t halfkernelsize, uint8_t halfsearch);
void NLmeanswithmixmain(uint8_t *imageSource, uint8_t *imageTarget,
                        uint16_t width, uint16_t height, double h,
                        uint8_t halfkernelsize, uint8_t halfsearch);
#endif