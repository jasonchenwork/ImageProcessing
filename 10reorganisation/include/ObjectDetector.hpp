#ifndef OBJECT_DETECTOR_HPP
#define OBJECT_DETECTOR_HPP

#include <string>
#include <vector>

#include "../include/IntegralImage2.hpp"

#ifndef dmin
#define dmin(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef dmxn
#define dmxn(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef dsqrt
#define dsqrt(x) ((x) * (x))
#endif

typedef struct {
  int x;
  int y;
  int w;
  int h;
} ORectangle;

class ObjectDetector {
 protected:
  double factor;
  int baseWidth, baseHeight;
  int lastWidth, lastHeight;
  std::vector<double> steps;
  // 純虛擬函式：強迫子類別實作核心邏輯
  virtual bool ClassifierCompute(IntegralImage2* im, ORectangle window,
                                 double scale) = 0;
  virtual void update(int width, int height) = 0;

 public:
  std::vector<ORectangle> detectedObjects;
  std::vector<ORectangle> lastObject;

  virtual ~ObjectDetector() {}

  // 純虛擬函式：子類別一定要實作
  virtual void LoadXML(std::string path) = 0;

  // 範本方法：可以先在基類寫好通用的邏輯
  virtual std::vector<ORectangle> ProcessMultiScaleImage(unsigned char** Src,
                                                         int width,
                                                         int height) = 0;
  virtual std::vector<ORectangle> ProcessMultiScaleWindow(unsigned char** Src,
                                                          int width,
                                                          int height) = 0;
};

#endif