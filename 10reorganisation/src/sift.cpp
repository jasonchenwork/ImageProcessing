#include "../include/sift.hpp"

#include "../include/SimpleImage.hpp"
#include "../include/SimpleMat.hpp"
#include "../include/Utility.hpp"
int SIFT_DESCR_HIST_BINS = 8;
float SIFT_CURV_THR = 10.0f;
float SIFT_CONTR_THR = 8.0f;
float SIFT_SIGMA = 1.6f;
int SIFT_INTVLS = 3;
// static float SIFT_INIT_SIGMA = 0.5f;
int SIFT_ORI_HIST_BINS = 36;
// static float SIFT_GAUSSIAN_FILTER_RADIUS = 3.0f;
float SIFT_ORI_SIG_FCTR = 1.5f;                 // sigma
float SIFT_ORI_RADIUS = 3 * SIFT_ORI_SIG_FCTR;  //
float SIFT_ORI_PEAK_RATIO = 0.8f;

float SIFT_DESCR_MAG_THR = 0.2f;

int SIFT_DESCR_WIDTH = 4;

float SIFT_DESCR_SCL_FCTR = 3.f;

float SIFT_INT_DESCR_FCTR = 512.f;

// |DR_nearest|/|DR_2nd_nearest|<SIFT_MATCH_NNDR_THR
float SIFT_MATCH_NNDR_THR = 0.2f;  // 0.65f

int SIFT_GassianMaxWindowsSize = 31;
/*
  this code is based on  https :  // github.com/leofansq/SIFT
 ref : https :  // github.com/leofansq/SIFT
 */
float get_pixel_f(float *imageData, int w, int h, int r, int c) {
  float val;
  if (c >= 0 && c < w && r >= 0 && r < h) {
    val = imageData[r * w + c];
  } else if (c < 0) {
    val = imageData[r * w];
  } else if (c >= w) {
    val = imageData[r * w + w - 1];
  } else if (r < 0) {
    val = imageData[c];
  } else if (r >= h) {
    val = imageData[(h - 1) * w + c];
  } else {
    val = 0.0f;
  }
  return val;
}

inline float fast_atan2_f(float y, float x) {
  float angle, r;
  float const c3 = 0.1821F;
  float const c1 = 0.9675F;
  float abs_y = fabsf(y) + EPSILON_F;

  if (x >= 0) {
    r = (x - abs_y) / (x + abs_y);
    angle = PI_4;
  } else {
    r = (x + abs_y) / (abs_y - x);
    angle = PI_3_4;
  }
  angle += (c3 * r * r - c1) * r;

  return (y < 0) ? _2PI - angle : angle;
}
inline float fast_resqrt_f(float x) {
  // 32-bit version
  union {
    float x;
    int i;
  } u;

  float xhalf = (float)0.5 * x;

  // convert floating point value in RAW integer
  u.x = x;

  // gives initial guess y0
  u.i = 0x5f3759df - (u.i >> 1);

  // two Newton steps
  u.x = u.x * ((float)1.5 - xhalf * u.x * u.x);
  u.x = u.x * ((float)1.5 - xhalf * u.x * u.x);
  return u.x;
}

