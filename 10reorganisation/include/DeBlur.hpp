#ifndef DEBLUR_HPP
#define DEBLUR_HPP
#include <bits/stdc++.h>

using namespace std;

void DeBlurWithWienerFilter(uint8_t* src, uint8_t* dst, int w, int h, int len,
                            int theta, double SNR);
#endif