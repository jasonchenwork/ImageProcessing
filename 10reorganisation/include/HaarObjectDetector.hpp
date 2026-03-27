#ifndef HAAROBJECTDETECTOR_HPP
#define HAAROBJECTDETECTOR_HPP

#include "ObjectDetector.hpp"
using namespace std;

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

class HaarObjectDetector : public ObjectDetector {
 private:
  // Haar 專有的私有方法
  void update(int width, int height) override;
  bool ClassifierCompute(IntegralImage2* im, ORectangle window,
                         double scale) override;
  bool ClassifierCompute2(IntegralImage2* im, ORectangle window, double scale);
  double FeatureGetSum(IntegralImage2* im, int x, int y, HaarFeature HF);
  double FeatureGetSum2(IntegralImage2* im, int x, int y, HaarFeature HF);
  void updateFeature(double scale);

 public:
  HaarCascadeStage myHaarCascadeStage;
  HaarObjectDetector(HaarCascadeStage myHaarCascadeStage);
  HaarObjectDetector();
  ~HaarObjectDetector() override;  // 記得 override 解構子
  // 實作基類的虛擬函式
  void LoadXML(string path) override;
  vector<ORectangle> ProcessMultiScaleImage(unsigned char** Src, int width,
                                            int height) override;
  vector<ORectangle> ProcessMultiScaleWindow(unsigned char** Src, int width,
                                             int height);
};

#endif