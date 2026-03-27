#include "../include/HOGObjectDetector.hpp"

#include "../include/IntegralImage2.hpp"
#include "../include/Tinyxml.hpp"
#include "../include/Utility.hpp"
static const double THRESHOLD_EPS = 1e-5;
HOGObjectDetector::HOGObjectDetector(HOGCascadeStage Cascade) {
  factor = 1.05;

  myHOGCascadeStage = Cascade;
  baseWidth = Cascade.Width;
  baseHeight = Cascade.Heigh;
};
HOGObjectDetector::HOGObjectDetector() { factor = 1.05; };
HOGObjectDetector::~HOGObjectDetector() {

};
double HOGObjectDetector::FeatureGetSum(IntegralImage2* im, int x, int y,
                                        HOGRectangle HOGRect, double scale) {
  double sum = 0.0;

  int binIDX = HOGRect.idx % 9;
  static int blocktable[4][2] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
  int blockidx = HOGRect.idx / 9;
  int curx = round(HOGRect.x * scale);
  int cury = round(HOGRect.y * scale);
  int w = round(HOGRect.w * scale);
  int h = round(HOGRect.h * scale);

#if 0
  if (HOGRect.idx < 9) {
    blockidx = 0;
  } else if (HOGRect.idx >= 9 && HOGRect.idx < 18) {
    blockidx = 1;
  } else if (HOGRect.idx >= 18 && HOGRect.idx < 27) {
    blockidx = 2;
  } else {
    blockidx = 3;
  }
#endif
  int tx = x + curx + blocktable[blockidx][0] * w;
  int ty = y + cury + blocktable[blockidx][1] * h;
  sum = im[binIDX].GetSum(tx, ty, w, h);
  if (sum < 0.001) {
    return 0;
  }

#if 0
// L2 double inv_area = sqrt(im[9].GetSum2(tx, ty, HOGRect.w * 2, HOGRect.h *
// 2) + 0.001f);
#else
  double inv_area = (im[9].GetSum(x + curx, y + cury, w * 2, h * 2) + 0.001);
  // double inv_area = (im[9].GetSum(tx, ty, HOGRect.w, HOGRect.h) + 0.001f);
  // inv_area = (im[9].GetSum(tx, ty, w, h) + 0.001f);
#endif
  return sum / (inv_area + 0.001);
};
bool HOGObjectDetector::ClassifierCompute(IntegralImage2* im,
                                          ORectangle rectangle, double scale) {
  int x = rectangle.x;
  int y = rectangle.y;
  // int w = rectangle.w;
  // int h = rectangle.h;

  for (int i = 0; i < (int)myHOGCascadeStage.Stage.size(); i++) {
    double value = 0.0;
    for (int j = 0; j < (int)myHOGCascadeStage.Stage[i].Tree.size(); j++) {
      int current = 0;

      for (int z = 0; z < (int)myHOGCascadeStage.Stage[i].Tree[j].Node.size();
           z++) {
        double sum = FeatureGetSum(
            im, x, y,
            features[myHOGCascadeStage.Stage[i].Tree[j].Node[z].FeatureIDX],
            scale);

        if (sum < myHOGCascadeStage.Stage[i].Tree[j].Node[z].Node_Threshold) {
          value += myHOGCascadeStage.Stage[i].Tree[j].Node[z].Left_value;
          current = myHOGCascadeStage.Stage[i].Tree[j].Node[z].LeftNodeIndex;
        } else {
          value += myHOGCascadeStage.Stage[i].Tree[j].Node[z].Right_value;
          current = myHOGCascadeStage.Stage[i].Tree[j].Node[z].RightNodeIndex;
        }

        if (current == -1) {
          break;
        }

      }  // end z node

    }  // end j tree

    if (value < (myHOGCascadeStage.Stage[i].Stage_Threshold - THRESHOLD_EPS)) {
      // If it is, the stage has rejected the current
      // image and it doesn't contains our object.
      myHOGCascadeStage.Stage[i].count++;

      // printf("Rejected at Stage %d, value: %f, thresh: %f\n", i, value,
      // myHOGCascadeStage.Stage[i].Stage_Threshold);

      return false;
    } else {
      // Current stage has accepted
    }

  }  // end i stage

  return true;  // The image has been detected.
}
std::vector<ORectangle> HOGObjectDetector::ProcessMultiScaleWindow(
    unsigned char** Src, int width, int height) {
  detectedObjects.clear();
  for (int i = 0; i < (int)myHOGCascadeStage.Stage.size(); i++) {
    myHOGCascadeStage.Stage[i].count = 0;
  }
  int process_width = width;
  int process_height = height;

  ORectangle window;

  unsigned char** Dst = new unsigned char*[process_height];
  for (int i = 0; i < process_height; i++) {
    Dst[i] = new unsigned char[process_width];
  }
  double** Dst_double = new double*[process_height];
  for (int i = 0; i < process_height; i++) {
    Dst_double[i] = new double[process_width];
  }

  imagescale2Donechannel(Src, Dst, width, height, process_width,
                         process_height);

  TypeConver(Dst, Dst_double, process_width, process_height);

  IntegralImage2* integralImage;
  integralImage = new IntegralImage2[10];

  for (int i = 0; i < 10; i++) {
    integralImage[i].INIT(process_width, process_height);
  }
  double filter[3] = {-1, 0, 1};

  double*** NineBins = new double**[10];
  for (int i = 0; i < 10; i++) {
    NineBins[i] = new double*[process_height];
  }
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < process_height; j++) {
      NineBins[i][j] = new double[process_width];
      for (int z = 0; z < process_width; z++) {
        NineBins[i][j][z] = 0;
      }
    }
  }

  // do  Gx Gy
  double** Gx =
      conv1D(Dst_double, process_width, process_height, filter, 3, conv1D_row);
  double** Gy =
      conv1D(Dst_double, process_width, process_height, filter, 3, conv1D_col);

  for (int y = 0; y < process_height; y++) {
    for (int x = 0; x < process_width; x++) {
#if 1
      double mag = sqrt(Gx[y][x] * Gx[y][x] + Gy[y][x] * Gy[y][x]);

      double angle = atan2(Gy[y][x], Gx[y][x]);
      // if (angle < 0) angle += PI;
      angle = angle * (9.0 / PI) - 0.5f;
      int bidx = floor(angle);
      /// angle -= bidx;
      // if (bidx >= 9) bidx = 8;
      // if (bidx < 0) bidx = 0;

      if (bidx < 0)
        bidx += 9;
      else if (bidx >= 9)
        bidx -= 9;
      NineBins[bidx][y][x] = mag;
#else
      double mag = sqrt(Gx[y][x] * Gx[y][x] + Gy[y][x] * Gy[y][x]);

      double angle = atan2(Gy[y][x], Gx[y][x]) * 180.0 / PI;
      if (angle < 0) angle += 180.0;

      if (angle >= 180.0) angle -= 180.0;

      int bin0 = (int)(angle / 20);
      int bin1 = bin0 + 1;

      double bin1W = (angle - bin0 * 20) / 20.0;
      double bin0W = 1.0 - bin1W;

      NineBins[bin0][y][x] += bin0W * mag;
      if (bin1 < 9) {
        NineBins[bin1][y][x] += bin1W * mag;
      } else {
        NineBins[0][y][x] += bin1W * mag;
      }
#endif
      NineBins[9][y][x] = mag;
    }
  }

  for (int i = 0; i < 10; i++) {
    integralImage[i].FromImage(NineBins[i]);
  }
  if (steps.size() < 1) update(width, height);

  if (steps.size() == 0) {
    steps.push_back(1);
  }

  for (int i = 0; i < (int)steps.size(); i++) {
    double scaling = steps[i];

    window.w = (int)(baseWidth * scaling);
    window.h = (int)(baseHeight * scaling);

    int xStep = 1;  // window.w >> 3;
    int yStep = 1;  // window.h >> 3;

    int xEnd = process_width - window.w;
    int yEnd = process_height - window.h;

    for (int y = 0; y < yEnd; y += yStep) {
      window.y = y;
      for (int x = 0; x < xEnd; x += xStep) {
        window.x = x;
        if (ClassifierCompute(integralImage, window, scaling)) {
          detectedObjects.push_back(window);
        }
      }
    }
    // release buf
  }
