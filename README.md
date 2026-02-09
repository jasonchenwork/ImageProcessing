
# Introduction

ImageProcessing

This is a project for practicing programming image processing algorithms.  

I implement image processing algorithms in C++ without using im libraries.  

Support Oepn&Save bmp files without any libraries.  
  
```C++
#include <bits/stdc++.h>
#include "../include/SimpleImage.hpp"

int main() {
  SimpleImage* img = new SimpleImage();
  bool res = img->Load("img/pattern.bmp");
  if (!res) {
    cout << "load file fail" << endl;
  }
  img->Save("img/test05.bmp");
  delete img;
  return 0;
}
```  

# **Algorithms**

[White balance](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/whitebalance.md)  

[HistogramEqualization](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/Histogramequalization.md)  

[Non-local means algorithm](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/NLM.md)  

[Non-local means algorithm with Integral Image skill](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/NLMwithintegrallmage.md)  

[Non-local means algorithm with Multi-thread](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/NLMwiththread.md)  

[Non-local means algorithm with Integral Image skill and Multi-thread](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/NLMwithintegrallmagethread.md)  

[Non-local means algorithm with OpenMP](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/NLMwithintegrallmageopenmp.md)  

Edge Detection  

Morphology  
  
[Harris Corner Detector](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/harriscorner.md)  

[SIFT](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/SIFT.md)
  
[FFT](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/FFT.md)  

[GaussianBlur](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/GaussianBlur.md)  

[SaliencyMap](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/saliencymap.md)  
  
[DWT](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/DWT.md)  

[DCT](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/DCT.md)

[Adaboostfacedetection](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/adaboostfacedetection.md)  

[FastMedianFilter](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/FastMedianFilter.md)  

[StereoImageSGM](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/StereoImageSGM.md)  

[OpticalFlowHS](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/OpticalFlowHS.md)  

[OpticalFlowLK](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/OpticalFlowLK.md)  

[CCL](https://github.com/jasonchenwork/ImageProcessing/blob/main/md/CCL.md)  
# **Author Information**

Name
Jason Chen
  
# **Contact Information**

Email: <laevatain103@gmail.com>

GitHub: [Jason chen](https://github.com/jasonchenwork)

LinkedIn: [Jason Chen](https://www.linkedin.com/in/tsungchun-chen-996729186/)