inline float fast_sqrt_f(float x) {
  return (x < 1e-8) ? 0 : x * fast_resqrt_f(x);
}
string int2str(int num) { return to_string(num); }
bool IsLocalMin(float *data, float val, int idx, int w) {
  return val < data[idx + 1] && val < data[idx - 1] &&
         val < data[idx - w - 1] && val < data[idx - w] &&
         val < data[idx - w + 1] && val < data[idx + w - 1] &&
         val < data[idx + w] && val < data[idx + w + 1];
}
bool IsLocalMax(float *data, float val, int idx, int w) {
  return val > data[idx + 1] && val > data[idx - 1] &&
         val > data[idx - w - 1] && val > data[idx - w] &&
         val > data[idx - w + 1] && val > data[idx + w - 1] &&
         val > data[idx + w] && val > data[idx + w + 1];
}
bool refine_local_extrema(vector<vector<SimpleMat<float>>> &dogPyr,
                          int nDogLayers, SiftKeypoint &kpt) {
  int nGpyrLayers = nDogLayers + 1;

  int w = 0, h = 0;
  // int layer_idx = 0;
  int octave = kpt.octave;
  int layer = kpt.layer;
  int r = (int)kpt.yi;
  int c = (int)kpt.xi;

  float *currData = nullptr;
  float *lowData = nullptr;
  float *highData = nullptr;

  int xs_i = 0, xr_i = 0, xc_i = 0;
  float tmp_r = 0.0f, tmp_c = 0.0f, tmp_layer = 0.0f;
  float xr = 0.0f, xc = 0.0f, xs = 0.0f;
  float x_hat[3] = {xc, xr, xs};
  float dx = 0.0f, dy = 0.0f, ds = 0.0f;
  float dxx = 0.0f, dyy = 0.0f, dss = 0.0f, dxs = 0.0f, dys = 0.0f, dxy = 0.0f;

  tmp_r = (float)r;
  tmp_c = (float)c;
  tmp_layer = (float)layer;
  int i = 0;
  for (; i < 5; i++) {  //

    c += xc_i;
    r += xr_i;
    w = dogPyr[octave][layer].width;
    h = dogPyr[octave][layer].height;
    currData = dogPyr[octave][layer].data;
    lowData = dogPyr[octave][layer - 1].data;
    highData = dogPyr[octave][layer + 1].data;

    dx = (get_pixel_f(currData, w, h, r, c + 1) -
          get_pixel_f(currData, w, h, r, c - 1)) *
         0.5f;
    dy = (get_pixel_f(currData, w, h, r + 1, c) -
          get_pixel_f(currData, w, h, r - 1, c)) *
         0.5f;
    ds =
        (get_pixel_f(highData, w, h, r, c) - get_pixel_f(lowData, w, h, r, c)) *
        0.5f;
    float dD[3] = {-dx, -dy, -ds};

    float v2 = 2.0f * get_pixel_f(currData, w, h, r, c);
    dxx = (get_pixel_f(currData, w, h, r, c + 1) +
           get_pixel_f(currData, w, h, r, c - 1) - v2);
    dyy = (get_pixel_f(currData, w, h, r + 1, c) +
           get_pixel_f(currData, w, h, r - 1, c) - v2);
    dss = (get_pixel_f(highData, w, h, r, c) +
           get_pixel_f(lowData, w, h, r, c) - v2);
    dxy = (get_pixel_f(currData, w, h, r + 1, c + 1) -
           get_pixel_f(currData, w, h, r + 1, c - 1) -
           get_pixel_f(currData, w, h, r - 1, c + 1) +
           get_pixel_f(currData, w, h, r - 1, c - 1)) *
          0.25f;
    dxs = (get_pixel_f(highData, w, h, r, c + 1) -
           get_pixel_f(highData, w, h, r, c - 1) -
           get_pixel_f(lowData, w, h, r, c + 1) +
           get_pixel_f(lowData, w, h, r, c - 1)) *
          0.25f;
    dys = (get_pixel_f(highData, w, h, r + 1, c) -
           get_pixel_f(highData, w, h, r - 1, c) -
           get_pixel_f(lowData, w, h, r + 1, c) +
           get_pixel_f(lowData, w, h, r - 1, c)) *
          0.25f;

    float H[3][3] = {{dxx, dxy, dxs}, {dxy, dyy, dys}, {dxs, dys, dss}};
    float Hinvert[3][3];
    float det;
    float tmp;
    DETERMINANT_3X3(det, H);
    if (fabsf(det) < (numeric_limits<float>::min)()) break;
    tmp = 1.0f / (det);
    SCALE_ADJOINT_3X3(Hinvert, tmp, H);
    MAT_DOT_VEC_3X3(x_hat, Hinvert, dD);

    xs = x_hat[2];
    xr = x_hat[1];
    xc = x_hat[0];
    tmp_r = r + xr;
    tmp_c = c + xc;
    tmp_layer = layer + xs;
    xc_i =
        ((xc >= 0.6f && c < w - 2) ? 1 : 0) + ((xc <= -0.6f && c > 1) ? -1 : 0);

    xr_i =
        ((xr >= 0.6f && r < h - 2) ? 1 : 0) + ((xr <= -0.6f && r > 1) ? -1 : 0);

    if (xc_i == 0 && xr_i == 0 && xs_i == 0) break;
  }
  if (i >= 5) return false;
  if (fabsf(xc) >= 1.5 || fabsf(xr) >= 1.5 || fabsf(xs) >= 1.5) return false;
  if (tmp_layer < 0 || tmp_layer > (nGpyrLayers - 1) || tmp_r < 0 ||
      tmp_r > h - 1 || tmp_c < 0 || tmp_c > w - 1)
    return false;

  {
    float value = get_pixel_f(currData, w, h, r, c) +
                  0.5f * (dx * xc + dy * xr + ds * xs);
    if (fabsf(value) < SIFT_CONTR_THR) return false;

    float trH = dxx + dyy;
    float detH = dxx * dyy - dxy * dxy;
    float response =
        (SIFT_CURV_THR + 1) * (SIFT_CURV_THR + 1) / (SIFT_CURV_THR);

    if (detH <= 0 || (trH * trH / detH) >= response) return false;
  }
  kpt.xi = tmp_c;
  kpt.yi = tmp_r;
  kpt.layer_scale = SIFT_SIGMA * powf(2.0f, tmp_layer / SIFT_INTVLS);
  float norm = powf(2.0f, (float)(octave));
  // normalize
  kpt.x = tmp_c * norm;
  kpt.y = tmp_r * norm;
  kpt.rlayer = tmp_layer;
  kpt.layer = layer;

  // Scale = sigma0 * 2^octave * 2^(layer/S);
  kpt.scale = kpt.layer_scale * norm;

  return true;
}
float compute_orientation_hist_with_gradient(const SimpleMat<float> &grdImage,
                                             const SimpleMat<float> &rotImage,
                                             SiftKeypoint &kpt, float *&hist)
