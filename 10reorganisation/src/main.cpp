#include <bits/stdc++.h>
#include <omp.h>

#include "../include/FFT.hpp"
#include "../include/HarrisCornerDetection.hpp"
#include "../include/Morphology.hpp"
#include "../include/NonLocalMeans.hpp"
#include "../include/SimpleImage.hpp"
#include "../include/SimpleMat.hpp"
#include "../include/Utility.hpp"
#include "../include/WhiteBalance.hpp"
#include "../include/sift.hpp"
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
void testsift() {
  SimpleImage *srcimg = new SimpleImage();
  SimpleImage *srcimg2 = new SimpleImage();

  struct timespec t_start, t_end;
  double elapsedTime;

  bool res = srcimg->Load("img/lena.bmp");
  srcimg2->Load("img/lena90degree.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);
  memcpy(dstimg->image, srcimg->image,
         (int)dstimg->width * (int)dstimg->height * (int)sizeof(uint8_t) * 3);
  SimpleImage *dstimg2 = new SimpleImage(srcimg2->width, srcimg2->height);
  memcpy(dstimg2->image, srcimg2->image,
         (int)dstimg2->width * (int)dstimg2->height * (int)sizeof(uint8_t) * 3);

  SimpleImage *dstimg3 =
      new SimpleImage(srcimg->width + srcimg2->width, srcimg->height);
  for (int i = 0; i < (int)dstimg3->width; i++) {
    for (int j = 0; j < (int)dstimg3->height; j++) {
      if (i < (int)srcimg->width) {
        for (int rgb = 0; rgb < 3; rgb++) {
          dstimg3->image[3 * (j * dstimg3->width + i) + rgb] =
              srcimg->image[3 * (j * srcimg->width + i) + rgb];
        }
      } else {
        int newi = i - srcimg->width;
        for (int rgb = 0; rgb < 3; rgb++) {
          dstimg3->image[3 * (j * dstimg3->width + i) + rgb] =
              srcimg2->image[3 * (j * srcimg->width + newi) + rgb];
        }
      }
    }
  }
  clock_gettime(CLOCK_REALTIME, &t_start);
  list<SiftKeypoint> kp;
  sift(srcimg->image, srcimg->width, srcimg->height, kp);
  list<SiftKeypoint> kp2;
  sift(srcimg2->image, srcimg2->width, srcimg2->height, kp2);
  list<MatchPair> match_list;
  match_keypoints(kp, kp2, match_list);

  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("elapsedTime: %lf ms\n", elapsedTime);
  cout << "kp nums:" << kp.size() << endl;

  draw_match_lines_to_image(dstimg3, match_list);
  draw_keypoints_image(dstimg, kp);
  draw_keypoints_image(dstimg2, kp2);
#if 0
  for (SiftKeypoint k : kp) {
    drawCircle(dstimg->image, dstimg->width, dstimg->height,
               k.xi * (exp2(k.octave)), k.yi * (exp2(k.octave)), 15,
               COLOR_GREEN);
  }
#endif
  dstimg->Save("img/sifttest.bmp");
  dstimg2->Save("img/sifttest2.bmp");
  dstimg3->Save("img/sifttest3.bmp");
  kp.clear();
  kp2.clear();
  match_list.clear();
  delete srcimg;
  delete srcimg2;
  delete dstimg;
  delete dstimg2;
  delete dstimg3;
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
    drawCircle(dstimg->image, dstimg->width, dstimg->height, c.x, c.y, 25,
               COLOR_RED);
  }
  dstimg->Save("img/chessAfterCornerPoint.bmp");
  delete srcimg;
  delete dstimg;
}
void testmomry2dvector() {
  int nGpyrLayers = 6;
  int nOctaves = 1;
  vector<vector<SimpleMat<float>>> guasspyr(
      nOctaves, vector<SimpleMat<float>>(nGpyrLayers));
  for (int i = 0; i < nOctaves; i++) {
    for (int j = 0; j < nGpyrLayers; j++) {
    }
  }
  for (int i = 0; i < nOctaves; i++) {
    for (int j = 0; j < nGpyrLayers; j++) {
      guasspyr[i][j].clear();
    }
  }
}
void testmomry1dvector() {
  int nGpyrLayers = 6;
  int nOctaves = 1;
  vector<SimpleMat<float>> guasspyr(nOctaves * nGpyrLayers);
}
void testmomrypureclass() {
  int nGpyrLayers = 6;
  int nOctaves = 1;
  SimpleMat<float> mat[nOctaves * nGpyrLayers];  // = new SimpleImage<float>;
}
void testFFT() {
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load("img/lena.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);
  uint8_t *grayimage = new uint8_t[srcimg->width * srcimg->height];

  colorimage2grayimage(srcimg->image, grayimage, srcimg->width, srcimg->height);
  double *input_real = new double[srcimg->width * srcimg->height];
  double *input_imag = new double[srcimg->width * srcimg->height];
  double *output_real = new double[srcimg->width * srcimg->height];
  double *output_imag = new double[srcimg->width * srcimg->height];
  double *image_shift = new double[srcimg->width * srcimg->height];
  double *image_log = new double[srcimg->width * srcimg->height];
  int width = srcimg->width;
  int height = srcimg->height;

  TypeConver(grayimage, input_real, (uint16_t)srcimg->width,
             (uint16_t)srcimg->height);

  FFT2D(input_real, input_imag, output_real, output_imag, width, height, 1,
        width, height);

  FFTShiftMagnitude(output_real, output_imag, image_shift, width, height);

  FFTShow(image_shift, image_log, width, height);

  TypeConver(image_log, grayimage, (uint16_t)srcimg->width,
             (uint16_t)srcimg->height);

  grayimage2colorimage(grayimage, dstimg->image, srcimg->width, srcimg->height);

  dstimg->Save("img/FFT.bmp");
  delete srcimg;
  delete dstimg;
}
void fastguassinafilter() {
  struct timespec t_start, t_end;
  double elapsedTime;
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load("img/InputImage01AfterGnoise.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }

  double *dst = new double[srcimg->width * srcimg->height];
  uint8_t *gray = new uint8_t[srcimg->width * srcimg->height];

  double *srcimage = new double[srcimg->width * srcimg->height];
  colorimage2grayimage(srcimg->image, gray, srcimg->width, srcimg->height);
  TypeConver(gray, srcimage, srcimg->width, srcimg->height);
  int windowsize = 13;
  double std = 5.0;
  double *gaussianfilter = new double[windowsize * windowsize];
  // 2D conv
  GaussianFilter(gaussianfilter, (uint16_t)windowsize, std);
  clock_gettime(CLOCK_REALTIME, &t_start);
  conv2D(srcimage, dst, srcimg->width, srcimg->height, gaussianfilter,
         (uint16_t)windowsize, conv2D_gray);
  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("2D conv elapsedTime: %lf ms\n", elapsedTime);
  // TypeConver(dst, gray, srcimg->width, srcimg->height);
  //  separable conv
  GuassianFilter1D(gaussianfilter, (uint16_t)windowsize, std);
  clock_gettime(CLOCK_REALTIME, &t_start);

  double *colconv = conv1D(srcimage, srcimg->width, srcimg->height,
                           gaussianfilter, windowsize, conv1D_col, conv2D_gray);
  dst = conv1D(colconv, srcimg->width, srcimg->height, gaussianfilter,
               windowsize, conv1D_row, conv2D_gray);
  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("separable conv elapsedTime: %lf ms\n", elapsedTime);
  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);
  TypeConver(dst, gray, srcimg->width, srcimg->height);
  grayimage2colorimage(gray, dstimg->image, srcimg->width, srcimg->height);
  dstimg->Save("img/separableconv.bmp");

  delete[] dst;
  delete[] gray;
  delete[] gaussianfilter;
  delete srcimg;
  delete dstimg;
}
int main() {
  cout << "start" << endl;
  fastguassinafilter();
  // testFFT();
  // testmat();
  //  testharriscornerdetection();
  // testsift();
  // testmomrypureclass();

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