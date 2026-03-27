
#include "../include/HaarObjectDetector.hpp"

#include "../include/Tinyxml.hpp"
#include "../include/Utility.hpp"
HaarObjectDetector::HaarObjectDetector(HaarCascadeStage Cascade) {
  factor = 1.1;

  myHaarCascadeStage = Cascade;
  baseWidth = Cascade.Width;
  baseHeight = Cascade.Heigh;
};
HaarObjectDetector::HaarObjectDetector() { factor = 1.1; };
HaarObjectDetector::~HaarObjectDetector() {

};
void HaarObjectDetector::updateFeature(double scale) {
  for (int i = 0; i < (int)myHaarCascadeStage.Stage.size(); i++) {
    for (int j = 0; j < (int)myHaarCascadeStage.Stage[i].Tree.size(); j++) {
      for (int z = 0; z < (int)myHaarCascadeStage.Stage[i].Tree[j].Node.size();
           z++) {
        if (myHaarCascadeStage.Stage[i]
                .Tree[j]
                .Node[z]
                .Feature.Rectangle.size() == 2) {
          // part B
          int b_w = myHaarCascadeStage.Stage[i]
                        .Tree[j]
                        .Node[z]
                        .Feature.Rectangle[1]
                        .w;
          int b_h = myHaarCascadeStage.Stage[i]
                        .Tree[j]
                        .Node[z]
                        .Feature.Rectangle[1]
                        .h;
          double b_wei = myHaarCascadeStage.Stage[i]
                             .Tree[j]
                             .Node[z]
                             .Feature.Rectangle[1]
                             .weight;

//
#if 0
          double b_orig_feature_size = (double)(b_w * b_h);
          double b_orig_window_size = (double)(baseWidth * baseHeight);
          double b_feature_size = (b_w * scale) * (b_h * scale);
          double b_target_ratio = b_orig_feature_size / b_orig_window_size;
          double b_correction_ratio = b_target_ratio / b_feature_size;
          double b_scaling_wei = b_wei * b_correction_ratio;
#endif
          double b_scaling_wei =
              b_wei * (1.0 / (baseWidth * baseHeight * scale * scale));

          //
          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[1]
              .ScaleWidth = (int)(b_w * scale);
          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[1]
              .ScaleHeight = (int)(b_h * scale);
          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[1]
              .ScaleWeight = b_scaling_wei;

          // part A
          int a_w = myHaarCascadeStage.Stage[i]
                        .Tree[j]
                        .Node[z]
                        .Feature.Rectangle[0]
                        .w;
          int a_h = myHaarCascadeStage.Stage[i]
                        .Tree[j]
                        .Node[z]
                        .Feature.Rectangle[0]
                        .h;

          // double a_wei =
          // myHaarCascadeStage.Stage[i].Tree[j].Node[z].Feature.Rectangle[0].weight;

          int b_area = myHaarCascadeStage.Stage[i]
                           .Tree[j]
                           .Node[z]
                           .Feature.Rectangle[1]
                           .ScaleWidth *
                       myHaarCascadeStage.Stage[i]
                           .Tree[j]
                           .Node[z]
                           .Feature.Rectangle[1]
                           .ScaleHeight;

          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[0]
              .ScaleWidth = (int)(a_w * scale);
          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[0]
              .ScaleHeight = (int)(a_h * scale);

          int a_area = myHaarCascadeStage.Stage[i]
                           .Tree[j]
                           .Node[z]
                           .Feature.Rectangle[0]
                           .ScaleWidth *
                       myHaarCascadeStage.Stage[i]
                           .Tree[j]
                           .Node[z]
                           .Feature.Rectangle[0]
                           .ScaleHeight;

          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[0]
              .ScaleWeight = -(b_area * b_scaling_wei) / a_area;

          // update ALL SScaleXY
          for (int zi = 0; zi < 2; zi++) {
            int tmp_x = myHaarCascadeStage.Stage[i]
                            .Tree[j]
                            .Node[z]
                            .Feature.Rectangle[zi]
                            .x;
            int tmp_y = myHaarCascadeStage.Stage[i]
                            .Tree[j]
                            .Node[z]
                            .Feature.Rectangle[zi]
                            .y;

            myHaarCascadeStage.Stage[i]
                .Tree[j]
                .Node[z]
                .Feature.Rectangle[zi]
                .ScaleX = (int)(tmp_x * scale);
            myHaarCascadeStage.Stage[i]
                .Tree[j]
                .Node[z]
                .Feature.Rectangle[zi]
                .ScaleY = (int)(tmp_y * scale);
          }

        } else {
          // max = 3

          // Part B C
          int b_w = myHaarCascadeStage.Stage[i]
                        .Tree[j]
                        .Node[z]
                        .Feature.Rectangle[1]
                        .w;
          int b_h = myHaarCascadeStage.Stage[i]
                        .Tree[j]
                        .Node[z]
                        .Feature.Rectangle[1]
                        .h;
          double b_wei = myHaarCascadeStage.Stage[i]
                             .Tree[j]
                             .Node[z]
                             .Feature.Rectangle[1]
                             .weight;
#if 0
          double b_orig_feature_size = (double)(b_w * b_h);
          double b_orig_window_size = (double)(baseWidth * baseHeight);
          double b_feature_size = (b_w * scale) * (b_h * scale);
          double b_target_ratio = b_orig_feature_size / b_orig_window_size;
          double b_correction_ratio = b_target_ratio / b_feature_size;
          double b_scaling_wei = b_wei * b_correction_ratio;
#endif
          // b_scaling_wei = b_wei * scale;
          double b_scaling_wei =
              b_wei * (1.0 / (baseWidth * baseHeight * scale * scale));

          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[1]
              .ScaleWidth = (int)(b_w * scale);
          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[1]
              .ScaleHeight = (int)(b_h * scale);
          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[1]
              .ScaleWeight = b_scaling_wei;

          int c_w = myHaarCascadeStage.Stage[i]
                        .Tree[j]
                        .Node[z]
                        .Feature.Rectangle[2]
                        .w;
          int c_h = myHaarCascadeStage.Stage[i]
                        .Tree[j]
                        .Node[z]
                        .Feature.Rectangle[2]
                        .h;
          double c_wei = myHaarCascadeStage.Stage[i]
                             .Tree[j]
                             .Node[z]
                             .Feature.Rectangle[2]
                             .weight;

#if 0
          double c_orig_feature_size = (double)(c_w * c_h);
          double c_orig_window_size = (double)(baseWidth * baseHeight);
          double c_feature_size = (c_w * scale) * (c_h * scale);
          double c_target_ratio = c_orig_feature_size / c_orig_window_size;
          double c_correction_ratio = c_target_ratio / c_feature_size;
          double c_scaling_wei = c_wei * c_correction_ratio;
#endif
          // c_scaling_wei = c_wei * scale;
          double c_scaling_wei =
              c_wei * (1.0 / (baseWidth * baseHeight * scale * scale));

          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[2]
              .ScaleWidth = (int)(c_w * scale);
          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[2]
              .ScaleHeight = (int)(c_h * scale);
          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[2]
              .ScaleWeight = c_scaling_wei;

          // Part A

          int a_w = myHaarCascadeStage.Stage[i]
                        .Tree[j]
                        .Node[z]
                        .Feature.Rectangle[0]
                        .w;
          int a_h = myHaarCascadeStage.Stage[i]
                        .Tree[j]
                        .Node[z]
                        .Feature.Rectangle[0]
                        .h;

          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[0]
              .ScaleWidth = (int)(a_w * scale);
          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[0]
              .ScaleHeight = (int)(a_h * scale);

          int a_area = myHaarCascadeStage.Stage[i]
                           .Tree[j]
                           .Node[z]
                           .Feature.Rectangle[0]
                           .ScaleWidth *
                       myHaarCascadeStage.Stage[i]
                           .Tree[j]
                           .Node[z]
                           .Feature.Rectangle[0]
                           .ScaleHeight;

          int b_area = myHaarCascadeStage.Stage[i]
                           .Tree[j]
                           .Node[z]
                           .Feature.Rectangle[1]
                           .ScaleWidth *
                       myHaarCascadeStage.Stage[i]
                           .Tree[j]
                           .Node[z]
                           .Feature.Rectangle[1]
                           .ScaleHeight;
          int c_area = myHaarCascadeStage.Stage[i]
                           .Tree[j]
                           .Node[z]
                           .Feature.Rectangle[2]
                           .ScaleWidth *
                       myHaarCascadeStage.Stage[i]
                           .Tree[j]
                           .Node[z]
                           .Feature.Rectangle[2]
                           .ScaleHeight;

          myHaarCascadeStage.Stage[i]
              .Tree[j]
              .Node[z]
              .Feature.Rectangle[0]
              .ScaleWeight =
              -(b_area * b_scaling_wei + c_area * c_scaling_wei) /
              (double)a_area;

          for (int zi = 0; zi < 3; zi++) {
            int tmp_x = myHaarCascadeStage.Stage[i]
                            .Tree[j]
                            .Node[z]
                            .Feature.Rectangle[zi]
                            .x;
            int tmp_y = myHaarCascadeStage.Stage[i]
                            .Tree[j]
                            .Node[z]
                            .Feature.Rectangle[zi]
                            .y;

            myHaarCascadeStage.Stage[i]
                .Tree[j]
                .Node[z]
                .Feature.Rectangle[zi]
                .ScaleX = (int)(tmp_x * scale);
            myHaarCascadeStage.Stage[i]
                .Tree[j]
                .Node[z]
                .Feature.Rectangle[zi]
                .ScaleY = (int)(tmp_y * scale);
          }
        }
      }
    }
  }
}
void HaarObjectDetector::update(int width, int height) {
  steps.clear();
  double start = 1.0;
  double stop = dmin(width / (double)baseWidth, height / (double)baseHeight);
  double step = factor;
  for (double f = start; f < stop; f *= step) steps.push_back(f);
};
double HaarObjectDetector::FeatureGetSum2(IntegralImage2* im, int x, int y,
                                          HaarFeature HF) {
  double sum = 0.0;
  if (!HF.Tilted) {
    for (int i = 0; i < (int)HF.Rectangle.size(); i++) {
      sum += im->GetSum(x + (HF.Rectangle[i].x), y + HF.Rectangle[i].y,
                        HF.Rectangle[i].w, HF.Rectangle[i].h) *
             (HF.Rectangle[i].weight * (1.0 / (baseWidth * baseHeight)));
    }
  }

  return sum;
};
double HaarObjectDetector::FeatureGetSum(IntegralImage2* im, int x, int y,
                                         HaarFeature HF) {
  double sum = 0.0;

  if (!HF.Tilted) {
    for (int i = 0; i < (int)HF.Rectangle.size(); i++) {
#if 1
      sum +=
          im->GetSum(x + (HF.Rectangle[i].ScaleX), y + HF.Rectangle[i].ScaleY,
                     HF.Rectangle[i].ScaleWidth, HF.Rectangle[i].ScaleHeight) *
          (HF.Rectangle[i].ScaleWeight);
#else
      sum +=
          im->GetSum(x + (HF.Rectangle[i].ScaleX), y + HF.Rectangle[i].ScaleY,
                     HF.Rectangle[i].ScaleWidth, HF.Rectangle[i].ScaleHeight) *
          (HF.Rectangle[i].weight) *
          (1.0 / (scale * scale *
                  (HF.Rectangle[i].ScaleWidth * HF.Rectangle[i].ScaleHeight) /
                  (baseHeight * baseWidth)));
#endif
    }
  }
  return sum;
};
bool HaarObjectDetector::ClassifierCompute2(IntegralImage2* im,
                                            ORectangle rectangle,
                                            double scale) {
  (void)scale;  // 消除警告
  int x = rectangle.x;
  int y = rectangle.y;
  int w = rectangle.w;
  int h = rectangle.h;
  double invArea = 1.0 / (myHaarCascadeStage.Width * myHaarCascadeStage.Heigh);

  double mean = im->GetSum(x, y, w, h) * invArea;

  double factor = im->GetSum2(x, y, w, h) * invArea - (mean * mean);

  factor = (factor >= 0.0) ? sqrt(factor) : 1.0;
  for (int i = 0; i < (int)myHaarCascadeStage.Stage.size(); i++) {
    double value = 0.0;
    for (int j = 0; j < (int)myHaarCascadeStage.Stage[i].Tree.size(); j++) {
      int current = 0;

      for (int z = 0; z < (int)myHaarCascadeStage.Stage[i].Tree[j].Node.size();
           z++) {
        double sum = FeatureGetSum2(
            im, x, y, myHaarCascadeStage.Stage[i].Tree[j].Node[z].Feature);

        if (sum < myHaarCascadeStage.Stage[i].Tree[j].Node[z].Node_Threshold *
                      factor) {
          value += myHaarCascadeStage.Stage[i].Tree[j].Node[z].Left_value;
          current = myHaarCascadeStage.Stage[i].Tree[j].Node[z].LeftNodeIndex;
        } else {
          value += myHaarCascadeStage.Stage[i].Tree[j].Node[z].Right_value;
          current = myHaarCascadeStage.Stage[i].Tree[j].Node[z].RightNodeIndex;
        }

        if (current == -1) {
          break;
        }

      }  // end z node

    }  // end j tree

    if (value < myHaarCascadeStage.Stage[i].Stage_Threshold) {
      // If it is, the stage has rejected the current
      // image and it doesn't contains our object.
      // myHaarCascadeStage.Stage[i].count++;

      return false;
    } else {
      // Current stage has accepted
    }

  }  // end i stage
  // cout << "pass" << endl;
  // myHaarCascadeStage.Stage[myHaarCascadeStage.Stage.size() - 1].count++;
  return true;  // The image has been detected.
}
bool HaarObjectDetector::ClassifierCompute(IntegralImage2* im,
                                           ORectangle rectangle, double scale) {
  int x = rectangle.x;
  int y = rectangle.y;
  int w = rectangle.w;
  int h = rectangle.h;

  double invArea = 1.0 / (myHaarCascadeStage.Width * scale *
                          myHaarCascadeStage.Heigh * scale);

  double mean = im->GetSum(x, y, w, h) * invArea;

  double factor = im->GetSum2(x, y, w, h) * invArea - (mean * mean);

  factor = (factor >= 0.0) ? sqrt(factor) : 1.0;

  for (int i = 0; i < (int)myHaarCascadeStage.Stage.size(); i++) {
    double value = 0.0;
    for (int j = 0; j < (int)myHaarCascadeStage.Stage[i].Tree.size(); j++) {
      int current = 0;

      for (int z = 0; z < (int)myHaarCascadeStage.Stage[i].Tree[j].Node.size();
           z++) {
        double sum = FeatureGetSum(
            im, x, y, myHaarCascadeStage.Stage[i].Tree[j].Node[z].Feature);

        if (sum < myHaarCascadeStage.Stage[i].Tree[j].Node[z].Node_Threshold *
                      factor) {
          value += myHaarCascadeStage.Stage[i].Tree[j].Node[z].Left_value;
          current = myHaarCascadeStage.Stage[i].Tree[j].Node[z].LeftNodeIndex;
        } else {
          value += myHaarCascadeStage.Stage[i].Tree[j].Node[z].Right_value;
          current = myHaarCascadeStage.Stage[i].Tree[j].Node[z].RightNodeIndex;
        }

        if (current == -1) {
          break;
        }

      }  // end z node

    }  // end j tree

    if (value < myHaarCascadeStage.Stage[i].Stage_Threshold) {
      // If it is, the stage has rejected the current
      // image and it doesn't contains our object.

      return false;
    } else {
      // Current stage has accepted
    }

  }  // end i stage

  return true;  // The image has been detected.
}