/*
    Parameters:
        grdImage:
        rotImage:
        kpt:
        hist:
*/
{
  int nBins = SIFT_ORI_HIST_BINS;

  float kptr = kpt.yi;
  float kptc = kpt.xi;
  float kpt_scale = kpt.layer_scale;

  int kptr_i = (int)(kptr + 0.5f);
  int kptc_i = (int)(kptc + 0.5f);
  float d_kptr = kptr - kptr_i;
  float d_kptc = kptc - kptc_i;

  // 邻域参数
  float sigma = SIFT_ORI_SIG_FCTR * kpt_scale;
  int win_radius = (int)(SIFT_ORI_RADIUS * kpt_scale);
  float exp_factor = -1.0f / (2.0f * sigma * sigma);

  float *grdData = grdImage.data;
  float *rotData = rotImage.data;
  int w = grdImage.width;
  int h = grdImage.height;

  int r, c;
  float magni, angle, weight;
  int bin;
  float fbin;

  float *tmpHist = new float[nBins];
  memset(tmpHist, 0, nBins * sizeof(float));

  for (int i = -win_radius; i <= win_radius; i++)  // rows
  {
    r = kptr_i + i;
    if (r <= 0 || r >= h - 1)  // Cannot calculate dy
      continue;
    for (int j = -win_radius; j <= win_radius; j++)  // columns
    {
      c = kptc_i + j;
      if (c <= 0 || c >= w - 1) continue;

      magni = grdData[r * w + c];
      angle = rotData[r * w + c];

      fbin = angle * nBins / _2PI;
      weight =
          expf(((i - d_kptr) * (i - d_kptr) + (j - d_kptc) * (j - d_kptc)) *
               exp_factor);

      // interpolation
      bin = (int)(fbin - 0.5f);
      float d_fbin = fbin - 0.5f - bin;

      float mw = weight * magni;
      float dmw = d_fbin * mw;
      tmpHist[(bin + nBins) % nBins] += mw - dmw;
      tmpHist[(bin + 1) % nBins] += dmw;
    }
  }

#define TMPHIST(idx) \
  (idx < 0 ? tmpHist[0] : (idx >= nBins ? tmpHist[nBins - 1] : tmpHist[idx]))

  // Smooth the histogram. Algorithm comes from OpenCV.
  hist[0] = (tmpHist[0] + tmpHist[2]) * 1.0f / 16.0f +
            (tmpHist[0] + tmpHist[1]) * 4.0f / 16.0f +
            tmpHist[0] * 6.0f / 16.0f;
  hist[1] = (tmpHist[0] + tmpHist[3]) * 1.0f / 16.0f +
            (tmpHist[0] + tmpHist[2]) * 4.0f / 16.0f +
            tmpHist[1] * 6.0f / 16.0f;
  hist[nBins - 2] = (tmpHist[nBins - 4] + tmpHist[nBins - 1]) * 1.0f / 16.0f +
                    (tmpHist[nBins - 3] + tmpHist[nBins - 1]) * 4.0f / 16.0f +
                    tmpHist[nBins - 2] * 6.0f / 16.0f;
  hist[nBins - 1] = (tmpHist[nBins - 3] + tmpHist[nBins - 1]) * 1.0f / 16.0f +
                    (tmpHist[nBins - 2] + tmpHist[nBins - 1]) * 4.0f / 16.0f +
                    tmpHist[nBins - 1] * 6.0f / 16.0f;

  for (int i = 2; i < nBins - 2; i++) {
    hist[i] = (tmpHist[i - 2] + tmpHist[i + 2]) * 1.0f / 16.0f +
              (tmpHist[i - 1] + tmpHist[i + 1]) * 4.0f / 16.0f +
              tmpHist[i] * 6.0f / 16.0f;
  }

  // find peak
  float maxitem = hist[0];
  int max_i = 0;
  for (int i = 0; i < nBins; i++) {
    if (maxitem < hist[i]) {
      maxitem = hist[i];
      max_i = i;
    }
  }

  kpt.ori = max_i * _2PI / nBins;

  delete[] tmpHist;
  tmpHist = nullptr;
  return maxitem;
}
int extract_descriptor(vector<vector<SimpleMat<float>>> &grdPyr,
                       vector<vector<SimpleMat<float>>> &rotPyr,
                       list<SiftKeypoint> &kpt_list)
