#ifndef WHITEBALANCE_HPP
#define WHITEBALANCE_HPP

#include <bits/stdc++.h>
using namespace std;
void HistogramEqualization(const uint8_t *imageSource, uint8_t *imageTarget,
                           uint16_t width, uint16_t height);
void whitebalanceGWA(const uint8_t *imageSource, uint8_t *imageTarget,
                     uint16_t width, uint16_t height);

void whitebalance2005Lam(const uint8_t *imageSource, uint8_t *imageTarget,
                         uint16_t width, uint16_t height);
#endif