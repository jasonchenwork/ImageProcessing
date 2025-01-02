#include "../include/Morphology.hpp"

void Erode(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
           uint16_t height, uint16_t filtersize) {
  int halfkernelsize = filtersize / 2;

  if (halfkernelsize < 1) {
    return;  // do nothing
  }

  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      uint8_t res = imageSource[(width * y + x)];
      for (int my = y - halfkernelsize; my <= y + halfkernelsize; ++my) {
        if (my < 0 || my >= height) continue;
        for (int mx = x - halfkernelsize; mx <= x + halfkernelsize; ++mx) {
          if (mx < 0 || mx >= width) continue;
          res &= imageSource[(width * my + mx)];
        }
      }
      imageTarget[(width * y + x)] = res;
    }
  }
}
void Dilation(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
              uint16_t height, uint16_t filtersize) {
  int halfkernelsize = filtersize / 2;

  if (halfkernelsize < 1) {
    return;  // do nothing
  }

  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      uint8_t res = imageSource[(width * y + x)];
      for (int my = y - halfkernelsize; my <= y + halfkernelsize; ++my) {
        if (my < 0 || my >= height) continue;
        for (int mx = x - halfkernelsize; mx <= x + halfkernelsize; ++mx) {
          if (mx < 0 || mx >= width) continue;
          res |= imageSource[(width * my + mx)];
        }
      }
      imageTarget[(width * y + x)] = res;
    }
  }
}

void Opening(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
             uint16_t height, uint16_t filtersize) {
  uint8_t *tmpimage = new uint8_t[width * height];
  Erode(imageSource, tmpimage, width, height, filtersize);
  Dilation(tmpimage, imageTarget, width, height, filtersize);
  delete tmpimage;
}

void Closing(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
             uint16_t height, uint16_t filtersize) {
  uint8_t *tmpimage = new uint8_t[width * height];
  Dilation(imageSource, tmpimage, width, height, filtersize);
  Erode(tmpimage, imageTarget, width, height, filtersize);
  delete tmpimage;
}
void diffimage(uint8_t *imageSource1, uint8_t *imageSource2,
               uint8_t *imageTarget, uint16_t width, uint16_t height) {
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      imageTarget[width * y + x] = (uint8_t)abs(imageSource1[width * y + x] -
                                                imageSource2[width * y + x]);
    }
  }
}
void MorphologicalGradient(uint8_t *imageSource, uint8_t *imageTarget,
                           uint16_t width, uint16_t height,
                           uint16_t filtersize) {
  uint8_t *dilation = new uint8_t[width * height];
  uint8_t *erode = new uint8_t[width * height];

  Dilation(imageSource, dilation, width, height, filtersize);
  Erode(imageSource, erode, width, height, filtersize);
  diffimage(dilation, erode, imageTarget, width, height);
  delete dilation;
  delete erode;
}
void TopHat(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
            uint16_t height, uint16_t filtersize) {
  uint8_t *opening = new uint8_t[width * height];
  Opening(imageSource, opening, width, height, filtersize);
  diffimage(imageSource, opening, imageTarget, width, height);
  delete opening;
}
void BlackHat(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
              uint16_t height, uint16_t filtersize) {
  uint8_t *closing = new uint8_t[width * height];
  Closing(imageSource, closing, width, height, filtersize);
  diffimage(imageSource, closing, imageTarget, width, height);
  delete closing;
}