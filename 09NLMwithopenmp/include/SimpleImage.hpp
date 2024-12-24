#ifndef SAMPLEIMAGE_HPP
#define SAMPLEIMAGE_HPP

#include <bits/stdc++.h>

#include <iostream>
using namespace std;

struct BITMAPFILE {
  uint8_t identity[2];  // 2 byte : "BM" is BMP
};

struct BITMAPFILEHEADER {
  uint32_t file_size;    // 4 byte : file size
  uint32_t reserved;     // 4 byte
  uint32_t data_offset;  // 4 byte :
};
struct BITMAPINFOHEADER {
  uint32_t header_size;       // 4 byte :
  uint32_t width;             // 4 byte :  (pixel)
  uint32_t height;            // 4 byte :  (pixel)
  uint16_t planes;            // 2 byte :
  uint16_t bit_per_pixel;     // 2 byte :
  uint32_t compression;       // 4 byte :
  uint32_t data_size;         // 4 byte :
  uint32_t hresolution;       // 4 byte : pixel/m
  uint32_t vresolution;       // 4 byte : pixel/m
  uint32_t used_colors;       // 4 byte :
  uint32_t important_colors;  // 4 byte :
};
;
struct PALLETTE {
  uint8_t blue;      // 1 byte :
  uint8_t green;     // 1 byte :
  uint8_t red;       // 1 byte :
  uint8_t reserved;  // 1 byte :
};
struct BMPFileHeader {
  struct BITMAPFILE B_H;
  struct BITMAPFILEHEADER F_H;
  struct BITMAPINFOHEADER F_Info;
  // struct PALLETTE p_;
} __attribute__((packed));
class SimpleImage {
 private:
  void bmpheaderINIT();

 public:
  struct BMPFileHeader bmpf_h;
  uint32_t width;
  uint32_t height;
  uint8_t *image;
  SimpleImage();
  SimpleImage(uint32_t w, uint32_t h);
  ~SimpleImage();

  bool Load(const char *fname_s);
  bool Save(const char *fname_s);
};
#endif