#ifndef SIMPLEMAT_HPP
#define SIMPLEMAT_HPP
#include <bits/stdc++.h>

#include <iostream>
using namespace std;
template <typename T>
class SimpleMat {
 public:
  // member variables
  uint32_t width;
  uint32_t height;
  T* data;

  // constructor and destructor
  SimpleMat() {
    width = 0;
    height = 0;
    data = nullptr;
  }
  SimpleMat(const SimpleMat<T>& mat) {
    width = mat.width;
    height = mat.height;
    data = new T[width * height];
    memcpy(data, mat.data, mat.width * mat.height * sizeof(T));
  }
  SimpleMat(uint32_t w, uint32_t h) {
    width = w;
    height = h;
    data = new T[w * h]();
  }
  SimpleMat(uint32_t w, uint32_t h, T* indata) {
    width = w;
    height = h;
    data = new T[w * h];
    memcpy(data, indata, width * height * sizeof(T));
  };
  ~SimpleMat() {
    if (data) {
      delete[] data;
      data = nullptr;
    }
  }
  // operator
  // inline function
  friend SimpleMat<T> operator*(const SimpleMat<T>& mat1,
                                const SimpleMat<T>& mat2) {
    uint32_t m = mat2.width;
    uint32_t n = mat1.height;
    uint32_t innerl = mat2.height;
    assert(mat1.width == mat2.height);
    SimpleMat<T> result(m, n);
    for (uint32_t i = 0; i < n; i++) {
      for (uint32_t j = 0; j < m; j++) {
        T dot = 0;
        for (uint32_t k = 0; k < innerl; k++) {
          dot += (T)mat1.data[i * mat1.width + k] *
                 (T)mat2.data[k * mat2.width + j];
        }
        result.data[i * m + j] = dot;
      }
    }
    return result;
  }
  friend SimpleMat<T> operator+(const SimpleMat<T>& mat1,
                                const SimpleMat<T>& mat2) {
    uint32_t m = mat2.width;
    uint32_t n = mat1.height;

    assert(mat1.width == mat2.width && mat1.height == mat2.height);
    SimpleMat<T> result(m, n);
    for (uint32_t i = 0; i < n; i++) {
      for (uint32_t j = 0; j < m; j++) {
        result.data[i * m + j] =
            (T)mat1.data[i * m + j] + (T)mat2.data[i * m + j];
      }
    }
    return result;
  }
  friend SimpleMat<T> operator-(const SimpleMat<T>& mat1,
                                const SimpleMat<T>& mat2) {
    uint32_t m = mat2.width;
    uint32_t n = mat1.height;

    assert(mat1.width == mat2.width && mat1.height == mat2.height);
    SimpleMat<T> result(m, n);
    for (uint32_t i = 0; i < n; i++) {
      for (uint32_t j = 0; j < m; j++) {
        result.data[i * m + j] =
            (T)mat1.data[i * m + j] - (T)mat2.data[i * m + j];
      }
    }
    return result;
  }
  friend SimpleMat<T> operator/(const SimpleMat<T>& mat1,
                                const SimpleMat<T>& mat2) {
    uint32_t m = mat1.width;
    uint32_t n = mat1.height;

    assert(mat1.width == mat2.width && mat1.height == mat2.height);
    SimpleMat<T> result(m, n);
    for (uint32_t i = 0; i < n; i++) {
      for (uint32_t j = 0; j < m; j++) {
        if ((T)mat2.data[i * m + j] == 0) {
          result.data[i * m + j] = 0;
        } else
          result.data[i * m + j] =
              (T)mat1.data[i * m + j] / (T)mat2.data[i * m + j];
      }
    }
    return result;
  }
  friend SimpleMat<T> operator/(const SimpleMat<T>& mat1, T scalar) {
    uint32_t m = mat1.width;
    uint32_t n = mat1.height;
    SimpleMat<T> result(m, n);
    for (uint32_t i = 0; i < n; i++) {
      for (uint32_t j = 0; j < m; j++) {
        if (scalar == 0) {
          result.data[i * m + j] = 0;
        } else
          result.data[i * m + j] = (T)mat1.data[i * m + j] / scalar;
      }
    }
    return result;
  }
  friend SimpleMat<T> operator+(const SimpleMat<T>& mat1, T scalar) {
    uint32_t m = mat1.width;
    uint32_t n = mat1.height;
    SimpleMat<T> result(m, n);
    for (uint32_t i = 0; i < n; i++) {
      for (uint32_t j = 0; j < m; j++) {
        result.data[i * m + j] = (T)mat1.data[i * m + j] + scalar;
      }
    }
    return result;
  }
  friend SimpleMat<T> operator-(const SimpleMat<T>& mat1, T scalar) {
    uint32_t m = mat1.width;
    uint32_t n = mat1.height;
    SimpleMat<T> result(m, n);
    for (uint32_t i = 0; i < n; i++) {
      for (uint32_t j = 0; j < m; j++) {
        result.data[i * m + j] = (T)mat1.data[i * m + j] - scalar;
      }
    }
    return result;
  }
  SimpleMat<T>& operator=(const SimpleMat<T>& mat) {
    if (this == &mat) return *this;
    delete[] data;
    width = mat.width;
    height = mat.height;
    data = new T[width * height];
    memcpy(data, mat.data, mat.width * mat.height * sizeof(T));
    return *this;
  }
  // functions
  SimpleMat<T> mul(const SimpleMat<T>& mat1) {
    uint32_t m = mat1.width;
    uint32_t n = mat1.height;

    assert(this->width == mat1.width &&
           this->height == mat1.height);  // Note: this was wrong, should be
                                          // this->width == mat1.width etc.
    SimpleMat<T> result(m, n);
    for (uint32_t i = 0; i < n; i++) {
      for (uint32_t j = 0; j < m; j++) {
        result.data[i * m + j] =
            (T)mat1.data[i * m + j] * (T)this->data[i * m + j];
      }
    }
    return result;
  }
  double sum() {
    double total = 0.0;
    for (uint32_t i = 0; i < width * height; i++) {
      total += (double)data[i];
    }
    return total;
  }

