#include <bits/stdc++.h>

#include <iostream>

using namespace std;
/// mutex u_mutex;
// condition_variable cv;
// int threadsReady = 0;  // 記錄已到達同步點的執行緒數
int totalThreads = 0;

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
  SimpleImage() {};
  ~SimpleImage() {
    delete[] image;
    image = nullptr;
  };
  SimpleImage(uint32_t w, uint32_t h) {
    bmpheaderINIT();
    width = w;
    height = h;
    image = new uint8_t[width * height * 3 *
                        sizeof(uint8_t)];  //(uint8_t *)malloc((size_t)width *
                                           // height * 3 * sizeof(uint8_t));
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
    int Mod4;

    fp_s = fopen(fname_s, "rb");
    if (fp_s == NULL) {
      printf("fopen fname_t error\n");
      return false;
    }

    fread(&bmpf_h, sizeof(struct BMPFileHeader), 1, fp_s);

    width = bmpf_h.F_Info.width;
    height = bmpf_h.F_Info.height;
    Mod4 = width % 4;

    if (Mod4 != 0) {
      width = (((int)((width - Mod4) / 4)) + 1) * 4;
    }
    fseek(fp_s, bmpf_h.F_H.data_offset, SEEK_SET);
    image = new uint8_t[width * height * 3 *
                        sizeof(uint8_t)];  //(uint8_t *)malloc((size_t)width *
                                           // height * 3 * sizeof(uint8_t));

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
    int Mod4;

    fp_s = fopen(fname_s, "wb");

    if (fp_s == NULL) {
      printf("fopen fname_t error\n");
      return false;
    }
    Mod4 = width % 4;

    if (Mod4 != 0) {
      width = (((int)((width - Mod4) / 4)) + 1) * 4;
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
void GenDifferIntegralImage(const uint8_t *imageSource, double *imageTarget,
                            uint16_t width, uint16_t height, int dx, int dy,
                            int RGB) {
  double *buf = new double[width * height];
  for (int i = 0; i < width * height; i++) {
    imageTarget[i] = 0;
    buf[i] = 0;
  }
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int tx = x + dx;
      int ty = y + dy;
      if (tx < 0 || tx >= width || ty < 0 || ty >= height) continue;
      double tmp = (double)imageSource[3 * ((y)*width + (x)) + RGB] -
                   (double)imageSource[3 * ((ty)*width + (tx)) + RGB];

      buf[y * width + x] = tmp * tmp;
    }
  }

  imageTarget[0] = buf[0];

  for (int x = 1; x < width; x++) {
    imageTarget[x] = buf[x];
  }
  for (int y = 1; y < height; y++) {
    imageTarget[y * width] = buf[y * width];
  }
  for (int y = 1; y < height; y++) {
    for (int x = 1; x < width; x++) {
      imageTarget[y * width + x] = buf[(y)*width + (x)] +
                                   imageTarget[y * width + (x - 1)] +
                                   imageTarget[(y - 1) * width + x] -
                                   imageTarget[(y - 1) * width + (x - 1)];
    }
  }
  delete[] buf;
}
double GetSumFromIntegralImg(double *imageTarget, uint16_t width,
                             uint16_t height, int16_t x1, int16_t y1,
                             int16_t x2, int16_t y2) {
  if (x2 >= width) x2 = width - 1;
  if (y2 >= height) y2 = height - 1;
  if (x2 < 0) x2 = 0;
  if (y2 < 0) y2 = 0;

  if (x1 >= width) x1 = width - 1;
  if (y1 >= height) y1 = height - 1;
  if (x1 < 0) x1 = 0;
  if (y1 < 0) y1 = 0;

  double sum = imageTarget[y2 * width + x2];

  if (x1 >= 1) {
    sum -= imageTarget[y2 * width + (x1 - 1)];
  }
  if (y1 >= 1) {
    sum -= imageTarget[(y1 - 1) * width + (x2)];
  }
  if (x1 >= 1 && y1 >= 1) {
    sum += imageTarget[(y1 - 1) * width + (x1 - 1)];
  }

  return sum;
}

