#include "../include/NonLocalMeans.hpp"

void NLmeans(const uint8_t *imageSource, uint8_t *imageTarget, uint16_t width,
             uint16_t height, double h, uint8_t halfkernelsize,
             uint8_t halfsearch) {
  double h2 = (h * 2.0 + 1) * (h * 2.0 + 1) * 3;
  // double sigma2 = 5 * 5;

  for (int x = 0; x < width; ++x) {
    // cout << "X:" << x << endl;
    for (int y = 0; y < height; ++y) {
      int w_xmin = max(0, x - halfsearch);
      int w_xmax = min((int)width, x + halfsearch + 1);
      int w_ymin = max(0, y - halfsearch);
      int w_ymax = min((int)height, y + halfsearch + 1);

      double sumwR = 0, sumwG = 0, sumwB = 0;
      double pR = 0, pG = 0, pB = 0;
      double maxweightR = 0, maxweightG = 0, maxweightB = 0;

      for (int wx = w_xmin; wx <= w_xmax; wx++) {
        for (int wy = w_ymin; wy <= w_ymax; wy++) {
          double distR = 0, distG = 0, distB = 0;

          if (wx == x && wy == y) continue;

          for (int kx = -halfkernelsize; kx <= halfkernelsize; kx++) {
            for (int ky = -halfkernelsize; ky <= halfkernelsize; ky++) {
              int nx = x + kx, ny = y + ky;
              int nwx = wx + kx, nwy = wy + ky;

              if (nx < 0 || nx >= width || ny < 0 || ny >= height || nwx < 0 ||
                  nwx >= width || nwy < 0 || nwy >= height)
                continue;

              int curIndex = 3 * (width * ny + nx);
              int targetIndex = 3 * (width * nwy + nwx);

              int curR = *(imageSource + curIndex + 2);
              int curG = *(imageSource + curIndex + 1);
              int curB = *(imageSource + curIndex + 0);

              int targetR = *(imageSource + targetIndex + 2);
              int targetG = *(imageSource + targetIndex + 1);
              int targetB = *(imageSource + targetIndex + 0);

              distR += (curR - targetR) * (curR - targetR);
              distG += (curG - targetG) * (curG - targetG);
              distB += (curB - targetB) * (curB - targetB);
            }
          }

          distR /= (2 * halfkernelsize + 1) * (2 * halfkernelsize + 1);
          distG /= (2 * halfkernelsize + 1) * (2 * halfkernelsize + 1);
          distB /= (2 * halfkernelsize + 1) * (2 * halfkernelsize + 1);

#if 0
          distR = max(distR - 2 * sigma2, 0.0);
          distG = max(distG - 2 * sigma2, 0.0);
          distB = max(distB - 2 * sigma2, 0.0);
#endif
          double weightR = exp(-distR / h2);
          double weightG = exp(-distG / h2);
          double weightB = exp(-distB / h2);

          int targetIndex = 3 * (width * wy + wx);
          pR += weightR * (*(imageSource + targetIndex + 2));
          pG += weightG * (*(imageSource + targetIndex + 1));
          pB += weightB * (*(imageSource + targetIndex + 0));

          sumwR += weightR;
          sumwG += weightG;
          sumwB += weightB;

          if (weightR > maxweightR) maxweightR = weightR;
          if (weightG > maxweightG) maxweightG = weightG;
          if (weightB > maxweightB) maxweightB = weightB;
        }
      }
      double curR = *(imageSource + 3 * (width * (y) + x) + 2);
      double curG = *(imageSource + 3 * (width * (y) + x) + 1);
      double curB = *(imageSource + 3 * (width * (y) + x) + 0);

      pR += curR * maxweightR;
      pG += curG * maxweightG;
      pB += curB * maxweightB;

      sumwR += maxweightR;
      sumwG += maxweightG;
      sumwB += maxweightB;

      double newvalueR = (sumwR > 0) ? (pR / sumwR) : 0;
      double newvalueG = (sumwG > 0) ? (pG / sumwG) : 0;
      double newvalueB = (sumwB > 0) ? (pB / sumwB) : 0;

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
void NLmeanswithIntegralImage(const uint8_t *imageSource, uint8_t *imageTarget,
                              uint16_t width, uint16_t height, double h,
                              uint8_t halfkernelsize, uint8_t halfsearch) {
  double h2 = (h * 2.0 + 1) * (h * 2.0 + 1) * 3;
  // double sigma2 = 5 * 5;

  double *sumR = new double[(width) * (height)];
  double *sumG = new double[(width) * (height)];
  double *sumB = new double[(width) * (height)];
  double *weightsumR = new double[(width) * (height)];
  double *weightsumG = new double[(width) * (height)];
  double *weightsumB = new double[(width) * (height)];
  double *weightmaxR = new double[(width) * (height)];
  double *weightmaxG = new double[(width) * (height)];
  double *weightmaxB = new double[(width) * (height)];
  double *integralR = new double[(width) * (height)];
  double *integralG = new double[(width) * (height)];
  double *integralB = new double[(width) * (height)];
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

      GenDifferIntegralImage(imageSource, integralR, width, height, wx, wy, 2);
      GenDifferIntegralImage(imageSource, integralG, width, height, wx, wy, 1);
      GenDifferIntegralImage(imageSource, integralB, width, height, wx, wy, 0);

      for (int x = 0; x < width; ++x) {
        if (x + wx < 0 || x + wx >= width) continue;
        for (int y = 0; y < height; ++y) {
          if (y + wy < 0 || y + wy >= height) continue;

          double distR = 0, distG = 0, distB = 0;

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

#if 1
          distR /= (2.0 * halfkernelsize + 1.0) * (2.0 * halfkernelsize + 1.0);
          distG /= (2.0 * halfkernelsize + 1.0) * (2.0 * halfkernelsize + 1.0);
          distB /= (2.0 * halfkernelsize + 1.0) * (2.0 * halfkernelsize + 1.0);
#endif

          double weightR = exp(-distR / h2);
          double weightG = exp(-distG / h2);
          double weightB = exp(-distB / h2);

          int targetIndex = 3 * (width * (wy + y) + (wx + x));

          sumR[y * width + x] += weightR * (*(imageSource + targetIndex + 2));
          sumG[y * width + x] += weightG * (*(imageSource + targetIndex + 1));
          sumB[y * width + x] += weightB * (*(imageSource + targetIndex + 0));

          weightsumR[y * width + x] += weightR;
          weightsumG[y * width + x] += weightG;
          weightsumB[y * width + x] += weightB;

          weightmaxR[y * width + x] = max(weightmaxR[y * width + x], weightR);
          weightmaxG[y * width + x] = max(weightmaxG[y * width + x], weightG);
          weightmaxB[y * width + x] = max(weightmaxB[y * width + x], weightB);
        }
      }
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
  delete[] integralR;
  delete[] integralG;
  delete[] integralB;

  delete[] sumR;
  delete[] sumG;
  delete[] sumB;

  delete[] weightsumR;
  delete[] weightsumG;
  delete[] weightsumB;

  delete[] weightmaxR;
  delete[] weightmaxG;
  delete[] weightmaxB;
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
};
void NLmeanstask(const struct NLmeansParam param) {
  uint8_t *imageSource = param.imageSource;
  uint8_t *imageTarget = param.imageTarget;
  uint16_t width = param.width;
  uint16_t height = param.height;
  double h = param.h;
  uint8_t halfkernelsize = param.halfkernelsize;
  uint8_t halfsearch = param.halfsearch;
  uint16_t width_start = param.width_start;
  uint16_t width_end = param.width_end;

  double h2 = (h * 2.0 + 1) * (h * 2.0 + 1) * 3;
  // double sigma2 = 5 * 5;

  for (int x = width_start; x < width_end; ++x) {
    // cout << "X:" << x << endl;
    for (int y = 0; y < height; ++y) {
      int w_xmin = max(0, x - halfsearch);
      int w_xmax = min((int)width, x + halfsearch + 1);
      int w_ymin = max(0, y - halfsearch);
      int w_ymax = min((int)height, y + halfsearch + 1);

      double sumwR = 0, sumwG = 0, sumwB = 0;
      double pR = 0, pG = 0, pB = 0;
      double maxweightR = 0, maxweightG = 0, maxweightB = 0;

      for (int wx = w_xmin; wx <= w_xmax; wx++) {
        for (int wy = w_ymin; wy <= w_ymax; wy++) {
          double distR = 0, distG = 0, distB = 0;

          if (wx == x && wy == y) continue;

          for (int kx = -halfkernelsize; kx <= halfkernelsize; kx++) {
            for (int ky = -halfkernelsize; ky <= halfkernelsize; ky++) {
              int nx = x + kx, ny = y + ky;
              int nwx = wx + kx, nwy = wy + ky;

              if (nx < 0 || nx >= width || ny < 0 || ny >= height || nwx < 0 ||
                  nwx >= width || nwy < 0 || nwy >= height)
                continue;

              int curIndex = 3 * (width * ny + nx);
              int targetIndex = 3 * (width * nwy + nwx);

              int curR = *(imageSource + curIndex + 2);
              int curG = *(imageSource + curIndex + 1);
              int curB = *(imageSource + curIndex + 0);

              int targetR = *(imageSource + targetIndex + 2);
              int targetG = *(imageSource + targetIndex + 1);
              int targetB = *(imageSource + targetIndex + 0);

              distR += (curR - targetR) * (curR - targetR);
              distG += (curG - targetG) * (curG - targetG);
              distB += (curB - targetB) * (curB - targetB);
            }
          }

          distR /= (2 * halfkernelsize + 1) * (2 * halfkernelsize + 1);
          distG /= (2 * halfkernelsize + 1) * (2 * halfkernelsize + 1);
          distB /= (2 * halfkernelsize + 1) * (2 * halfkernelsize + 1);

          double weightR = exp(-distR / h2);
          double weightG = exp(-distG / h2);
          double weightB = exp(-distB / h2);

          int targetIndex = 3 * (width * wy + wx);
          pR += weightR * (*(imageSource + targetIndex + 2));
          pG += weightG * (*(imageSource + targetIndex + 1));
          pB += weightB * (*(imageSource + targetIndex + 0));

          sumwR += weightR;
          sumwG += weightG;
          sumwB += weightB;

          if (weightR > maxweightR) maxweightR = weightR;
          if (weightG > maxweightG) maxweightG = weightG;
          if (weightB > maxweightB) maxweightB = weightB;
        }
      }
      double curR = *(imageSource + 3 * (width * (y) + x) + 2);
      double curG = *(imageSource + 3 * (width * (y) + x) + 1);
      double curB = *(imageSource + 3 * (width * (y) + x) + 0);

      pR += curR * maxweightR;
      pG += curG * maxweightG;
      pB += curB * maxweightB;

      sumwR += maxweightR;
      sumwG += maxweightG;
      sumwB += maxweightB;

      double newvalueR = (sumwR > 0) ? (pR / sumwR) : 0;
      double newvalueG = (sumwG > 0) ? (pG / sumwG) : 0;
      double newvalueB = (sumwB > 0) ? (pB / sumwB) : 0;

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
void NLmeanswiththreadmain(uint8_t *imageSource, uint8_t *imageTarget,
                           uint16_t width, uint16_t height, double h,
                           uint8_t halfkernelsize, uint8_t halfsearch) {
  int threadN = 8;
  std::vector<std::thread> threads;
  uint16_t segment = width / threadN;
  uint16_t width_start = 0;
  uint16_t width_end = segment;

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

    param.width_start = width_start;
    param.width_end = width_end;
    threads.emplace_back(NLmeanstask, param);

    width_start = width_end;
    width_end += segment;
    if (width_end > width) width_end = width;
    if (width_start > width) width_start = width;
  }

  // Join threads
  for (auto &th : threads) {
    if (th.joinable()) {
      th.join();
    }
  }
}

struct NLmeansmixParam {
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

void NLmeanswithIntegralImageTask(const struct NLmeansmixParam param) {
  // unique_lock<std::mutex> lock(u_mutex);
  uint8_t *imageSource = param.imageSource;
  // uint8_t *imageTarget = param.imageTarget;
  uint16_t width = param.width;
  uint16_t height = param.height;
  double h = param.h;
  uint8_t halfkernelsize = param.halfkernelsize;
  // uint8_t halfsearch = param.halfsearch;
  // uint16_t width_start = param.width_start;
  // uint16_t width_end = param.width_end;
  uint16_t x_start = param.x_start;
  double h2 = (h * 2.0 + 1) * (h * 2.0 + 1) * 3;
  // double sigma2 = 5 * 5;

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

#if 1
        distR /= (2.0 * halfkernelsize + 1.0) * (2.0 * halfkernelsize + 1.0);
        distG /= (2.0 * halfkernelsize + 1.0) * (2.0 * halfkernelsize + 1.0);
        distB /= (2.0 * halfkernelsize + 1.0) * (2.0 * halfkernelsize + 1.0);
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
int totalThreads = 0;
void NLmeanswithmixmain(uint8_t *imageSource, uint8_t *imageTarget,
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
    NLmeansmixParam param;
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