std::vector<ORectangle> HaarObjectDetector::ProcessMultiScaleWindow(
    unsigned char** Src, int width, int height) {
  IntegralImage2* integralImage = new IntegralImage2(width, height);
  integralImage->FromImage(Src);

  detectedObjects.clear();

  if (steps.size() < 1) update(width, height);

  ORectangle window;

  if (steps.size() == 0) {
    steps.push_back(1);
  }

  for (int i = 0; i < (int)steps.size(); i++) {
    double scaling = steps[i];

    updateFeature(scaling);
    window.w = (int)(baseWidth * scaling);
    window.h = (int)(baseHeight * scaling);

    int xStep = window.w >> 3;
    int yStep = window.h >> 3;

    int xEnd = width - window.w;
    int yEnd = height - window.h;

    for (int y = 0; y < yEnd; y += yStep) {
      window.y = y;
      for (int x = 0; x < xEnd; x += xStep) {
        window.x = x;
        if (ClassifierCompute(integralImage, window, scaling)) {
          detectedObjects.push_back(window);
        }
      }
    }
  }

  return detectedObjects;
};
std::vector<ORectangle> HaarObjectDetector::ProcessMultiScaleImage(
    unsigned char** Src, int width, int height) {
  detectedObjects.clear();

  double scale = 0;
  for (scale = 1;; scale *= factor) {
    int process_width = round(width / scale);
    int process_height = round(height / scale);
    if (process_width < baseWidth || process_height < baseHeight) break;

    ORectangle window;

    window.w = (int)(baseWidth);
    window.h = (int)(baseHeight);

    int xStep = window.w >> 3;
    int yStep = window.h >> 3;
    xStep = yStep = 1;

    //

    unsigned char** Dst =
        (unsigned char**)malloc(process_height * sizeof(unsigned char*));
    for (int i = 0; i < process_height; i++) {
      Dst[i] = (unsigned char*)malloc(process_width * sizeof(unsigned char));
    }

    imagescale2Donechannel(Src, Dst, width, height, process_width,
                           process_height);

    IntegralImage2* integralImage =
        new IntegralImage2(process_width, process_height);
    integralImage->FromImage(Dst);

    int xEnd = process_width - window.w;
    int yEnd = process_height - window.h;

    for (int y = 0; y < yEnd; y += yStep) {
      window.y = y;
      for (int x = 0; x < xEnd; x += xStep) {
        window.x = x;
        if (ClassifierCompute2(integralImage, window, scale)) {
          ORectangle object;
          object.x = x * scale;
          object.y = y * scale;
          object.w = baseWidth * scale;
          object.h = baseHeight * scale;

          detectedObjects.push_back(object);
        }
      }
    }
  }

  return detectedObjects;
}
void HaarObjectDetector::LoadXML(string path) {
  XmlNode* xmlnode = readXml(path);

  XmlNode* sizenode = findxmlnode(xmlnode, "size");
  vector<char> buf(sizenode->val.begin(), sizenode->val.end());
  buf.push_back('\0');
  char* SizeData = buf.data();
  int size_w, size_h;
  sscanf(SizeData, "%d %d", &size_w, &size_h);
  myHaarCascadeStage.Heigh = size_h;
  myHaarCascadeStage.Width = size_w;

  XmlNode* stagesnode = findfullxmlnode(xmlnode, "stages");

  for (auto curstagenode : stagesnode->children) {
    XmlNode* curstage = findxmlnode(curstagenode, "trees");
    HaarStage haarstage;

    for (auto curtree : curstage->children) {
      StageTree stagetree;

      for (auto curnode : curtree->children) {
        XmlNode* rects = findfullxmlnode(curnode, "rects");
        HaarNode node;

        for (auto rect : rects->children) {
          HaarRectangle Rect;
          vector<char> tmpbuf(rect->val.begin(), rect->val.end());
          tmpbuf.push_back('\0');
          char* reactsdata = tmpbuf.data();
          int r_x, r_y, r_w, r_h, r_weight;
          sscanf(reactsdata, "%d %d %d %d %d.", &r_x, &r_y, &r_w, &r_h,
                 &r_weight);
          Rect.x = r_x;
          Rect.y = r_y;
          Rect.w = r_w;
          Rect.h = r_h;
          Rect.weight = r_weight;
          node.Feature.Rectangle.push_back(Rect);
        }

        XmlNode* tiltednode = findfullxmlnode(curnode, "tilted");

        if (tiltednode->val == "true") {
          node.Feature.Tilted = true;
        } else {
          node.Feature.Tilted = false;
        }

        XmlNode* thresholdnode = findfullxmlnode(curnode, "threshold");
        node.Node_Threshold = stod(thresholdnode->val);

        XmlNode* curleft_node = findfullxmlnode(curnode, "left_val");
        if (curleft_node) {
          node.Left_value = stod(curleft_node->val);
          node.LeftNodeIndex = -1;
        } else {
          node.Left_value = 0;
          node.LeftNodeIndex = 1;
        }

        XmlNode* curright_node = findfullxmlnode(curnode, "right_val");
        if (curright_node) {
          node.Right_value = stod(curright_node->val);
          node.RightNodeIndex = -1;
        } else {
          node.Right_value = 0;
          node.RightNodeIndex = 1;
        }
        stagetree.Node.push_back(node);
      }

      haarstage.Tree.push_back(stagetree);
    }  // tree level

    curstage = findxmlNxtnode(curstagenode, "stage_threshold");
    haarstage.Stage_Threshold = stod(curstage->val);

    myHaarCascadeStage.Stage.push_back(haarstage);

  }  // stage level

  baseWidth = myHaarCascadeStage.Width;
  baseHeight = myHaarCascadeStage.Heigh;
}