void GenIntegralImage(const uint8_t *imageSource, uint64_t *imageTarget,
                      uint16_t width, uint16_t height, int RGB) {
  // int newwidth = width + 1;
  // int newheight = height + 1;

  imageTarget[0] = imageSource[RGB];

  for (int x = 1; x < width; x++) {
    (imageTarget)[x] =
        (imageTarget)[x - 1] + (uint64_t)imageSource[3 * ((x)) + RGB] *
                                   (uint64_t)imageSource[3 * ((x)) + RGB];
  }
  for (int y = 1; y < height; y++) {
    (imageTarget)[y * width] = (imageTarget)[(y - 1) * width] +
                               (uint64_t)imageSource[3 * ((y)*width) + RGB] *
                                   (uint64_t)imageSource[3 * ((y)*width) + RGB];
  }
  for (int y = 1; y < height; y++) {
    for (int x = 1; x < width; x++) {
      (imageTarget)[y * width + x] =
          (uint64_t)imageSource[3 * ((y - 1) * width + (x - 1)) + RGB] *
              (uint64_t)imageSource[3 * ((y - 1) * width + (x - 1)) + RGB] +
          (imageTarget)[y * width + (x - 1)] +
          (imageTarget)[(y - 1) * width + x] -
          (imageTarget)[(y - 1) * width + (x - 1)];
    }
  }
}
struct NLmeansParam {
  uint8_t *imageSource;
  uint8_t *imageTarget;
  uint16_t width;
  uint16_t height;
  double h;
  uint8_t halfkernelsize;
  uint8_t halfsearch;
  uint16_t width_start;
  uint16_t width_end;
  double *sumR;
  double *sumG;
  double *sumB;
  double *weightsumR;
  double *weightsumG;
  double *weightsumB;
  double *weightmaxR;
  double *weightmaxG;
  double *weightmaxB;
  uint8_t x_start;
  vector<pair<int, int>> w;
};
std::atomic<int> threadsReady(0);
void NLmeanswithIntegralImageTask(const struct NLmeansParam param) {
  // unique_lock<std::mutex> lock(u_mutex);
  uint8_t *imageSource = param.imageSource;
  uint8_t *imageTarget = param.imageTarget;
  uint16_t width = param.width;
  uint16_t height = param.height;
  double h = param.h;
  uint8_t halfkernelsize = param.halfkernelsize;
  uint8_t halfsearch = param.halfsearch;
  uint16_t width_start = param.width_start;
  uint16_t width_end = param.width_end;
  uint16_t x_start = param.x_start;
  double h2 = (h * 2.0 + 1) * (h * 2.0 + 1) * 3;
  double sigma2 = 5 * 5;

  double *sumR = param.sumR;
  double *sumG = param.sumG;
  double *sumB = param.sumB;
  double *weightsumR = param.weightsumR;
  double *weightsumG = param.weightsumG;
  double *weightsumB = param.weightsumB;
  double *weightmaxR = param.weightmaxR;
  double *weightmaxG = param.weightmaxG;
  double *weightmaxB = param.weightmaxB;
  double *integralR = new double[(width) * (height)];
  double *integralG = new double[(width) * (height)];
  double *integralB = new double[(width) * (height)];

  for (auto cur : param.w) {
    int wx = cur.first;
    int wy = cur.second;

    GenDifferIntegralImage(imageSource, integralR, width, height, wx, wy, 2);
    GenDifferIntegralImage(imageSource, integralG, width, height, wx, wy, 1);
    GenDifferIntegralImage(imageSource, integralB, width, height, wx, wy, 0);

    for (int vx = 0; vx < width; ++vx) {
      int x = (vx + x_start) % width;
      if (x + wx < 0 || x + wx >= width) continue;
      for (int y = 0; y < height; ++y) {
        if (y + wy < 0 || y + wy >= height) continue;

        double distR = 0, distG = 0, distB = 0;

#if 0
          double distR0 = 0, distG0 = 0, distB0 = 0;
          for (int kx = -halfkernelsize; kx <= halfkernelsize; kx++) {
            for (int ky = -halfkernelsize; ky <= halfkernelsize; ky++) {
              int nx = x + kx, ny = y + ky;
              int nwx = wx + nx, nwy = wy + ny;

              if (nx < 0 || nx >= width || ny < 0 || ny >= height || nwx < 0 ||
                  nwx >= width || nwy < 0 || nwy >= height)
                continue;

              int curIndex = 3 * (width * ny + nx);
              int targetIndex = 3 * (width * nwy + nwx);

              double curR = *(imageSource + curIndex + 2);
              double curG = *(imageSource + curIndex + 1);
              double curB = *(imageSource + curIndex + 0);

              double targetR = *(imageSource + targetIndex + 2);
              double targetG = *(imageSource + targetIndex + 1);
              double targetB = *(imageSource + targetIndex + 0);

              distR0 += (curR - targetR) * (curR - targetR);
              distG0 += (curG - targetG) * (curG - targetG);
              distB0 += (curB - targetB) * (curB - targetB);
            }
          }
#else
        distR = GetSumFromIntegralImg(integralR, width, height,
                                      (int16_t)x - (int16_t)halfkernelsize,
                                      (int16_t)y - (int16_t)halfkernelsize,
                                      (int16_t)x + (int16_t)halfkernelsize,
                                      (int16_t)y + (int16_t)halfkernelsize);
        distG = GetSumFromIntegralImg(integralG, width, height,
                                      (int16_t)x - (int16_t)halfkernelsize,
                                      (int16_t)y - (int16_t)halfkernelsize,
                                      (int16_t)x + (int16_t)halfkernelsize,
                                      (int16_t)y + (int16_t)halfkernelsize);
        distB = GetSumFromIntegralImg(integralB, width, height,
                                      (int16_t)x - (int16_t)halfkernelsize,
                                      (int16_t)y - (int16_t)halfkernelsize,
                                      (int16_t)x + (int16_t)halfkernelsize,
                                      (int16_t)y + (int16_t)halfkernelsize);
#endif
#if 1
        distR /= (2.0 * halfkernelsize + 1.0) * (2.0 * halfkernelsize + 1.0);
        distG /= (2.0 * halfkernelsize + 1.0) * (2.0 * halfkernelsize + 1.0);
        distB /= (2.0 * halfkernelsize + 1.0) * (2.0 * halfkernelsize + 1.0);
#endif
#if 0
          distR = max(distR - 2 * sigma2, 0.0);
          distG = max(distG - 2 * sigma2, 0.0);
          distB = max(distB - 2 * sigma2, 0.0);
#endif

        double weightR = exp(-distR / h2);
        double weightG = exp(-distG / h2);
        double weightB = exp(-distB / h2);

        int targetIndex = 3 * (width * (wy + y) + (wx + x));
        // u_mutex.lock();
        sumR[y * width + x] += weightR * (*(imageSource + targetIndex + 2));
        sumG[y * width + x] += weightG * (*(imageSource + targetIndex + 1));
        sumB[y * width + x] += weightB * (*(imageSource + targetIndex + 0));

        weightsumR[y * width + x] += weightR;
        weightsumG[y * width + x] += weightG;
        weightsumB[y * width + x] += weightB;

        weightmaxR[y * width + x] = max(weightmaxR[y * width + x], weightR);
        weightmaxG[y * width + x] = max(weightmaxG[y * width + x], weightG);
        weightmaxB[y * width + x] = max(weightmaxB[y * width + x], weightB);
        // u_mutex.unlock();
      }
    }
#if 0
    threadsReady++;
    if (threadsReady == totalThreads) {
      cv.notify_all();  // 通知所有執行緒繼續
    } else {
      cv.wait(lock);  // 等待其他執行緒
    }
    threadsReady = 0;
#endif
#if 0
    threadsReady.fetch_add(1, std::memory_order_relaxed);

    while (threadsReady.load(std::memory_order_relaxed) < totalThreads) {
      // std::this_thread::sleep_for(std::chrono::microseconds(14));
    }
    threadsReady.store(0, std::memory_order_relaxed);
#endif
  }
  delete[] integralR;
  delete[] integralG;
  delete[] integralB;
}

