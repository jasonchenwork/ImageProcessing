#ifndef SIFT_HPP
#define SIFT_HPP
#include <bits/stdc++.h>

#include "../include/SimpleImage.hpp"
using namespace std;
/*
 this code is based on  https :  // github.com/leofansq/SIFT
 ref : https :  // github.com/leofansq/SIFT
 */
#define SIFT_IMG_BORDER 20
#define SQRT2 1.414213562373095f
#define EPSILON_F 1.19209290E-07F
// #define PI_o 3.141592653589793f
#define _2PI 6.283185307179586f
#define PI_4 0.785398163397448f
#define PI_3_4 2.356194490192345f
// SIFT : 128
#define DEGREE_OF_DESCRIPTORS (128)
struct SiftKeypoint {
  int octave;
  int layer;
  float yi;  // layer'y
  float xi;  // layer'x
  float y;   // layer'y
  float x;   // layer'x
  float ori;
  float mag;
  float layer_scale;
  float rlayer;
  float scale;
  float descriptors[DEGREE_OF_DESCRIPTORS];
};
struct MatchPair {
  int r1;
  int c1;
  int r2;
  int c2;
};
void draw_keypoints_image(SimpleImage *image, list<SiftKeypoint> kpt_list);
int match_keypoints(list<SiftKeypoint> &kpt_list1,
                    list<SiftKeypoint> &kpt_list2, list<MatchPair> &match_list);
int draw_match_lines_to_image(SimpleImage *Outimage,
                              std::list<MatchPair> &match_list);
void sift(const uint8_t *imageSource, uint16_t width, uint16_t height,
          list<SiftKeypoint> &kp);

#define MAT_DOT_VEC_3X3(p, m, v)                             \
                                                             \
  {                                                          \
    p[0] = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2]; \
    p[1] = m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2]; \
    p[2] = m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2]; \
  }

#define DETERMINANT_3X3(d, m)                               \
                                                            \
  {                                                         \
    d = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);  \
    d -= m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]); \
    d += m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]); \
  }
#define SCALE_ADJOINT_3X3(a, s, m)                           \
                                                             \
  {                                                          \
    a[0][0] = (s) * (m[1][1] * m[2][2] - m[1][2] * m[2][1]); \
    a[1][0] = (s) * (m[1][2] * m[2][0] - m[1][0] * m[2][2]); \
    a[2][0] = (s) * (m[1][0] * m[2][1] - m[1][1] * m[2][0]); \
                                                             \
    a[0][1] = (s) * (m[0][2] * m[2][1] - m[0][1] * m[2][2]); \
    a[1][1] = (s) * (m[0][0] * m[2][2] - m[0][2] * m[2][0]); \
    a[2][1] = (s) * (m[0][1] * m[2][0] - m[0][0] * m[2][1]); \
                                                             \
    a[0][2] = (s) * (m[0][1] * m[1][2] - m[0][2] * m[1][1]); \
    a[1][2] = (s) * (m[0][2] * m[1][0] - m[0][0] * m[1][2]); \
    a[2][2] = (s) * (m[0][0] * m[1][1] - m[0][1] * m[1][0]); \
  }
#endif