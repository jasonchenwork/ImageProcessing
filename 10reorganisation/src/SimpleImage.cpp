#include "../include/SimpleImage.hpp"

SimpleImage::SimpleImage() {

};
SimpleImage::~SimpleImage() {
  delete[] image;
  image = nullptr;
};
SimpleImage::SimpleImage(uint32_t w, uint32_t h) {
  bmpheaderINIT();
  width = w;
  height = h;
  image = new uint8_t[width * height * 3 *
                      sizeof(uint8_t)]();  //(uint8_t *)malloc((size_t)width *
                                           // height * 3 * sizeof(uint8_t));
}

void SimpleImage::bmpheaderINIT() {
  memset(&bmpf_h, (uint8_t)0, sizeof(bmpf_h));
  bmpf_h.B_H.identity[0] = 'B';
  bmpf_h.B_H.identity[1] = 'M';
  bmpf_h.F_H.data_offset = 54;
  bmpf_h.F_Info.header_size = 40;
  bmpf_h.F_Info.planes = 1;
  bmpf_h.F_Info.bit_per_pixel = 24;
}

bool SimpleImage::Load(const char *fname_s) {
  FILE *fp_s = NULL;  // source file handler
  int Mod4;
  uint8_t padding[3 * 3] = {0};

  fp_s = fopen(fname_s, "rb");
  if (fp_s == NULL) {
    printf("fopen fname_t error\n");
    return false;
  }

  fread(&bmpf_h, sizeof(struct BMPFileHeader), 1, fp_s);

  if (bmpf_h.F_Info.bit_per_pixel != 24) return false;  // only support 24 bit

  width = bmpf_h.F_Info.width;
  height = bmpf_h.F_Info.height;
  Mod4 = (width * 3) % 4;

  fseek(fp_s, bmpf_h.F_H.data_offset, SEEK_SET);
  image = new uint8_t[width * height * 3 *
                      sizeof(uint8_t)];  //(uint8_t *)malloc((size_t)width *
                                         // height * 3 * sizeof(uint8_t));

  for (uint32_t row = 0; row < height; row++) {
    uint32_t findrow = height - 1 - row;
    fread(image + (findrow * 3 * width), sizeof(uint8_t),
          (size_t)(width * 3 * sizeof(uint8_t)), fp_s);
    if (Mod4 != 0) {
      fread(padding, sizeof(uint8_t), (size_t)(4 - Mod4), fp_s);
    }
  }

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

bool SimpleImage::Save(const char *fname_s) {
  FILE *fp_s = NULL;  // source file handler
  int Mod4;
  int savewidth;
  uint8_t padding[3 * 3] = {0};

  fp_s = fopen(fname_s, "wb");

  if (fp_s == NULL) {
    printf("fopen fname_t error\n");
    return false;
  }

  Mod4 = (width * 3) % 4;

  if (Mod4 != 0) {
    savewidth =
        (((int)((width - Mod4) / 4)) + 1) * 4;  // width + (4 - Mod4);  //
  } else {
    savewidth = width;
  }
  savewidth = width;
  uint32_t rgb_raw_data_offset = bmpf_h.F_H.data_offset;
  uint32_t file_size = savewidth * height * 3 + rgb_raw_data_offset;
  bmpf_h.F_H.file_size = file_size;
  bmpf_h.F_Info.width = savewidth;  // width;
  bmpf_h.F_Info.height = height;
  bmpf_h.F_Info.data_size = savewidth * height * 3;
  // write header
#if 0
    cout << "rgb_raw_data_offset:" << rgb_raw_data_offset << endl;
    cout << "bmpf_h:" << sizeof(bmpf_h) << endl;
#endif
  fwrite(&bmpf_h, sizeof(bmpf_h), 1, fp_s);
  for (uint32_t row = 0; row < height; row++) {
    uint32_t findrow = height - 1 - row;
    fwrite(image + (findrow * 3 * width), sizeof(uint8_t), (size_t)(width * 3),
           fp_s);
    if (Mod4 != 0) {
      fwrite(padding, sizeof(uint8_t), (size_t)(4 - Mod4), fp_s);
    }
  }

  fclose(fp_s);
  return true;
}