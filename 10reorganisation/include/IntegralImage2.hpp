

class IntegralImage2 {
 private:
 public:
  IntegralImage2(int width, int height);
  ~IntegralImage2();

  int** nSumImage;  // normal  integral image
  int** sSumImage;  // squared integral image

  double* nSum;  // normal  integral image
  double* sSum;  // squared integral image

  int width;
  int height;
  int nWidth;
  int nHeight;

  void FromImage(unsigned char** Src);
  double GetSum(int x, int y, int width, int height);
  double GetSum2(int x, int y, int width, int height);
};
