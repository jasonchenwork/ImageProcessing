
# ADABOOST Face detection with Haar feature

This algorithm, published in 2001, is based on the concept of combining many weak classifiers to form a strong classifier. Each stage is a strong classifier composed of multiple weak classifiers, where each weak classifier is implemented using a decision tree.

At each stage, the algorithm determines whether a region is a non-face or a potential face. If it is classified as a potential face, it proceeds to the next stage for further detection. Only if the region passes all stages is it finally classified as a face; otherwise, it is considered a non-face.

Each weak classifier uses Haar features for computation. Haar features are used because they can be efficiently calculated using an integral image, which significantly improves overall performance.

This project is mainly based on the OpenCV source code.

Would you like me to make it sound more academic (e.g., for a paper/report) or more natural (for a presentation or project description)?

# Result  

![](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/testadaboost.bmp)
  
# Implement note  

TBD

# How to improve speed  

TBD

# Reference  

[1] Rapid Object Detection using a Boosted Cascade of Simple Features” “Robust Real-Time Face Detection
