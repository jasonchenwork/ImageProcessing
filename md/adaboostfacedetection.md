
# ADABOOST Face detection with Haar feature

This algorithm, published in 2001, is based on the concept of combining many weak classifiers to form a strong classifier. Each stage is a strong classifier composed of multiple weak classifiers, where each weak classifier is implemented using a decision tree.

At each stage, the algorithm determines whether a region is a non-face or a potential face. If it is classified as a potential face, it proceeds to the next stage for further detection. Only if the region passes all stages is it finally classified as a face; otherwise, it is considered a non-face.

Each weak classifier uses Haar features for computation. Haar features are used because they can be efficiently calculated using an integral image, which significantly improves overall performance.

This project is mainly based on the OpenCV source code.

There are two scanning methods: Scaling windows: fix image size, change window size Scaling image: change image size, fix window size

![](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/scalingwindowansscalingimage.bmp)
  
test image is lena(512x512)  
  
|  | Scale Windows|Scale Image |
| - | - | - |
| Speed(ms)|3501ms*| 16731ms|
| accuracy| Lower| Better|

# Result  

![](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/testadaboost.bmp)
  
# Implement note  

1. haar's weights need to normalize
   $$w'_i​= \frac{w_i}{CascadeW×CascadeH}$$

The method for scaling windows is integrated into the update feature function. This is the method in scaling image:

```CPP
   double HaarObjectDetector::FeatureGetSum2(IntegralImage2* im, int x, int y,
                                          HaarFeature HF, double scale) {
  double sum = 0.0;
  if (!HF.Tilted) {
    for (int i = 0; i < HF.Rectangle.size(); i++) {
      sum += im->GetSum(x + (HF.Rectangle[i].x), y + HF.Rectangle[i].y,
                        HF.Rectangle[i].w, HF.Rectangle[i].h) *
             (HF.Rectangle[i].weight * (1.0 / (baseWidth * baseHeight)));
    }
  }

  return sum;
};

```

1. brightness normalize

$$f_{\text{norm}} = \frac{\sum_i w_i S(R_i) - \text{mean}_{\text{window}} \cdot \sum_i w_i |R_i|}{\text{std}_{\text{window}}}$$

$S(R_i)$ → rectangle sum from integral image
$w_i$ → rectangle weight (+1 / -1)
$|R_i|$ → rectangle area

mean_window / std_window → window pixel mean & std
Due to  
$ \sum_i w_i |R_i| =0 $  
Let's  
$$f_{\text{norm}} = \frac{\sum_i w_i S(R_i)}{\text{std}_{\text{window}}}$$

Therefore, there is no need to follow the calculation based on the current feature rectangle, and it can be further optimized afterwards.  
Let's  
$$
\frac{GetFeature()}{\text{std}_{\text{window}}} < {\text{node}_{\text{threshold}}}
$$

to

$$
\text{GetFeature()} < {\text{node}_{\text{threshold}}} * {\text{std}_{\text{window}}}
$$

factor = $ {\text{std}_{\text{window}}} $

```CPP
bool HaarObjectDetector::HaarClassifierCompute2(IntegralImage2* im,
                                                ORectangle rectangle,
                                                double scale) {
  int x = rectangle.x;
  int y = rectangle.y;
  int w = rectangle.w;
  int h = rectangle.h;
  double invArea = 1.0 / (myHaarCascadeStage.Width * myHaarCascadeStage.Heigh);

  double mean = im->GetSum(x, y, w, h) * invArea;

  double factor = im->GetSum2(x, y, w, h) * invArea - (mean * mean);

  factor = (factor >= 0.0) ? sqrt(factor) : 1.0;

    ...
    ...
    double sum = FeatureGetSum2(
            im, x, y, myHaarCascadeStage.Stage[i].Tree[j].Node[z].Feature,
            scale);

        if (sum < myHaarCascadeStage.Stage[i].Tree[j].Node[z].Node_Threshold *
                      factor) 
          {
            ...
          }
    ...
 
```

# Reference  

[1] Rapid Object Detection using a Boosted Cascade of Simple Features” “Robust Real-Time Face Detection
