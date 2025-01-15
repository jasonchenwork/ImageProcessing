#include <bits/stdc++.h>
#include <omp.h>

#include "../include/HarrisCornerDetection.hpp"
#include "../include/Morphology.hpp"
#include "../include/NonLocalMeans.hpp"
#include "../include/SimpleImage.hpp"
#include "../include/SimpleMat.hpp"
#include "../include/Utility.hpp"
#include "../include/WhiteBalance.hpp"
void testRWbmp() {
  SimpleImage *img = new SimpleImage();
  bool res = img->Load("img/pattern.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  img->Save("img/test05.bmp");
  // cout << "res:" << res << endl;
  delete img;
}
void testWhiteBalance() {
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load("img/InputImage01.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);
  whitebalanceGWA(srcimg->image, dstimg->image, srcimg->width, srcimg->height);
  dstimg->Save("img/InputImage01AfterGWA.bmp");
  whitebalance2005Lam(srcimg->image, dstimg->image, srcimg->width,
                      srcimg->height);
  dstimg->Save("img/InputImage01After2005Lam.bmp");
  delete srcimg;
  delete dstimg;
}

void testcomparedimage() {
  SimpleImage *srcimg1 = new SimpleImage();
  srcimg1->Load("img/InputImage01AfterGnoise.bmp");
  SimpleImage *srcimg2 = new SimpleImage();
  srcimg2->Load("img/InputImage01AfterNLmeans.bmp");
  SimpleImage *dstimg = new SimpleImage(srcimg1->width, srcimg1->height);
  CompareImage(srcimg1->image, srcimg2->image, dstimg->image, srcimg1->width,
               srcimg1->height);

  dstimg->Save("img/differ.bmp");
  delete srcimg1;
  delete srcimg2;
  delete dstimg;
}
void testDeNoise() {
  struct timespec t_start, t_end;
  double elapsedTime;
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load("img/InputImage01AfterGnoise.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);
  clock_gettime(CLOCK_REALTIME, &t_start);
  NLmeans(srcimg->image, dstimg->image, srcimg->width, srcimg->height, 5, 3, 5);
  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("elapsedTime: %lf ms\n", elapsedTime);

  dstimg->Save("img/InputImage01AfterNLmeans.bmp");

  delete srcimg;
  delete dstimg;
}
void testDeNoisewithIntegralImage() {
  struct timespec t_start, t_end;
  double elapsedTime;
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load(
      "img/InputImage01AfterGnoise.bmp");  // pattern InputImage01AfterGnoise
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);
  clock_gettime(CLOCK_REALTIME, &t_start);
  NLmeanswithIntegralImage(srcimg->image, dstimg->image, srcimg->width,
                           srcimg->height, 5, 3, 5);
  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("elapsedTime: %lf ms\n", elapsedTime);

  dstimg->Save("img/InputImage01AfterNLmeans.bmp");

  delete srcimg;
  delete dstimg;
}
void testNLmeanswiththread() {
  struct timespec t_start, t_end;
  double elapsedTime;
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load("img/InputImage01AfterGnoise.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);
  clock_gettime(CLOCK_REALTIME, &t_start);
  NLmeanswiththreadmain(srcimg->image, dstimg->image, srcimg->width,
                        srcimg->height, 5, 3, 5);

  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("elapsedTime: %lf ms\n", elapsedTime);

  dstimg->Save("img/InputImage01AfterNLmeans.bmp");
  delete srcimg;
  delete dstimg;
}
void testNLmeanswithintegralimagethread() {
  struct timespec t_start, t_end;
  double elapsedTime;
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load("img/InputImage01AfterGnoise.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);

  clock_gettime(CLOCK_REALTIME, &t_start);
  NLmeanswithmixmain(srcimg->image, dstimg->image, srcimg->width,
                     srcimg->height, 5, 3, 5);

  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("elapsedTime: %lf ms\n", elapsedTime);

  dstimg->Save("img/InputImage01AfterNLmeans.bmp");
  delete srcimg;
  delete dstimg;
}
int filtersobel(vector<int> in) {
  double res = 0;
  for (auto i : in) {
    res += (double)i * (double)i;
  }
  return (int)sqrt(res);
}
void edgedetection(const uint8_t *imageSource, uint8_t *imageTarget,
                   uint16_t width, uint16_t height, int sobelthr) {
  uint8_t *grayimage = new uint8_t[width * height];
  uint8_t *edgeimage = new uint8_t[width * height];
  colorimage2grayimage(imageSource, grayimage, width, height);

  STfilter2D sobel;
  sobel.kernels = {{{-1, 0, 1, -2}, {-2, 0, 2}, {-1, 0, 1}},
                   {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}}

  };
  sobel.threshold = sobelthr;
  sobel.threshold_bypass = 0;
  sobel.filterfunc = filtersobel;

  filter2D(grayimage, edgeimage, width, height, sobel);

  grayimage2colorimage(edgeimage, imageTarget, width, height);
  delete grayimage;
}
void testedgedection() {
  struct timespec t_start, t_end;
  double elapsedTime;
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load("img/InputImage01.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);

  clock_gettime(CLOCK_REALTIME, &t_start);
  edgedetection(srcimg->image, dstimg->image, srcimg->width, srcimg->height,
                70);

  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("elapsedTime: %lf ms\n", elapsedTime);

  dstimg->Save("img/InputImage01Afteredgedetection.bmp");
}

void gaussiansmooth(uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
                    uint16_t height) {
  int windowsize = 13;
  double std = 1.0;
  double *gaussianfilter = new double[windowsize * windowsize];
  GaussianFilter(gaussianfilter, (uint16_t)windowsize, std);
  conv2D(imageSource, imageTarget, width, height, gaussianfilter,
         (uint16_t)windowsize, conv2D_color);
}
void testgaussianfilter() {
  struct timespec t_start, t_end;
  double elapsedTime;
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load("img/InputImage01AfterGnoise.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);

  clock_gettime(CLOCK_REALTIME, &t_start);
  gaussiansmooth(srcimg->image, dstimg->image, srcimg->width, srcimg->height);

  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("elapsedTime: %lf ms\n", elapsedTime);

  dstimg->Save("img/InputImage01AfterGsmooth.bmp");
}
void testmediafilter() {
  struct timespec t_start, t_end;
  double elapsedTime;
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load("img/InputImage01AfterGnoise.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);

  clock_gettime(CLOCK_REALTIME, &t_start);
  int filtersize = 7;
  MedianFilter(srcimg->image, dstimg->image, srcimg->width, srcimg->height,
               filtersize, conv2D_color);

  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("elapsedTime: %lf ms\n", elapsedTime);

  dstimg->Save("img/InputImage01AfterMedianFilter.bmp");
}
void testresize() {
  struct timespec t_start, t_end;
  double elapsedTime;
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load("img/InputImage01.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  int newwidth = srcimg->width * 2;
  int newheight = srcimg->height * 2;
  SimpleImage *dstimg = new SimpleImage(newwidth, newheight);

  clock_gettime(CLOCK_REALTIME, &t_start);

  imagescale(srcimg->image, dstimg->image, srcimg->width, srcimg->height,
             newwidth, newheight);
  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("elapsedTime: %lf ms\n", elapsedTime);

  dstimg->Save("img/InputImage01Afterresize.bmp");
}
void testDeNoisewithopenmp() {
  struct timespec t_start, t_end;
  double elapsedTime;
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load("img/InputImage01AfterGnoise.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);
  clock_gettime(CLOCK_REALTIME, &t_start);
  NLmeanswithopenmp(srcimg->image, dstimg->image, srcimg->width, srcimg->height,
                    5, 3, 5);
  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("elapsedTime: %lf ms\n", elapsedTime);

  dstimg->Save("img/InputImage01AfterNLmeans.bmp");

  delete srcimg;
  delete dstimg;
}

void testDeNoisewithIntegralImageopenmp() {
  struct timespec t_start, t_end;
  double elapsedTime;
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load(
      "img/InputImage01AfterGnoise.bmp");  // pattern InputImage01AfterGnoise
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);
  clock_gettime(CLOCK_REALTIME, &t_start);
  NLmeanswithIntegralImageopenmp(srcimg->image, dstimg->image, srcimg->width,
                                 srcimg->height, 5, 3, 5);
  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("elapsedTime: %lf ms\n", elapsedTime);

  dstimg->Save("img/InputImage01AfterNLmeansopenmp.bmp");

  delete srcimg;
  delete dstimg;
}
void genpaatern() {
  int width = 255;
  int height = 255;
  SimpleImage *dstimg = new SimpleImage(width, height);

  uint8_t *buf = new uint8_t[width * height];
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      buf[(width * (y) + x)] = (x) % 255;
    }
  }
#if 0
  // left top -> left bottom
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      dstimg->image[3 * (width * (y) + x) + 2] =
          buf[(width * (height - y) + (x))];
      dstimg->image[3 * (width * (y) + x) + 1] =
          buf[(width * (height - y) + (x))];
      dstimg->image[3 * (width * (y) + x) + 0] =
          buf[(width * (height - y) + (x))];
    }
  }
#else
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      dstimg->image[3 * (width * (y) + x) + 2] = buf[(width * (y) + (x))];
      dstimg->image[3 * (width * (y) + x) + 1] = buf[(width * (y) + (x))];
      dstimg->image[3 * (width * (y) + x) + 0] = buf[(width * (y) + (x))];
    }
  }
#endif

  dstimg->Save("img/pattern.bmp");
}
void testmorphology() {
  struct timespec t_start, t_end;
  double elapsedTime;
  SimpleImage *srcimg = new SimpleImage();
  uint16_t filtersize = 9;

  bool res = srcimg->Load("img/testMorphology.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);
  uint8_t *grayimage = new uint8_t[srcimg->width * srcimg->height];
  uint8_t *outimage = new uint8_t[srcimg->width * srcimg->height];
  clock_gettime(CLOCK_REALTIME, &t_start);
  colorimage2grayimage(srcimg->image, grayimage, srcimg->width, srcimg->height);
  TopHat(grayimage, outimage, srcimg->width, srcimg->height, filtersize);
  grayimage2colorimage(outimage, dstimg->image, srcimg->width, srcimg->height);
  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("elapsedTime: %lf ms\n", elapsedTime);
  delete grayimage;
  delete outimage;
  dstimg->Save("img/testMorphologyafterTopHat.bmp");
}
void testhistogramequalization() {
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load("img/InputImage01.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);
  HistogramEqualization(srcimg->image, dstimg->image, srcimg->width,
                        srcimg->height);
  dstimg->Save("img/InputImage01Afteristogramequalization.bmp");
  delete srcimg;
  delete dstimg;
}
void testharriscornerdetection() {
  SimpleImage *srcimg = new SimpleImage();
  struct timespec t_start, t_end;
  double elapsedTime;

  bool res = srcimg->Load("img/chess.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);
  memcpy(dstimg->image, srcimg->image,
         (int)dstimg->width * (int)dstimg->height * (int)sizeof(uint8_t) * 3);
  clock_gettime(CLOCK_REALTIME, &t_start);
  list<cornerpoint> corners;
  HarrisCornerArg harrisarg;
  harrisarg.alpha = 0.05;
  harrisarg.threshold = 100000;
  HarrisCornerDetection(srcimg->image, srcimg->width, srcimg->height, corners,
                        harrisarg);
  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("elapsedTime: %lf ms\n", elapsedTime);

  cout << "corners nums:" << corners.size() << endl;

  for (cornerpoint c : corners) {
    drawCircle(dstimg->image, dstimg->width, dstimg->height, c.x, c.y, 25);
  }
  dstimg->Save("img/chessAfterCornerPoint.bmp");
  delete srcimg;
  delete dstimg;
}
int main() {
  cout << "start" << endl;
  testharriscornerdetection();
  // testsift();
  //  testhistogramequalization();
  //   testmorphology();
  //    genpaatern();
  //     testgaussianfilter();
  //     testmediafilter();
  //    testDeNoisewithopenmp();               // 6498.930026 ms
  //    testDeNoisewithIntegralImageopenmp();  // 2790.592246 ms
  //    testNLmeanswiththread();               // 8965.213364 ms
  //    testNLmeanswithintegralimagethread();  // 3427.264040 ms
  //       testresize();
  //       testedgedection();
  //    testRWbmp();
  //      testWhiteBalance();
  //    testDeNoise();
  //    testDeNoisewithIntegralImage();

  // testcomparedimage();
  //  testDeNoise();

  return 0;
}