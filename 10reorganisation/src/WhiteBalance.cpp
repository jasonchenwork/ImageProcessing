#include "../include/WhiteBalance.hpp"

void whitebalanceGWA(const uint8_t *imageSource, uint8_t *imageTarget,
                     uint16_t width, uint16_t height) {
  uint16_t y, x;
  uint16_t sR, sG, sB;
  int16_t NewR, NewB, NewG;

  double AvgR = 0, AvgG = 0, AvgB = 0;
  double SumR = 0, SumG = 0, SumB = 0;
  double grayValue;
  double scaleValueR, scaleValueG, scaleValueB;

  for (y = 0; y != height; ++y) {
    for (x = 0; x != width; ++x) {
      sR = *(imageSource + 3 * (width * y + x) + 2);
      sG = *(imageSource + 3 * (width * y + x) + 1);
      sB = *(imageSource + 3 * (width * y + x) + 0);

      SumR += sR;
      SumB += sB;
      SumG += sG;
    }
  }

  AvgR = (double)SumR / (width * height);
  AvgG = (double)SumG / (width * height);
  AvgB = (double)SumB / (width * height);

  grayValue = (AvgR + AvgG + AvgG) / 3.0;

  scaleValueR = (double)grayValue / AvgR;
  scaleValueB = (double)grayValue / AvgB;
  scaleValueG = (double)grayValue / AvgG;

  for (y = 0; y != height; ++y) {
    for (x = 0; x != width; ++x) {
      NewR = (scaleValueR * (*(imageSource + 3 * (width * y + x) + 2)));
      NewG = (scaleValueG * (*(imageSource + 3 * (width * y + x) + 1)));
      NewB = (scaleValueB * (*(imageSource + 3 * (width * y + x) + 0)));

      NewR = (NewR > 255) ? 255 : NewR;
      NewG = (NewG > 255) ? 255 : NewG;
      NewB = (NewB > 255) ? 255 : NewB;

      NewR = (NewR < 0) ? 0 : NewR;
      NewG = (NewG < 0) ? 0 : NewG;
      NewB = (NewB < 0) ? 0 : NewB;

      *(imageTarget + 3 * (width * y + x) + 2) = (uint8_t)NewR;
      *(imageTarget + 3 * (width * y + x) + 1) = (uint8_t)NewG;
      *(imageTarget + 3 * (width * y + x) + 0) = (uint8_t)NewB;
    }
  }
}
void whitebalance2005Lam(const uint8_t *imageSource, uint8_t *imageTarget,
                         uint16_t width, uint16_t height) {
  uint16_t y, x;
  uint16_t sR, sG, sB;
  float NewR = 0, NewB = 0;
  // float Ir2 = 0, Ir = 0, Ib2 = 0, Ib = 0;
  float Sum_Ir2 = 0, Sum_Ir = 0, Sum_Ib2 = 0, Sum_Ib = 0;
  float MaxIr2 = 0, MaxIr = 0, MaxIb2 = 0, MaxIb = 0;
  // float Ig = 0;
  float Sum_Ig = 0, MaxIg = 0;

  for (y = 0; y != height; ++y) {
    for (x = 0; x != width; ++x) {
      sR = *(imageSource + 3 * (width * y + x) + 2);
      sG = *(imageSource + 3 * (width * y + x) + 1);
      sB = *(imageSource + 3 * (width * y + x) + 0);

      Sum_Ir += sR;
      Sum_Ib += sB;
      Sum_Ig += sG;

      Sum_Ir2 += sR * sR;
      Sum_Ib2 += sB * sB;

      if (sR > MaxIr) {
        MaxIr = (float)sR;
      }
      if (sB > MaxIb) {
        MaxIb = (float)sB;
      }
      if (sG > MaxIg) {
        MaxIg = (float)sG;
      }

      if ((sR * sR) > MaxIr2) {
        MaxIr2 = sR * sR;
      }
      if ((sB * sB) > MaxIb2) {
        MaxIb2 = sB * sB;
      }
    }
  }

  // slove the equations
  float uR = 0, vR = 0;
  float uB = 0, vB = 0;
  float baseR = (Sum_Ir2 * MaxIr - Sum_Ir * MaxIr2);
  float baseB = (Sum_Ib2 * MaxIb - Sum_Ib * MaxIb2);

  uR = (Sum_Ig * MaxIr - Sum_Ir * MaxIg) / baseR;
  vR = (Sum_Ir2 * MaxIg - Sum_Ig * MaxIr2) / baseR;

  uB = (Sum_Ig * MaxIb - Sum_Ib * MaxIg) / baseB;
  vB = (Sum_Ib2 * MaxIg - Sum_Ig * MaxIb2) / baseB;

  for (y = 0; y != height; ++y) {
    for (x = 0; x != width; ++x) {
      sR = *(imageSource + 3 * (width * y + x) + 2);
      sG = *(imageSource + 3 * (width * y + x) + 1);
      sB = *(imageSource + 3 * (width * y + x) + 0);

      NewR = uR * (sR * sR) + vR * sR;
      NewB = uB * (sB * sB) + vB * sB;

      if (NewR > 255) {
        NewR = 255;
      }
      if (NewB > 255) {
        NewB = 255;
      }
      if (NewR < 0) {
        NewR = 0;
      }
      if (NewB < 0) {
        NewB = 0;
      }
      *(imageTarget + 3 * (width * y + x) + 2) = (char)NewR;
      *(imageTarget + 3 * (width * y + x) + 1) = sG;
      *(imageTarget + 3 * (width * y + x) + 0) = (char)NewB;
    }
  }
}