void NLmeanswiththreadmain(uint8_t *imageSource, uint8_t *imageTarget,
                           uint16_t width, uint16_t height, double h,
                           uint8_t halfkernelsize, uint8_t halfsearch) {
  int threadN = 8;
  totalThreads = threadN;
  vector<thread> threads;
  vector<pair<int, int>> windows;

  double *sumR = new double[width * height];
  double *sumG = new double[width * height];
  double *sumB = new double[width * height];
  double *weightsumR = new double[width * height];
  double *weightsumG = new double[width * height];
  double *weightsumB = new double[width * height];
  double *weightmaxR = new double[width * height];
  double *weightmaxG = new double[width * height];
  double *weightmaxB = new double[width * height];

  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      sumR[y * width + x] = 0;
      sumG[y * width + x] = 0;
      sumB[y * width + x] = 0;
      weightsumR[y * width + x] = 0;
      weightsumG[y * width + x] = 0;
      weightsumB[y * width + x] = 0;
      weightmaxR[y * width + x] = 0;
      weightmaxG[y * width + x] = 0;
      weightmaxB[y * width + x] = 0;
    }
  }
  for (int wx = -(int)halfsearch; wx <= (int)halfsearch; wx++) {
    for (int wy = -(int)halfsearch; wy <= (int)halfsearch; wy++) {
      if (wx == 0 && wy == 0) continue;
      windows.push_back(make_pair(wx, wy));
    }
  }
  uint16_t segment = ceil((double)windows.size() / (double)threadN);
  uint16_t x_segment = ceil((double)width / (double)threadN);
  uint16_t w_start = 0;
  uint16_t w_end = segment;
  uint16_t x_start = 0;
  // Create threads
  for (int i = 0; i < threadN; i++) {
    NLmeansParam param;
    param.imageSource = imageSource;
    param.imageTarget = imageTarget;
    param.width = width;
    param.height = height;
    param.h = h;
    param.halfkernelsize = halfkernelsize;
    param.halfsearch = halfsearch;

    param.sumR = sumR;
    param.sumG = sumG;
    param.sumB = sumB;
    param.weightsumR = weightsumR;
    param.weightsumG = weightsumG;
    param.weightsumB = weightsumB;
    param.weightmaxR = weightmaxR;
    param.weightmaxG = weightmaxG;
    param.weightmaxB = weightmaxB;

    param.width_start = w_start;
    param.width_end = w_end;
    param.x_start = x_start;
    vector<pair<int, int>> w;
    for (int i = w_start; i < w_end; i++) {
      w.push_back(windows[i]);
    }
    // cout << "w:" << w_start << "," << w_end << endl;
    param.w = w;

    threads.emplace_back(NLmeanswithIntegralImageTask, param);

    w_start = w_end;
    w_end += segment;
    if (w_end > windows.size()) w_end = windows.size();
    if (w_start > windows.size()) w_start = windows.size();

    x_start += x_segment;
  }

  // Join threads
  for (auto &th : threads) {
    if (th.joinable()) {
      th.join();
    }
  }

  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      double curR = *(imageSource + 3 * (width * (y) + x) + 2);
      double curG = *(imageSource + 3 * (width * (y) + x) + 1);
      double curB = *(imageSource + 3 * (width * (y) + x) + 0);

      sumR[y * width + x] += (double)curR * weightmaxR[y * width + x];
      sumG[y * width + x] += (double)curG * weightmaxG[y * width + x];
      sumB[y * width + x] += (double)curB * weightmaxB[y * width + x];

      weightsumR[y * width + x] += weightmaxR[y * width + x];
      weightsumG[y * width + x] += weightmaxG[y * width + x];
      weightsumB[y * width + x] += weightmaxB[y * width + x];

      double newvalueR = (weightsumR[y * width + x] > 0)
                             ? (sumR[y * width + x] / weightsumR[y * width + x])
                             : 0;
      double newvalueG = (weightsumG[y * width + x] > 0)
                             ? (sumG[y * width + x] / weightsumG[y * width + x])
                             : 0;
      double newvalueB = (weightsumB[y * width + x] > 0)
                             ? (sumB[y * width + x] / weightsumB[y * width + x])
                             : 0;

      if (newvalueR > 255) newvalueR = 255;
      if (newvalueR < 0) newvalueR = 0;
      if (newvalueB > 255) newvalueB = 255;
      if (newvalueB < 0) newvalueB = 0;
      if (newvalueG > 255) newvalueG = 255;
      if (newvalueG < 0) newvalueG = 0;

      *(imageTarget + 3 * (width * (y) + x) + 2) = (uint8_t)newvalueR;
      *(imageTarget + 3 * (width * (y) + x) + 1) = (uint8_t)newvalueG;
      *(imageTarget + 3 * (width * (y) + x) + 0) = (uint8_t)newvalueB;
    }
  }
}
void testNLmeanswithintegralimagethread() {
  struct timespec t_start, t_end;
  double elapsedTime;
  SimpleImage *srcimg = new SimpleImage();

  bool res = srcimg->Load("InputImage01AfterGnoise.bmp");

  SimpleImage *dstimg = new SimpleImage(srcimg->width, srcimg->height);

  clock_gettime(CLOCK_REALTIME, &t_start);
  NLmeanswiththreadmain(srcimg->image, dstimg->image, srcimg->width,
                        srcimg->height, 5, 3, 5);

  clock_gettime(CLOCK_REALTIME, &t_end);
  elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
  elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
  printf("elapsedTime: %lf ms\n", elapsedTime);

  dstimg->Save("InputImage01AfterNLmeans.bmp");
}

int main() {
  testNLmeanswithintegralimagethread();

  return 0;
}