#if 1
  for (int i = 0; i < process_height; i++) {
    delete[] Dst_double[i];
  }
  delete[] Dst_double;
  Dst_double = nullptr;

  for (int i = 0; i < process_height; i++) {
    delete[] Dst[i];
  }
  delete[] Dst;
  Dst = nullptr;

  //
  for (int i = 0; i < process_height; i++) {
    delete[] Gx[i];
  }
  delete[] Gx;
  Gx = nullptr;

  for (int i = 0; i < process_height; i++) {
    delete[] Gy[i];
  }
  delete[] Gy;
  Gy = nullptr;

  delete[] integralImage;
  integralImage = nullptr;

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < process_height; j++) {
      delete[] NineBins[i][j];
    }
  }

  for (int i = 0; i < 10; i++) {
    delete[] NineBins[i];
  }

  delete[] NineBins;
  NineBins = nullptr;
#endif

  for (int i = 0; i < (int)myHOGCascadeStage.Stage.size(); i++) {
    cout << "i: " << i << " , " << myHOGCascadeStage.Stage[i].count << endl;
  }
  return detectedObjects;
}
void HOGObjectDetector::update(int width, int height) {
  steps.clear();
  double start = 1.0;
  double stop = dmin(width / (double)baseWidth, height / (double)baseHeight);
  double step = factor;
  for (double f = start; f < stop; f *= step) steps.push_back(f);
};
std::vector<ORectangle> HOGObjectDetector::ProcessMultiScaleImage(
    unsigned char** Src, int width, int height) {
  for (int i = 0; i < (int)myHOGCascadeStage.Stage.size(); i++) {
    myHOGCascadeStage.Stage[i].count = 0;
  }
  detectedObjects.clear();

  double scale;
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

    unsigned char** Dst = new unsigned char*[process_height];
    for (int i = 0; i < process_height; i++) {
      Dst[i] = new unsigned char[process_width];
    }
    double** Dst_double = new double*[process_height];
    for (int i = 0; i < process_height; i++) {
      Dst_double[i] = new double[process_width];
    }

    imagescale2Donechannel(Src, Dst, width, height, process_width,
                           process_height);

    TypeConver(Dst, Dst_double, process_width, process_height);

    IntegralImage2* integralImage;
    integralImage = new IntegralImage2[10];

    for (int i = 0; i < 10; i++) {
      integralImage[i].INIT(process_width, process_height);
    }
    double filter[3] = {-1, 0, 1};

    double*** NineBins = new double**[10];
    for (int i = 0; i < 10; i++) {
      NineBins[i] = new double*[process_height];
    }
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < process_height; j++) {
        NineBins[i][j] = new double[process_width];
        for (int z = 0; z < process_width; z++) {
          NineBins[i][j][z] = 0;
        }
      }
    }

    // do  Gx Gy
    double** Gx = conv1D(Dst_double, process_width, process_height, filter, 3,
                         conv1D_row);
    double** Gy = conv1D(Dst_double, process_width, process_height, filter, 3,
                         conv1D_col);

    for (int y = 0; y < process_height; y++) {
      for (int x = 0; x < process_width; x++) {
#if 1
        double mag = sqrt(Gx[y][x] * Gx[y][x] + Gy[y][x] * Gy[y][x]);

        double angle = atan2(Gy[y][x], Gx[y][x]);
        // if (angle < 0) angle += PI;  // 先轉為 0 ~ PI
        angle = angle * (9.0 / PI) - 0.5;  // - 0.5f;
        int bidx = floor(angle);

        if (bidx < 0)
          bidx += 9;
        else if (bidx >= 9)
          bidx -= 9;
        NineBins[bidx][y][x] = mag;
#else
        double mag = sqrt(Gx[y][x] * Gx[y][x] + Gy[y][x] * Gy[y][x]);

        double angle = atan2(Gy[y][x], Gx[y][x]) * 180.0 / PI;
        if (angle < 0) angle += 180.0;

        if (angle >= 180.0) angle -= 180.0;

        int bin0 = (int)(angle / 20);
        int bin1 = bin0 + 1;

        double bin1W = (angle - bin0 * 20) / 20.0;
        double bin0W = 1.0 - bin1W;

        NineBins[bin0][y][x] += bin0W * mag;
        if (bin1 < 9) {
          NineBins[bin1][y][x] += bin1W * mag;
        } else {
          NineBins[0][y][x] += bin1W * mag;
        }
#endif
        NineBins[9][y][x] = mag;
      }
    }

    for (int i = 0; i < 10; i++) {
      integralImage[i].FromImage(NineBins[i]);
    }
    if (steps.size() < 1) update(width, height);

    if (steps.size() == 0) {
      steps.push_back(1);
    }

    int xEnd = process_width - window.w;
    int yEnd = process_height - window.h;

    for (int y = 0; y < yEnd; y += yStep) {
      window.y = y;
      for (int x = 0; x < xEnd; x += xStep) {
        window.x = x;
        if (ClassifierCompute(integralImage, window, 1)) {
          ORectangle object;
          object.x = x * scale;
          object.y = y * scale;
          object.w = baseWidth * scale;
          object.h = baseHeight * scale;

          detectedObjects.push_back(object);
        }
      }
    }
    // release buf
