
#ifndef INTEGRALIMAGE2_HPP
#define INTEGRALIMAGE2_HPP

class IntegralImage2 {
 private:
 public:
  IntegralImage2(int width, int height);
  IntegralImage2();
  ~IntegralImage2();

  double** nSumImage;  // normal  integral image
  double** sSumImage;  // squared integral image

  double* nSum;  // normal  integral image
  double* sSum;  // squared integral image

  int width;
  int height;
  int nWidth;
  int nHeight;
  template <typename T>
  void FromImage(T** Src) {
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
  void INIT(int width, int height);
  double GetSum(int x, int y, int width, int height);
  double GetSum2(int x, int y, int width, int height);
};
#endif
