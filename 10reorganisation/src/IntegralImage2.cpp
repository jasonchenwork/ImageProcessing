
#include "../include/IntegralImage2.hpp"

void Creat2Darray(int*** StoSpc, int Width, int Height) {
  (*StoSpc) = new int*[Height];
  (*StoSpc)[0] = new int[Width * Height];

  int j;
  int GapSize = Width;
  int Offset = 0;
  for (j = 1; j < Height; j++) {
    Offset = Offset + GapSize;
    (*StoSpc)[j] = (((*StoSpc)[0]) + Offset);
  }
}

void Delete2Darray(int** arr, int y) {
  for (int i = 0; i < y; i++) {
    delete[] arr[i];
  }
  delete[] arr;
}

IntegralImage2::IntegralImage2(int width, int height) {
  this->width = width;
  this->height = height;

  this->nWidth = width + 1;
  this->nHeight = height + 1;

  nSum = new double[this->nWidth * this->nHeight];
  sSum = new double[this->nWidth * this->nHeight];
  for (int i = 0; i < this->nWidth * this->nHeight; i++) {
    nSum[i] = sSum[i] = 0;
  }
};

// release source
IntegralImage2::~IntegralImage2() {
  delete[] nSum;
  delete[] sSum;
};

void IntegralImage2::FromImage(unsigned char** Src) {
  for (int y = 1; y < this->height; y++) {
    int yy = this->nWidth * (y);
    int y1 = this->nWidth * (y - 1);

    for (int x = 1; x < this->width; x++) {
      double p1 = Src[y][x];
      double p2 = p1 * p1;

      int r = yy + (x);
      int a = yy + (x - 1);
      int b = y1 + (x);
      int c = y1 + (x - 1);

      nSum[r] = p1 + nSum[a] + nSum[b] - nSum[c];
      sSum[r] = p2 + sSum[a] + sSum[b] - sSum[c];
    }
  }
};

double IntegralImage2::GetSum(int x, int y, int width, int height) {
  int a = nWidth * (y) + (x);
  int b = nWidth * (y + height) + (x + width);
  int c = nWidth * (y + height) + (x);
  int d = nWidth * (y) + (x + width);

  return nSum[a] + nSum[b] - nSum[c] - nSum[d];
};

double IntegralImage2::GetSum2(int x, int y, int width, int height) {
  int a = nWidth * (y) + (x);
  int b = nWidth * (y + height) + (x + width);
  int c = nWidth * (y + height) + (x);
  int d = nWidth * (y) + (x + width);

  return sSum[a] + sSum[b] - sSum[c] - sSum[d];
};