  void show() {
    cout << endl;

    for (uint32_t j = 0; j < height; j++) {
      for (uint32_t i = 0; i < width; i++) {
        cout << data[j * width + i] << " , ";
      }
      cout << endl;
    }
  }
  void clear() {
    if (data) {
      delete[] data;
      data = nullptr;
    }
    this->width = 0;
    this->height = 0;
  }
  void init(int _w, int _h) {
    clear();  // Clear existing data to prevent memory leak
    width = _w;
    height = _h;
    data = new T[_w * _h];
  };

  SimpleMat<T> Create(uint32_t w, uint32_t h) { return SimpleMat<T>(w, h); }

  void setMat(uint32_t w, uint32_t h) {
    clear();  // Clear existing data to prevent memory leak
    width = w;
    height = h;
    data = new T[w * h];
  }
  T* toArray() {
    T* newdata = new T[width * height];
    memcpy(newdata, data, width * height * sizeof(T));
    return newdata;
  }
  T** to2DArray() {
    T** arr2D = new T*[height];
    for (uint32_t i = 0; i < height; i++) {
      arr2D[i] = new T[width];
      memcpy(arr2D[i], data + i * width, width * sizeof(T));
    }
    return arr2D;
  }
  SimpleMat<T> downsample2x() {
    int dw = (width + 1) >> 1;
    int dh = (height + 1) >> 1;
    SimpleMat<T> dst(dw, dh);
    for (int x = 0; x < dw; ++x) {
      for (int y = 0; y < dh; ++y) {
        int sx = x << 1;
        int sy = y << 1;
        dst.data[dw * y + x] = data[width * sy + sx];
      }
    }
    return dst;
  }
};

#endif