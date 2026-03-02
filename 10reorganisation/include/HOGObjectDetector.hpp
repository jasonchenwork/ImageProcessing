#ifndef HOGOBJECTDETECTOR_HPP
#define HOGROBJECTDETECTOR_HPP
#include <bits/stdc++.h>

#include "../include/HaarObjectDetector.hpp"
#include "../include/IntegralImage2.hpp"
#include "../include/Utility.hpp"
using namespace std;
/*
ref:
https://github.com/Schmetzler/opencv3_CascadeHOG
https://github.com/opencv/opencv/issues/4989
*/

//
// HOG DataStruct
//

typedef struct {
  int x;
  int y;
  int w;
  int h;
  int idx;
  int ScaleX;
  int ScaleY;
  int ScaleWidth;
  int ScaleHeight;
  // double ScaleWeight;
} HOGRectangle;

typedef struct {
  vector<HOGRectangle> Rectangle;

  bool Tilted;  // no use
} HOGFeature;

typedef struct {
  // HOGFeature Feature;

  double Node_Threshold;
  double Left_value;
  double Right_value;
  int LeftNodeIndex;
  int RightNodeIndex;
  int FeatureIDX;
} HOGNode;

typedef struct {
  vector<HOGNode> Node;
} HOGStageTree;

typedef struct {
  double Stage_Threshold;
  int count;
  vector<HOGStageTree> Tree;

} HOGStage;

typedef struct {
  int Width;
  int Heigh;
  vector<HOGStage> Stage;
} HOGCascadeStage;

class HOGObjectDetector {
 private:
  vector<double> steps;
  void update(int width, int height);
  // void update(int width, int height);
  // bool HaarClassifierCompute(IntegralImage2* im, ORectangle window,double
  // scale); bool HaarClassifierCompute2(IntegralImage2* im, ORectangle
  // window,double scale);
  // double FeatureGetSum(IntegralImage2* im, int x, int y, HaarFeature
  // HF,double scale); double FeatureGetSum2(IntegralImage2* im, int x, int y,
  // HaarFeature HF,double scale); void updateFeature(double scale);

 public:
  // costructor
  HOGObjectDetector(HOGCascadeStage myHOGCascadeStage);
  HOGObjectDetector();
  ~HOGObjectDetector();

  // members

  bool HOGClassifierCompute(IntegralImage2* im, ORectangle rectangle,
                            double scale);
  HOGCascadeStage myHOGCascadeStage;
  vector<HOGRectangle> features;
  vector<ORectangle> lastObject;
  vector<ORectangle> detectedObjects;

  double factor;

  int baseWidth;
  int baseHeight;

  int lastWidth;
  int lastHeight;

  // Classifier

  // methods
  void LoadXML(string path);
  double FeatureGetSum(IntegralImage2* im, int x, int y, HOGRectangle HOGRect,
                       double scale);
  std::vector<ORectangle> ProcessMultiScaleImage(unsigned char** Src, int width,
                                                 int height);
  std::vector<ORectangle> ProcessMultiScaleWindow(unsigned char** Src,
                                                  int width, int height);
  // vector<ORectangle> ProcessMultiScaleWindow(unsigned char** Src, int
  // width,int height);
  // vector<ORectangle> ProcessMultiScaleImage(unsigned char** Src, int
  // width,int height);
};

#endif