/*
    Parameters:
        grdPyr:
        rotPyr:
        nOctaves:
        nGpyrLayers:
        kpt_list:
*/
{
  // 4*4 blocks
  int nSubregion = SIFT_DESCR_WIDTH;
  int nHalfSubregion = nSubregion >> 1;

  // 8 dirs
  int nBinsPerSubregion = SIFT_DESCR_HIST_BINS;
  float nBinsPerSubregionPerDegree = (float)nBinsPerSubregion / _2PI;

  // (rbin, cbin, obin):(row of hist bin, column of hist bin, orientation bin)

  int nBins = nSubregion * nSubregion * nBinsPerSubregion;  // 4*4*8=128维向量
  int nHistBins = (nSubregion + 2) * (nSubregion + 2) * (nBinsPerSubregion + 2);
  int nSliceStep = (nSubregion + 2) * (nBinsPerSubregion + 2);
  int nRowStep = (nBinsPerSubregion + 2);
  float *histBin = new float[nHistBins];

  for (list<SiftKeypoint>::iterator kpt = kpt_list.begin();
       kpt != kpt_list.end(); kpt++) {
    int octave = kpt->octave;
    int layer = kpt->layer;

    float kpt_ori = kpt->ori;
    float kptr = kpt->yi;
    float kptc = kpt->xi;
    float kpt_scale = kpt->layer_scale;

    int kptr_i = (int)(kptr + 0.5f);
    int kptc_i = (int)(kptc + 0.5f);
    float d_kptr = kptr_i - kptr;
    float d_kptc = kptc_i - kptc;

    // int layer_index = octave * nGpyrLayers + layer;
    int w = grdPyr[octave][layer].width;
    int h = grdPyr[octave][layer].height;
    float *grdData = grdPyr[octave][layer].data;
    float *rotData = rotPyr[octave][layer].data;

    float subregion_width = SIFT_DESCR_SCL_FCTR * kpt_scale;

    int win_size =
        (int)(SQRT2 * subregion_width * (nSubregion + 1) * 0.5f + 0.5f);

    // normalize cos() 和 sin()
    float sin_t = sinf(kpt_ori) / (float)subregion_width;
    float cos_t = cosf(kpt_ori) / (float)subregion_width;

    // Re-init histBin
    memset(histBin, 0, nHistBins * sizeof(float));

    // calculate histgram
    float rr, cc;
    float mag, angle, gaussian_weight;

    float rrotate, crotate;
    float rbin, cbin, obin;
    float d_rbin, d_cbin, d_obin;

    // bound
    int r, c;
    int left = max(-win_size, 1 - kptc_i);
    int right = min(win_size, w - 2 - kptc_i);
    int top = max(-win_size, 1 - kptr_i);
    int bottom = min(win_size, h - 2 - kptr_i);

    for (int i = top; i <= bottom; i++) {
      for (int j = left; j <= right; j++) {
        rr = i + d_kptr;
        cc = j + d_kptc;

        rrotate = (cos_t * cc + sin_t * rr);
        crotate = (-sin_t * cc + cos_t * rr);

        rbin = rrotate + nHalfSubregion - 0.5f;
        cbin = crotate + nHalfSubregion - 0.5f;

        if (rbin <= -1 || rbin >= nSubregion || cbin <= -1 ||
            cbin >= nSubregion)
          continue;

        r = kptr_i + i;
        c = kptc_i + j;
        mag = grdData[r * w + c];
        angle = rotData[r * w + c] - kpt_ori;
        float angle1 = (angle < 0) ? (_2PI + angle) : angle;  //  [0, 2PI)
        obin = angle1 * nBinsPerSubregionPerDegree;

        int x0, y0, z0;
        int x1, y1;  //, z1;
        y0 = (int)floor(rbin);
        x0 = (int)floor(cbin);
        z0 = (int)floor(obin);
        d_rbin = rbin - y0;
        d_cbin = cbin - x0;
        d_obin = obin - z0;
        x1 = x0 + 1;
        y1 = y0 + 1;
        // z1 = z0 + 1;

        // guassian weight
        float exp_scale = -2.0f / (nSubregion * nSubregion);
        gaussian_weight =
            expf((rrotate * rrotate + crotate * crotate) * exp_scale);
        float gm = mag * gaussian_weight;

        // 8 dir
        //  Tri-linear interpolation
        float vr1, vr0;
        float vrc11, vrc10, vrc01, vrc00;
        float vrco110, vrco111, vrco100, vrco101, vrco010, vrco011, vrco000,
            vrco001;

        vr1 = gm * d_rbin;
        vr0 = gm - vr1;
        vrc11 = vr1 * d_cbin;
        vrc10 = vr1 - vrc11;
        vrc01 = vr0 * d_cbin;
        vrc00 = vr0 - vrc01;
        vrco111 = vrc11 * d_obin;
        vrco110 = vrc11 - vrco111;
        vrco101 = vrc10 * d_obin;
        vrco100 = vrc10 - vrco101;
        vrco011 = vrc01 * d_obin;
        vrco010 = vrc01 - vrco011;
        vrco001 = vrc00 * d_obin;
        vrco000 = vrc00 - vrco001;

        // int idx =  y0  * nSliceStep + x0  * nRowStep + z0;
        // All coords are offseted by 1. so x=[1, 4], y=[1, 4];
        // data for -1 coord is stored at position 0;
        // data for 8 coord is stored at position 9.
        // z doesn't need to move.
        int idx = y1 * nSliceStep + x1 * nRowStep + z0;
        histBin[idx] += vrco000;

        idx++;
        histBin[idx] += vrco001;

        idx += nRowStep - 1;
        histBin[idx] += vrco010;

        idx++;
        histBin[idx] += vrco011;

        idx += nSliceStep - nRowStep - 1;
        histBin[idx] += vrco100;

        idx++;
        histBin[idx] += vrco101;

        idx += nRowStep - 1;
        histBin[idx] += vrco110;

        idx++;
        histBin[idx] += vrco111;
      }
    }

    // Discard all the edges for row and column.
    // Only retrive edges for orientation bins.
    float *dstBins = new float[nBins];
    for (int i = 1; i <= nSubregion; i++)  // slice
    {
      for (int j = 1; j <= nSubregion; j++)  // row
      {
        int idx = i * nSliceStep + j * nRowStep;
        // comments: how this line works.
        // Suppose you want to write w=width, y=1, due to circular
        // buffer, we should write it to w=0, y=1; since we use a
        // circular buffer, it is written into w=width, y=1. Now, we
        // fectch the data back.
        histBin[idx] = histBin[idx + nBinsPerSubregion];

        // comments: how this line works.
        // Suppose you want to write x=-1 y=1, due to circular, it
        // should be at y=1, x=width-1; since we use circular buffer,
        // the value goes to y=0, x=width, now, we need to get it back.
        if (idx != 0) histBin[idx + nBinsPerSubregion + 1] = histBin[idx - 1];

        int idx1 = ((i - 1) * nSubregion + j - 1) * nBinsPerSubregion;
        for (int k = 0; k < nBinsPerSubregion; k++) {
          dstBins[idx1 + k] = histBin[idx + k];
        }
      }
    }

    float sum_square = 0.0f;
    for (int i = 0; i < nBins; i++) sum_square += dstBins[i] * dstBins[i];

    float thr = fast_sqrt_f(sum_square) * SIFT_DESCR_MAG_THR;

    float tmp = 0.0;
    sum_square = 0.0;

    for (int i = 0; i < nBins; i++) {
      tmp = fmin(thr, dstBins[i]);
      dstBins[i] = tmp;
      sum_square += tmp * tmp;
    }

    float norm_factor = SIFT_INT_DESCR_FCTR / fast_sqrt_f(sum_square);
    for (int i = 0; i < nBins; i++) dstBins[i] = dstBins[i] * norm_factor;

    memcpy(kpt->descriptors, dstBins, nBins * sizeof(float));

    if (dstBins) {
      delete[] dstBins;
      dstBins = nullptr;
    }
  }
  if (histBin) {
    delete[] histBin;
    histBin = nullptr;
  }

  return 0;
}
void sift(const uint8_t *imageSource, uint16_t width, uint16_t height,
          list<SiftKeypoint> &kp) {
  int nLayers = 3;
  int nDogLayers = nLayers + 2;
  int nGpyrLayers = nLayers + 3;
  int nOctaves = (int)log2((float)fmin(width, height)) - 3;

  uint8_t *grayimage = new uint8_t[width * height];
  // vector<float> grayimagef(width * height, 0);
  float *grayimagef = new float[width * height];
  colorimage2grayimage(imageSource, grayimage, width, height);
  uint8Tofloat(grayimage, grayimagef, width, height);
  SimpleMat<float> mat((uint32_t)width, (uint32_t)height, grayimagef);

  vector<SimpleMat<float>> octaves(nOctaves);
  double std = 1.6;  // sqrt(1.6 * 1.6 - 0.5 * 0.5);
  int windowsize = 7;

  vector<vector<SimpleMat<float>>> guasspyr(
      nOctaves, vector<SimpleMat<float>>(nGpyrLayers));
  vector<vector<SimpleMat<float>>> Dogpyr(nOctaves,
                                          vector<SimpleMat<float>>(nDogLayers));

  vector<vector<SimpleMat<float>>> grdPyr(nOctaves,
                                          vector<SimpleMat<float>>(nDogLayers));
  vector<vector<SimpleMat<float>>> rotPyr(nOctaves,
                                          vector<SimpleMat<float>>(nDogLayers));
  // build_octaves
  for (int i = 0; i < nOctaves; i++) {
    if (i == 0) {
      octaves[i] = mat;
    } else {
      octaves[i] = octaves[i - 1].downsample2x();
    }
  }

  double *gaussianfilter =
      new double[SIFT_GassianMaxWindowsSize * SIFT_GassianMaxWindowsSize];
  // build_gaussian_pyramid
  for (int i = 0; i < nOctaves; i++) {
#if 0
    float sigma, sigma_pre;
    float sig;
    float sigma0 = SIFT_SIGMA;
    float k = powf(2.0f, 1.0f / nLayers);
#endif

    for (int j = 0; j < nGpyrLayers; j++) {
      // double k;
      double cur_std;
      uint16_t cur_w = octaves[i].width;
      uint16_t cur_h = octaves[i].height;
      if (i == 0 && j == 0) {
        cur_std = std;
      } else {
        cur_std = exp2(i + ((double)j / nLayers)) * std;
      }
#if 0

      if (j == 0) {
        sigma_pre = SIFT_INIT_SIGMA;
        sig = sqrtf(sigma0 * sigma0 - sigma_pre * sigma_pre);

      } else {
        sigma_pre = powf(k, (float)(i - 1)) * sigma0;
        sigma = sigma_pre * k;
        sig = sqrtf(sigma * sigma - sigma_pre * sigma_pre);
      }
      float factor = SIFT_GAUSSIAN_FILTER_RADIUS;
      int gR = (sig * factor > 1.0f) ? (int)ceilf(sig * factor) : 1;
      int gW = gR * 2 + 1;
      windowsize = gW;
      cout << "j: " << j << " gW:" << gW << endl;
      //  cur_std = sig;
#else

      if (cur_std < 0) {
        windowsize = 7;
      } else {
        windowsize = 6 * ceilf(cur_std) + 1;
      }
      if (windowsize >= SIFT_GassianMaxWindowsSize)
        windowsize = SIFT_GassianMaxWindowsSize;
#endif

      GaussianFilter(gaussianfilter, (uint16_t)windowsize, cur_std);
      guasspyr[i][j].setMat(cur_w, cur_h);
      conv2D(octaves[i].data, guasspyr[i][j].data, cur_w, cur_h, gaussianfilter,
             (uint16_t)windowsize, conv2D_gray);
#if 0
      SimpleImage *dstimg = new SimpleImage(cur_w, cur_h);
      uint8_t *outimage = new uint8_t[dstimg->width * dstimg->height];
      floatTouint8(guasspyr[i][j].data, outimage, dstimg->width,
                   dstimg->height);
      grayimage2colorimage(outimage, dstimg->image, dstimg->width,
                           dstimg->height);
      dstimg->Save(("guass" + int2str(i) + "_" + int2str(j) + ".bmp").c_str());
#endif
    }
  }

  float *src1;
  float *src2;
  uint64_t idx = 0;
  // build_dog_pyr
  // cout << "build_dog_pyr" << endl;
  for (int i = 0; i < nOctaves; i++) {
    uint16_t cur_w = guasspyr[i][0].width;
    uint16_t cur_h = guasspyr[i][0].height;
    for (int j = 0; j < nDogLayers; j++) {
      // cout << "i,j: " << i << " , " << j << endl;
      Dogpyr[i][j].setMat(cur_w, cur_h);
      src1 = guasspyr[i][j].data;
      src2 = guasspyr[i][j + 1].data;
      idx = 0;
      while (idx < (uint64_t)cur_w * (uint64_t)cur_h) {
        Dogpyr[i][j].data[idx] = (src2[idx] - src1[idx]);
        idx++;
      }
    }
  }
  // cout << "build_grd_rot_pyr" << endl;
  // build_grd_rot_pyr
  for (int i = 0; i < nOctaves; i++) {
    uint16_t cur_w = Dogpyr[i][0].width;
    uint16_t cur_h = Dogpyr[i][0].height;
    for (int j = 1; j <= nLayers; j++) {
      grdPyr[i][j].setMat(cur_w, cur_h);
      rotPyr[i][j].setMat(cur_w, cur_h);
      for (int y = 0; y < cur_h; y++) {
        for (int x = 0; x < cur_w; x++) {
          float dy = get_pixel_f(Dogpyr[i][j].data, cur_w, cur_h, y + 1, x) -
                     get_pixel_f(Dogpyr[i][j].data, cur_w, cur_h, y - 1, x);
          float dx = get_pixel_f(Dogpyr[i][j].data, cur_w, cur_h, y, x + 1) -
                     get_pixel_f(Dogpyr[i][j].data, cur_w, cur_h, y, x - 1);
          grdPyr[i][j].data[y * cur_w + x] = fast_sqrt_f(dy * dy + dx * dx);
          float angle = fast_atan2_f(dy, dx);
          rotPyr[i][j].data[y * cur_w + x] = angle;
        }
      }
    }
  }
  // cout << "find key point" << endl;
  //  return;
  //   detect_keypoints
  //   list<SiftKeypoint> siftkp;
  int nBins = SIFT_ORI_HIST_BINS;  // bin:36
  float *hist = new float[nBins];
  float contr_thr = 0.8 * SIFT_CONTR_THR;  // 0.8 * 0.8;
  for (int i = 0; i < nOctaves; i++) {
    uint16_t cur_w = Dogpyr[i][0].width;
    uint16_t cur_h = Dogpyr[i][0].height;

    for (int j = 1; j < nDogLayers - 1; j++) {  //
      float *HighData = Dogpyr[i][j + 1].data;
      float *CurData = Dogpyr[i][j].data;
      float *LowData = Dogpyr[i][j - 1].data;

      for (int x = SIFT_IMG_BORDER; x < cur_w - SIFT_IMG_BORDER; x++) {
        for (int y = SIFT_IMG_BORDER; y < cur_h - SIFT_IMG_BORDER; y++) {
          int idx = y * (int)cur_w + x;
          float val = CurData[idx];
          if (abs(val) >= 10) continue;

          bool bExtrema =
              (val >= contr_thr && val > HighData[idx] &&
               IsLocalMax(HighData, val, idx, (int)cur_w) &&
               IsLocalMax(CurData, val, idx, (int)cur_w) &&
               val > LowData[idx] && IsLocalMax(LowData, val, idx, (int)cur_w));

          bExtrema = (bExtrema || (val < -contr_thr && val < HighData[idx] &&
                                   IsLocalMin(HighData, val, idx, (int)cur_w) &&
                                   IsLocalMin(CurData, val, idx, (int)cur_w) &&
                                   val < LowData[idx] &&
                                   IsLocalMin(LowData, val, idx, (int)cur_w)));

          if (bExtrema) {
            SiftKeypoint kptmp;
            kptmp.octave = i;
            kptmp.layer = j;
            kptmp.xi = x;
            kptmp.yi = y;

            bool bgoodkeypoint =
                refine_local_extrema(Dogpyr, nDogLayers, kptmp);
            if (!bgoodkeypoint) continue;

            float max_mag = compute_orientation_hist_with_gradient(
                grdPyr[i][kptmp.layer], rotPyr[i][kptmp.layer], kptmp, hist);

            // direction
            float threshold = max_mag * SIFT_ORI_PEAK_RATIO;
            for (int ii = 0; ii < nBins; ii++) {
              // Use 3 points to fit a curve and find the accurate
              // location of a keypoints
              int left = ii > 0 ? ii - 1 : nBins - 1;
              int right = ii < (nBins - 1) ? ii + 1 : 0;
              float currHist = hist[ii];
              float lhist = hist[left];
              float rhist = hist[right];
              if (currHist > lhist && currHist > rhist &&
                  currHist > threshold) {
                // Refer to here:
                // http://stackoverflow.com/questions/717762/how-to-calculate-the-vertex-of-a-parabola-given-three-points
                float accu_ii = ii + 0.5f * (lhist - rhist) /
                                         (lhist - 2.0f * currHist + rhist);

                // Since bin index means the starting point of a
                // bin, so the real orientation should be bin
                // index plus 0.5. for example, angles in bin 0
                // should have a mean value of 5 instead of 0;
                accu_ii += 0.5f;
                accu_ii = accu_ii < 0        ? (accu_ii + nBins)
                          : accu_ii >= nBins ? (accu_ii - nBins)
                                             : accu_ii;
                // The magnitude should also calculate the max
                // number based on fitting But since we didn't
                // actually use it in image matching, we just
                // lazily use the histogram value.
                kptmp.mag = currHist;
                kptmp.ori = accu_ii * _2PI / nBins;
                kp.push_back(kptmp);
              }
            }
          }
        }
      }
      // cout << "key point nums:" << kp.size() << endl;
    }
  }
  // cout << "extract_descriptor" << endl;
  extract_descriptor(grdPyr, rotPyr, kp);

  // cout << "key point nums:" << siftkp.size() << endl;
  //  cout << "key point nums:" << siftkp.size() << endl;
  delete[] grayimage;
  grayimage = nullptr;
  delete[] grayimagef;
  grayimagef = nullptr;
  delete[] hist;
  hist = nullptr;
  delete[] gaussianfilter;
  gaussianfilter = nullptr;
  mat.clear();

  for (int i = 0; i < nOctaves; i++) {
    octaves[i].clear();
    for (int j = 0; j < nDogLayers; j++) {
      Dogpyr[i][j].clear();
      guasspyr[i][j].clear();
    }
  }
  for (int i = 0; i < nOctaves; i++) {
    for (int j = 1; j <= nLayers; j++) {
      grdPyr[i][j].clear();
      rotPyr[i][j].clear();
    }
  }
}

