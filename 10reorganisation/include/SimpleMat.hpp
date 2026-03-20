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
  // 2. 拷貝建構子 (Deep Copy)
  SimpleMat(const SimpleMat<T>& other)
      : width(other.width), height(other.height) {
    data = new T[width * height];
    std::copy(other.data, other.data + (width * height), data);
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
  // 3. 移動建構子 (Move Constructor) - 效能優化關鍵
  SimpleMat(SimpleMat<T>&& other) noexcept
      : width(other.width), height(other.height), data(other.data) {
    other.data = nullptr;
    other.width = 0;
    other.height = 0;
  }
  ~SimpleMat() {
    if (data) {
      delete[] data;
      data = nullptr;
    }
  }
  // operator

  // 矩陣運算
  SimpleMat<T>& operator+=(const SimpleMat<T>& other) {
    assert(width == other.width && height == other.height);
    for (uint32_t i = 0; i < width * height; ++i) data[i] += other.data[i];
    return *this;
  }
  friend SimpleMat<T> operator+(SimpleMat<T> lhs, const SimpleMat<T>& rhs) {
    lhs += rhs;
    return lhs;
  }

  SimpleMat<T>& operator-=(const SimpleMat<T>& other) {
    assert(width == other.width && height == other.height);
    uint32_t total_size = width * height;

    for (uint32_t i = 0; i < total_size; ++i) {
      data[i] -= other.data[i];
    }
    return *this;
  }
  friend SimpleMat<T> operator-(SimpleMat<T> lhs, const SimpleMat<T>& rhs) {
    lhs -= rhs;
    return lhs;
  }

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

  SimpleMat<T>& operator/=(const SimpleMat<T>& other) {
    assert(width == other.width && height == other.height);
    uint32_t total_size = width * height;

    for (uint32_t i = 0; i < total_size; ++i) {
      if (other.data[i] != 0) {
        data[i] /= other.data[i];
      } else {
        data[i] = 0;
      }
    }
    return *this;
  }

  friend SimpleMat<T> operator/(SimpleMat<T> lhs, const SimpleMat<T>& rhs) {
    lhs /= rhs;
    return lhs;
  }
  // 純量運算

  //  1. 先實作成員函數版本的 operator-= (基礎邏輯)
  SimpleMat<T>& operator+=(T scalar) {
    uint32_t size = width * height;
    for (uint32_t i = 0; i < size; ++i) {
      data[i] += scalar;
    }
    return *this;
  }
  friend SimpleMat<T> operator+(SimpleMat<T> lhs, T scalar) {
    lhs += scalar;
    return lhs;
  }
  SimpleMat<T>& operator-=(T scalar) {
    uint32_t size = width * height;
    for (uint32_t i = 0; i < size; ++i) {
      data[i] -= scalar;
    }
    return *this;
  }
  friend SimpleMat<T> operator-(SimpleMat<T> lhs, T scalar) {
    lhs -= scalar;
    return lhs;
  }
  SimpleMat<T>& operator*=(T scalar) {
    uint32_t size = width * height;
    for (uint32_t i = 0; i < size; ++i) {
      data[i] *= scalar;
    }
    return *this;
  }
  friend SimpleMat<T> operator*(SimpleMat<T> lhs, T scalar) {
    lhs *= scalar;
    return lhs;
  }

  SimpleMat<T>& operator/=(T scalar) {
    assert(scalar != 0);
    uint32_t size = width * height;
    for (uint32_t i = 0; i < size; ++i) {
      data[i] /= scalar;
    }
    return *this;
  }
  friend SimpleMat<T> operator/(SimpleMat<T> lhs, T scalar) {
    lhs /= scalar;
    return lhs;
  }

  // 5. 賦值運算子 (Copy & Move Assignment)
  SimpleMat<T>& operator=(SimpleMat<T> other) {  // 使用 Copy-and-Swap 技巧
    swap(*this, other);
    return *this;
  }
  friend void swap(SimpleMat<T>& first, SimpleMat<T>& second) noexcept {
    using std::swap;
    swap(first.width, second.width);
    swap(first.height, second.height);
    swap(first.data, second.data);
  }

  // 2. 利用 += 來實作 + (純量加法)
  // 這裡傳入 lhs (left hand side) 時不加
  // &，是為了利用「移動建構子」自動複製一份新的

  // 元素個別乘法 functions
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