#ifndef HAAROBJECTDETECTOR_HPP
#define HAAROBJECTDETECTOR_HPP
#include <bits/stdc++.h>

#include "../include/IntegralImage2.hpp"
#include "../include/Utility.hpp"
using namespace std;

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

//
// Haar DataStruct
//
typedef struct {
  int x;
  int y;
  int w;
  int h;
  int weight;
  int ScaleX;
  int ScaleY;
  int ScaleWidth;
  int ScaleHeight;
  double ScaleWeight;
} HaarRectangle;

typedef struct {
  vector<HaarRectangle> Rectangle;

  bool Tilted;  // no use
} HaarFeature;

typedef struct {
  HaarFeature Feature;
  double Node_Threshold;
  double Left_value;
  double Right_value;
  int LeftNodeIndex;
  int RightNodeIndex;
} HaarNode;

typedef struct {
  vector<HaarNode> Node;
} StageTree;

typedef struct {
  double Stage_Threshold;
  // int count;
  vector<StageTree> Tree;

} HaarStage;

typedef struct {
  int Width;
  int Heigh;
  vector<HaarStage> Stage;
} HaarCascadeStage;

class HaarObjectDetector {
 private:
  void update(int width, int height);
  bool HaarClassifierCompute(IntegralImage2* im, ORectangle window,
                             double scale);
  bool HaarClassifierCompute2(IntegralImage2* im, ORectangle window,
                              double scale);
  double FeatureGetSum(IntegralImage2* im, int x, int y, HaarFeature HF,
                       double scale);
  double FeatureGetSum2(IntegralImage2* im, int x, int y, HaarFeature HF,
                        double scale);
  void updateFeature(double scale);

 public:
  // costructor
  HaarObjectDetector(HaarCascadeStage myHaarCascadeStage);
  HaarObjectDetector();
  ~HaarObjectDetector();

  // members
  HaarCascadeStage myHaarCascadeStage;
  vector<ORectangle> lastObject;
  vector<ORectangle> detectedObjects;

  double factor;

  int baseWidth;
  int baseHeight;

  int lastWidth;
  int lastHeight;
  vector<double> steps;

  // Classifier

  // methods
  void LoadXML(string path);
  vector<ORectangle> ProcessMultiScaleWindow(unsigned char** Src, int width,
                                             int height);
  vector<ORectangle> ProcessMultiScaleImage(unsigned char** Src, int width,
                                            int height);
};
#endif