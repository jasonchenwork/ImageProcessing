#ifndef SIMPLEMAT_HPP
#define SIMPLEMAT_HPP
#include <bits/stdc++.h>

#include <iostream>
using namespace std;

class SimpleMat {
 public:
  uint32_t width;
  uint32_t height;
  void* data;
  SimpleMat();
  template <typename T>
  SimpleMat(uint32_t w, uint32_t h);

  ~SimpleMat();
};

#endif