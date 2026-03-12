#include "../include/DeBlur.hpp"

#include "../include/FFT.hpp"
#include "../include/SimpleImage.hpp"
#include "../include/SimpleMat.hpp"
#include "../include/Utility.hpp"

void getPSF(SimpleMat<double>& PSF, int w, int h, int len, int theta) {
  double angleRed = theta * PI / 180.0;
  int centerx = w / 2;
  int centery = h / 2;

  int x0 = centerx - 0.5 * static_cast<int>((len)*cos(angleRed));
  int y0 = centery - 0.5 * static_cast<int>((len)*sin(angleRed));
  int x1 = centerx + 0.5 * static_cast<int>((len)*cos(angleRed));
  int y1 = centery + 0.5 * static_cast<int>((len)*sin(angleRed));

  drawLine(PSF.data, x0, y0, x1, y1, w, h);
#if 0
  SimpleImage* dstimg = new SimpleImage(w, h);
  uint8_t* grayimage = new uint8_t[w * h];

  TypeConver(PSF.data, grayimage, (uint16_t)w, (uint16_t)h);
  grayimage2colorimage(grayimage, dstimg->image, w, h);
  dstimg->Save("img/PSFtest.bmp");
  delete dstimg;
  delete[] grayimage;
#endif
  // normalize PSF
  PSF = PSF / PSF.sum();
}
void Merge3MatsToimage(SimpleMat<double> rgb[3], uint8_t* image, int w, int h) {
  for (int i = 0; i < w * h; i++) {
    image[i * 3 + 0] = rgb[0].data[i];  // B
    image[i * 3 + 1] = rgb[1].data[i];  // G
    image[i * 3 + 2] = rgb[2].data[i];  // R
  }
}
void imageSplit3Mat(uint8_t* image, SimpleMat<double> rgb[3], int w, int h) {
  for (int i = 0; i < w * h; i++) {
    rgb[0].data[i] = (double)image[i * 3 + 0];  // B
    rgb[1].data[i] = (double)image[i * 3 + 1];  // G
    rgb[2].data[i] = (double)image[i * 3 + 2];  // R
  }
}

void DeBlurWithWienerFilter(uint8_t* src, uint8_t* dst, int w, int h, int len,
                            int theta, double SNR) {
  uint8_t* grayimage = new uint8_t[w * h];

  SimpleMat<double> PSF(w, h);
  getPSF(PSF, w, h, len, theta);       // Example: length=15, angle=30 degrees
  FFTshift(PSF.data, PSF.data, w, h);  // Shift PSF to center for DFT

  // G = H* / (|H|^2 + SNR)
  // H*: conjugate of H
  // |H|^2: magnitude squared of H
  ImageDFT motionblurredDFT(PSF);
  // double SNR = 0.04;
  SimpleMat<double> motionblurredMagnitude = motionblurredDFT.getMagnitude();

  SimpleMat<double> H_mag_sq =
      (motionblurredMagnitude.mul(motionblurredMagnitude)) +
      SNR;  // Add a small constant to avoid division by zero

  // conjugate
  ImageDFT H_conj = motionblurredDFT.conjugate();

  ImageDFT WienerFilter = H_conj.div(H_mag_sq);

  SimpleMat<double>* rgb = new SimpleMat<double>[3];
  for (int i = 0; i < 3; i++) {
    rgb[i] = SimpleMat<double>(w, h);
  }
  imageSplit3Mat(src, rgb, w, h);

  for (int i = 0; i < 3; i++) {
    // cout << "i: " << i << endl;
    ImageDFT imageDFT(rgb[i]);

    ImageDFT deblurredImage = imageDFT * WienerFilter;
    SimpleMat<double> outimage = deblurredImage.getIDFTImage();

    // clamp to [0, 255]
    clampImage(rgb[i].data, outimage.data, w, h, 0, 255);
  }

  Merge3MatsToimage(rgb, dst, w, h);

  delete[] grayimage;
  delete[] rgb;
}