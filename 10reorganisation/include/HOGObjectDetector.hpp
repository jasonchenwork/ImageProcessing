#ifndef HOGOBJECTDETECTOR_HPP
#define HOGROBJECTDETECTOR_HPP
#include <bits/stdc++.h>

#include "../include/ObjectDetector.hpp"

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

class HOGObjectDetector : public ObjectDetector {
 private:
  void update(int width, int height) override;
  bool ClassifierCompute(IntegralImage2* im, ORectangle window,
                         double scale) override;
  double FeatureGetSum(IntegralImage2* im, int x, int y, HOGRectangle HOGRect,
                       double scale);

 public:
  // costructor
  HOGObjectDetector(HOGCascadeStage myHOGCascadeStage);
  HOGObjectDetector();
  ~HOGObjectDetector() override;

  // members

  HOGCascadeStage myHOGCascadeStage;
  vector<HOGRectangle> features;

  // Classifier

  // methods
  void LoadXML(string path) override;

  std::vector<ORectangle> ProcessMultiScaleImage(unsigned char** Src, int width,
                                                 int height) override;
  std::vector<ORectangle> ProcessMultiScaleWindow(unsigned char** Src,
                                                  int width,
                                                  int height) override;
};
#endif