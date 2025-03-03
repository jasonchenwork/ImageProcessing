#ifndef DWT_HPP
#define DWT_HPP
#include <bits/stdc++.h>
using namespace std;

template <typename T1>
void DWT(T1 *imageSource, T1 *imageTarget, int width, int size);
template <typename T1>
void IDWT(T1 *imageSource, T1 *imageTarget, int width, int size);
#endif