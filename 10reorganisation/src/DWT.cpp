#include "../include/DWT.hpp"

template <typename T1>
void DWT(T1 *imageSource, T1 *imageTarget, int width, int size) {
  int x, y, i, j;
  int offset = width / 2;

  int *image_TMP = new int[width * width];  // image array

  // horization
  for (y = 0; y < size; y++) {
    for (x = 0; x < size / 2; x++)
      image_TMP[width * y + x] = (imageSource[width * y + 2 * x] +
                                  imageSource[width * y + (2 * x + 1)]) /
                                 2;

    for (x = 0; x < size / 2; x++)
      image_TMP[width * y + (x + size / 2)] =
          ((imageSource[width * y + 2 * (x)] -
            imageSource[width * y + (2 * (x) + 1)])) /
          2;
  }
#if 0
  for (y = 0; y < size; y++) {
    for (x = 0; x < size; x++) {
      image_TMP[width * y + x] = imageTarget[width * y + x];
    }
  }
#endif
#if 1
  // vertical
  for (y = 0; y < size; y++) {
    for (x = 0; x < size / 2; x++) {
      imageTarget[width * (x) + y] = (image_TMP[width * (2 * x) + y] +
                                      image_TMP[width * (2 * x + 1) + y]) /
                                     2;
    }
    for (x = 0; x < size / 2; x++) {
      imageTarget[width * (x + size / 2) + y] =
          (image_TMP[width * (2 * x) + y] -
           image_TMP[width * (2 * x + 1) + y]) /
          2;
    }
  }
#endif
  delete[] image_TMP;
}
// Explicit Instantiation
template void DWT(double *imageSource, double *imageTarget, int width,
                  int size);

template <typename T1>
void IDWT(T1 *imageSource, T1 *imageTarget, int width, int size) {
  int x, y;
  for (x = 0; x < size / 2; x++) {
    for (y = 0; y < size / 2; y++) {
      imageTarget[width * (2 * y) + 2 * x] =
          imageSource[width * y + x] +
          imageSource[width * (y + size / 2) + (x)] +
          imageSource[width * (y) + (x + size / 2)] +
          imageSource[width * (y + size / 2) + (x + size / 2)];
      imageTarget[width * (2 * (y)) + (2 * x + 1)] =
          imageSource[width * y + x] -
          imageSource[width * (y + size / 2) + (x)] +
          imageSource[width * (y) + (x + size / 2)] -
          imageSource[width * (y + size / 2) + (x + size / 2)];
      imageTarget[width * (2 * y + 1) + (2 * x)] =
          imageSource[width * y + x] +
          imageSource[width * (y + size / 2) + (x)] -
          imageSource[width * (y) + (x + size / 2)] -
          imageSource[width * (y + size / 2) + (x + size / 2)];
      imageTarget[width * (2 * y + 1) + (2 * x + 1)] =
          imageSource[width * y + x] -
          imageSource[width * (y + size / 2) + (x)] -
          imageSource[width * (y) + (x + size / 2)] +
          imageSource[width * (y + size / 2) + (x + size / 2)];
    }
  }
}
template <typename T1>
void IDWT(double *imageSource, double *imageTarget, int width, int size);