#if 1
    for (int i = 0; i < process_height; i++) {
      delete[] Dst_double[i];
    }
    delete[] Dst_double;
    Dst_double = nullptr;

    for (int i = 0; i < process_height; i++) {
      delete[] Dst[i];
    }
    delete[] Dst;
    Dst = nullptr;

    //
    for (int i = 0; i < process_height; i++) {
      delete[] Gx[i];
    }
    delete[] Gx;
    Gx = nullptr;

    for (int i = 0; i < process_height; i++) {
      delete[] Gy[i];
    }
    delete[] Gy;
    Gy = nullptr;

    delete[] integralImage;
    integralImage = nullptr;

    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < process_height; j++) {
        delete[] NineBins[i][j];
      }
    }

    for (int i = 0; i < 10; i++) {
      delete[] NineBins[i];
    }

    delete[] NineBins;
    NineBins = nullptr;
#endif
  }
  for (int i = 0; i < (int)myHOGCascadeStage.Stage.size(); i++) {
    cout << "i: " << i << " , " << myHOGCascadeStage.Stage[i].count << endl;
  }
  return detectedObjects;
}
void HOGObjectDetector::LoadXML(string path) {
  XmlNode* xmlnode = readXml(path);

  XmlNode* cascadenode = findfullxmlnode(xmlnode, "cascade");

  XmlNode* heightNode = findfullxmlnode(cascadenode, "height");
  baseHeight = stod(heightNode->val);
  myHOGCascadeStage.Heigh = baseHeight;

  XmlNode* widthNode = findfullxmlnode(cascadenode, "width");
  baseWidth = stod(widthNode->val);
  myHOGCascadeStage.Width = baseWidth;

  XmlNode* stagesnode = findfullxmlnode(cascadenode, "stages");

  for (auto curstage : stagesnode->children) {
    HOGStage curHOGstage;

    XmlNode* stageThreshold = findfullxmlnode(curstage, "stageThreshold");
    curHOGstage.Stage_Threshold = stod(stageThreshold->val);

    XmlNode* weakClassifiers = findfullxmlnode(curstage, "weakClassifiers");

    for (auto weakClassifier : weakClassifiers->children) {
      HOGStageTree curHOGStageTree;
      HOGNode curNode;

      XmlNode* internalNodes = findfullxmlnode(weakClassifier, "internalNodes");
      vector<char> tmpbuf(internalNodes->val.begin(), internalNodes->val.end());
      tmpbuf.push_back('\0');
      char* reactsdata = tmpbuf.data();
      int leftindex, rightindex, featureid;
      double thr;
      sscanf(reactsdata, "%d %d %d %lf", &leftindex, &rightindex, &featureid,
             &thr);

      XmlNode* leafValues = findfullxmlnode(weakClassifier, "leafValues");
      // cout << "leafValues: " << leafValues->val << endl;
      vector<char> tmpbuf2(leafValues->val.begin(), leafValues->val.end());
      tmpbuf2.push_back('\0');
      char* reactsdata2 = tmpbuf2.data();
      double leftvalue, rightvalue;
      sscanf(reactsdata2, "%lf %lf", &leftvalue, &rightvalue);

      curNode.LeftNodeIndex = leftindex;
      curNode.RightNodeIndex = rightindex;
      curNode.Node_Threshold = thr;
      curNode.FeatureIDX = featureid;
      curNode.Left_value = leftvalue;
      curNode.Right_value = rightvalue;

      curHOGStageTree.Node.push_back(curNode);
      curHOGstage.Tree.push_back(curHOGStageTree);
    }
    myHOGCascadeStage.Stage.push_back(curHOGstage);
  }

  XmlNode* featuresnode = findfullxmlnode(cascadenode, "features");

  for (auto featurenode : featuresnode->children) {
    HOGRectangle Rect;
    XmlNode* tmp = findfullxmlnode(featurenode, "rect");
    vector<char> tmpbuf(tmp->val.begin(), tmp->val.end());
    tmpbuf.push_back('\0');
    char* reactsdata = tmpbuf.data();
    int r_x, r_y, r_w, r_h, r_idx;
    sscanf(reactsdata, "%d %d %d %d %d", &r_x, &r_y, &r_w, &r_h, &r_idx);
    Rect.x = r_x;
    Rect.y = r_y;
    Rect.w = r_w;
    Rect.h = r_h;
    Rect.idx = r_idx;
    features.push_back(Rect);
  }
}