bool same_match_pair(const MatchPair &first, const MatchPair &second) {
  if (first.c1 == second.c1 && first.r1 == second.r1 && first.c2 == second.c2 &&
      first.r2 == second.r2)
    return true;
  else
    return false;
}

// brute force
int match_keypoints(list<SiftKeypoint> &kpt_list1,
                    list<SiftKeypoint> &kpt_list2, list<MatchPair> &match_list)
/*
    Parameters:
        kpt_list1:  input
        kpt_list2:  intput
        match_list:  output
*/
{
  list<SiftKeypoint>::iterator kpt1;
  list<SiftKeypoint>::iterator kpt2;

  for (kpt1 = kpt_list1.begin(); kpt1 != kpt_list1.end(); kpt1++) {
    int r1 = (int)kpt1->y;
    int c1 = (int)kpt1->x;

    float *descr1 = kpt1->descriptors;
    float score1 = (std::numeric_limits<float>::max)();  // highest score
    float score2 = (std::numeric_limits<float>::max)();  // 2nd highest score

    // find key point coordinate
    int r2 = 0, c2 = 0;
    for (kpt2 = kpt_list2.begin(); kpt2 != kpt_list2.end(); kpt2++) {
      float score = 0;
      float *descr2 = kpt2->descriptors;
      float dif;
      for (int i = 0; i < DEGREE_OF_DESCRIPTORS; i++) {
        dif = descr1[i] - descr2[i];
        score += dif * dif;
      }

      if (score < score1) {
        score2 = score1;
        score1 = score;
        r2 = (int)kpt2->y;
        c2 = (int)kpt2->x;
      } else if (score < score2) {
        score2 = score;
      }
    }

    // match
    if (fast_sqrt_f(score1 / score2) < SIFT_MATCH_NNDR_THR) {
      MatchPair mp;
      mp.r1 = r1;
      mp.c1 = c1;
      mp.r2 = r2;
      mp.c2 = c2;

      match_list.push_back(mp);
    }
  }
  // remove duplicated point
  match_list.unique(same_match_pair);

// output key point information
#if 0
  std::list<MatchPair>::iterator p;
  int match_idx = 0;
   
  for (p = match_list.begin(); p != match_list.end(); p++) {
    printf("\tNO. %3d: (%4d, %4d) -> (%4d, %4d)\n", match_idx, p->r1, p->c1,
           p->r2, p->c2);
    match_idx++;
  }
#endif
  return 0;
}
void draw_keypoints_image(SimpleImage *image, list<SiftKeypoint> kpt_list)
/*
    Parameters:
        Image:
        kpt_list:
*/
{
  list<SiftKeypoint>::iterator it;

  // int w = image->width;
  // int h = image->height;
  int r, c;

  //  cR = sigma * 4 * (2^O)
  int cR;

  //
  for (it = kpt_list.begin(); it != kpt_list.end(); it++) {
    // ratdius & location
    cR = (int)it->scale;
    if (cR <= 10) {
      cR = 10;
    }
    r = (int)it->y;
    c = (int)it->x;

    drawCircle(image->image, image->width, image->height, c, r, 1, COLOR_RED);
    drawCircle(image->image, image->width, image->height, c, r, cR + 1,
               COLOR_RED);
    float ori = it->ori;

    draw_red_orientation(image->image, image->width, image->height, c, r, ori,
                         cR);
  }
}
int draw_line_to_rgb_image(SimpleImage *Outimage, MatchPair &mp, int coloridx) {
  int r1 = mp.r1;
  int c1 = mp.c1;
  int r2 = mp.r2;
  int c2 = mp.c2;
  int w = Outimage->width;
  int h = Outimage->height;

  float k = (float)(r2 - r1) / (float)(c2 - c1);
  for (int c = c1; c < c2; c++) {
    int r = (int)(k * (c - c1) + r1);
    setPixelColor(Outimage->image, w, h, c, r, coloridx);
  }
  return 0;
}
int draw_match_lines_to_image(SimpleImage *Outimage,
                              list<MatchPair> &match_list)
/*
    Parameters:
        filename:
        image1:
        image2:
        match_list:
*/
{
  int w = Outimage->width;
  // int h = Outimage->height;

  list<MatchPair>::iterator p;
  for (p = match_list.begin(); p != match_list.end(); p++) {
    MatchPair mp;
    mp.r1 = p->r1;
    mp.c1 = p->c1;
    mp.r2 = p->r2;
    mp.c2 = p->c2 + w / 2;
    draw_line_to_rgb_image(Outimage, mp, COLOR_RED);
  }

  return 0;
}
