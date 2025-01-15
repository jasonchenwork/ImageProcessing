#ifndef SIMPLEMAT_HPP
#define SIMPLEMAT_HPP
#include <bits/stdc++.h>

#include <iostream>
using namespace std;
template <typename T>
class SimpleMat {
 public:
  uint32_t width;
  uint32_t height;
  T *data;
  void setMat(uint32_t w, uint32_t h);
  SimpleMat();
  SimpleMat(uint32_t w, uint32_t h);
  ~SimpleMat();
  SimpleMat<T> Create(uint32_t w, uint32_t h);
  SimpleMat<T> downsample2x();
  void clear();
  void init(int _w, int _h);
  SimpleMat<T> &operator=(const SimpleMat<T> &mat);
};
template <typename T>
SimpleMat<T> &SimpleMat<T>::operator=(const SimpleMat<T> &mat) {
  init(mat.width, mat.height);
  memcpy(data, mat.data, mat.width * mat.height * sizeof(T));
  return *this;
};
template <typename T>
void SimpleMat<T>::init(int _w, int _h) {
  width = _w;
  height = _h;
  data = new T[_w * _h];
};
template <typename T>
SimpleMat<T>::SimpleMat() {
  width = 0;
  height = 0;
  data = NULL;
};
template <typename T>
SimpleMat<T>::SimpleMat(uint32_t w, uint32_t h) {
  width = w;
  height = h;
  data = new T[w * h];
};
template <typename T>
SimpleMat<T> SimpleMat<T>::Create(uint32_t w, uint32_t h) {
  return SimpleMat<T>(w, h);
};
template <typename T>
void SimpleMat<T>::clear() {

};
template <typename T>
SimpleMat<T>::~SimpleMat() {
  if (data) {
    // delete[] data;
    data = NULL;
  }
};
template <typename T>
void SimpleMat<T>::setMat(uint32_t w, uint32_t h) {
  width = w;
  height = h;
  data = new T[w * h];
};
template <typename T>
SimpleMat<T> SimpleMat<T>::downsample2x() {
  int dw = width >> 1;
  int dh = height >> 1;
  SimpleMat<T> dst(dw, dh);
  for (int x = 0; x < dw; ++x) {
    for (int y = 0; y < dh; ++y) {
      int sx = x << 1;
      int sy = y << 1;
      // cout << "x,y" << x << "," << y << endl;
      dst.data[dw * y + x] = data[width * sy + sx];
    }
  }
  return dst;
}
#endif