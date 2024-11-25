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
  void bmpheaderINIT() {
    memset(&bmpf_h, (uint8_t)0, sizeof(bmpf_h));
    bmpf_h.B_H.identity[0] = 'B';
    bmpf_h.B_H.identity[1] = 'M';
    bmpf_h.F_H.data_offset = 54;
    bmpf_h.F_Info.header_size = 40;
    bmpf_h.F_Info.planes = 1;
    bmpf_h.F_Info.bit_per_pixel = 24;
  }

 public:
  struct BMPFileHeader bmpf_h;
  uint32_t width;
  uint32_t height;
  uint8_t *image;
  SimpleImage() {}
  SimpleImage(uint32_t w, uint32_t h) {
    bmpheaderINIT();
    width = w;
    height = h;
    image = (uint8_t *)malloc((size_t)width * height * 3 * sizeof(uint8_t));
  }
  /*
   for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
          image[(width * i + j) * 3 + 0] = (uint8_t)0;
          image[(width * i + j) * 3 + 1] = (uint8_t)0;
          image[(width * i + j) * 3 + 2] = (uint8_t)255;
        }
      }
  */
  bool Load(const char *fname_s) {
    FILE *fp_s = NULL;  // source file handler

    fp_s = fopen(fname_s, "rb");
    if (fp_s == NULL) {
      printf("fopen fname_t error\n");
      return false;
    }

    fread(&bmpf_h, sizeof(struct BMPFileHeader), 1, fp_s);

    width = bmpf_h.F_Info.width;
    height = bmpf_h.F_Info.height;
    fseek(fp_s, bmpf_h.F_H.data_offset, SEEK_SET);
    image = (uint8_t *)malloc((size_t)width * height * 3 * sizeof(uint8_t));

    fread(image, sizeof(uint8_t),
          (size_t)(width * height * 3 * sizeof(uint8_t)), fp_s);
#if 0
    // debug
    printf("Label: %c %c \n", bmpf_h.B_H.identity[0], bmpf_h.B_H.identity[1]);

    printf("File size: %d bytes \n", bmpf_h.F_H.file_size);
    printf("reserved: %d \n", bmpf_h.F_H.reserved);
    printf("offset: %d \n", bmpf_h.F_H.data_offset);

    printf("header size: %d  byte \n", bmpf_h.F_Info.header_size);
    printf("width: %d \n", bmpf_h.F_Info.width);
    printf("height: %d \n", bmpf_h.F_Info.height);
    printf("planes: %d \n", bmpf_h.F_Info.planes);
    printf("bit_per_pixel: %d pixel \n", bmpf_h.F_Info.bit_per_pixel);
    printf("compress: %d  \n", bmpf_h.F_Info.compression);
    printf("data_size: %d \n", bmpf_h.F_Info.data_size);
    printf("H-Resolution: %d \n", bmpf_h.F_Info.hresolution);
    printf("V-Resolution: %d \n", bmpf_h.F_Info.vresolution);
    printf("Used Color: %d \n", bmpf_h.F_Info.used_colors);
    printf("important colors %d \n", bmpf_h.F_Info.important_colors);
#endif
    fclose(fp_s);
    return true;
  }
  bool Save(const char *fname_s) {
    FILE *fp_s = NULL;  // source file handler

    fp_s = fopen(fname_s, "wb");

    if (fp_s == NULL) {
      printf("fopen fname_t error\n");
      return false;
    }
    uint32_t rgb_raw_data_offset = bmpf_h.F_H.data_offset;
    uint32_t file_size = width * height * 3 + rgb_raw_data_offset;
    bmpf_h.F_H.file_size = file_size;
    bmpf_h.F_Info.width = width;
    bmpf_h.F_Info.height = height;
    bmpf_h.F_Info.data_size = width * height * 3;
    // write header
#if 0
    cout << "rgb_raw_data_offset:" << rgb_raw_data_offset << endl;
    cout << "bmpf_h:" << sizeof(bmpf_h) << endl;
#endif
    fwrite(&bmpf_h, sizeof(bmpf_h), 1, fp_s);

    fwrite(image, sizeof(uint8_t), (size_t)(width * height * 3), fp_s);

    fclose(fp_s);
    return true;
  }
};

void testRWbmp() {
  SimpleImage *img = new SimpleImage();
  bool res = img->Load("Lenna.bmp");
  img->Save("test05.bmp");
  // cout << "res:" << res << endl;
}

int main() {
  testRWbmp();

  return 0;
}