#include <bits/stdc++.h>

#include "../include/NonLocalMeans.hpp"
#include "../include/SimpleImage.hpp"
#include "../include/Utility.hpp"
#include "../include/WhiteBalance.hpp"

void testRWbmp() {
  SimpleImage *img = new SimpleImage();
  bool res = img->Load("img/InputImage01.bmp");
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
int main() {
  cout << "start" << endl;
  testresize();
  // testedgedection();
  // testRWbmp();
  // testWhiteBalance();
  // testDeNoise();
  // testDeNoisewithIntegralImage();
  // testNLmeanswiththread();
  // testNLmeanswithintegralimagethread();
  // testcomparedimage();
  //  testDeNoise();

  return 0;
}