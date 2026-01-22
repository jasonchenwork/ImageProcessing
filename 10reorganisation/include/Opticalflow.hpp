#ifndef OPTICALFLOW_HPP
#define OPTICALFLOW_HPP
#include <bits/stdc++.h>

using namespace std;
void opticalflowHS(uint8_t* dst, uint8_t* srcFirst, uint8_t* srcSecond,
                   int width, int height, int iterations, double alpha);
void ShowOpticalFlowField(uint8_t* dst, vector<vector<vector<double>>>& motion,
                          int width, int height);
#endif