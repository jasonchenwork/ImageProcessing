#ifndef STEREOIMAGE_HPP
#define STEREOIMAGE_HPP
#include <bits/stdc++.h>

#include "../include/Utility.hpp"
using namespace std;

void stereoimageBM(uint8_t* dst, uint8_t* srcL, uint8_t* srcR, int width,
                   int height, int windows, int displacement);

void stereoimageBMwithSGM(uint8_t* dst, uint8_t* srcL, uint8_t* srcR, int width,
                          int height, int windows, int displacement, int P1,
                          int P2);
#endif