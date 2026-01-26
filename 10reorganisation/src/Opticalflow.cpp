#include "../include/Opticalflow.hpp"

#include "../include/Utility.hpp"
vector<vector<vector<double>>> GetPartialDerivative(uint8_t* srcFirst,
                                                    uint8_t* srcSecond,
                                                    int width, int height) {
  vector<vector<vector<double>>> E(
      height, vector<vector<double>>(width, vector<double>(3, 0)));

  for (int y = 0; y < height - 1; y++) {
    for (int x = 0; x < width - 1; x++) {
      E[y][x][0] =
          (srcFirst[(y + 1) * width + (x + 1)] -
           srcFirst[(y + 1) * width + (x)] + srcFirst[(y)*width + (x + 1)] -
           srcFirst[(y)*width + (x)] + srcSecond[(y + 1) * width + (x + 1)] -
           srcSecond[(y + 1) * width + (x)] + srcSecond[(y)*width + (x + 1)] -
           srcSecond[(y)*width + (x)]) /
          4.0;
      E[y][x][1] =
          (-srcFirst[(y)*width + x] + srcFirst[(y + 1) * width + x] -
           srcFirst[(y)*width + x + 1] + srcFirst[(y + 1) * width + x + 1] -
           srcSecond[(y)*width + x] + srcSecond[(y + 1) * width + x] -
           srcSecond[(y)*width + x + 1] + srcSecond[(y + 1) * width + x + 1]) /
          4.0;
      E[y][x][2] =
          (srcSecond[(y)*width + x] - srcFirst[(y)*width + x] +
           srcSecond[(y)*width + x + 1] - srcFirst[(y)*width + x + 1] +
           srcSecond[(y + 1) * width + x] - srcFirst[(y + 1) * width + x] +
           srcSecond[(y + 1) * width + x + 1] -
           srcFirst[(y + 1) * width + x + 1]) /
          4.0;
    }
  }
  return E;
}
vector<vector<double>> matrix_transform(vector<vector<double>>& A) {
  vector<vector<double>> At(A[0].size(), vector<double>(A.size(), 0));

  for (int y = 0; y < A.size(); y++) {
    for (int x = 0; x < A[0].size(); x++) {
      At[x][y] = A[y][x];
    }
  }

  return At;
}
vector<vector<double>> matrix_multiply(vector<vector<double>>& M1,
                                       vector<vector<double>>& M2) {
  vector<vector<double>> out(M1.size(), vector<double>(M2[0].size(), 0));

  for (int y = 0; y < out.size(); y++) {
    for (int x = 0; x < out[0].size(); x++) {
      double tmp = 0;
      for (int z = 0; z < M1[0].size(); z++) {
        tmp += M1[y][z] * M2[z][x];
      }
      out[y][x] = tmp;
    }
  }

  return out;
}
vector<vector<double>> matrix_2dinverse(vector<vector<double>>& A) {
  vector<vector<double>> out(2, vector<double>(2, 0));
  double a = A[0][0];
  double b = A[0][1];
  double c = A[1][0];
  double d = A[1][1];

  double T = 1.0 / (a * d - b * c);

  out[0][0] = T * d;
  out[0][1] = T * -b;
  out[1][0] = T * -c;
  out[1][1] = T * a;

  return out;
}
void opticalflowLK(uint8_t* dst, uint8_t* srcFirst, uint8_t* srcSecond,
                   int width, int height, int wsize) {
  stackblur(srcFirst, width, height, 5);

  stackblur(srcSecond, width, height, 5);

  vector<vector<vector<double>>> E =
      GetPartialDerivative(srcFirst, srcSecond, width, height);

  vector<vector<vector<double>>> motion(
      height, vector<vector<double>>(width, vector<double>(2, 0)));

  vector<vector<double>> A(wsize * wsize, vector<double>(2, 0));
  vector<vector<double>> B(wsize * wsize, vector<double>(1, 0));
  vector<vector<double>> At(wsize * wsize, vector<double>(2, 0));

  for (int y = wsize / 2; y < height - wsize / 2; y++) {
    for (int x = wsize / 2; x < width - wsize / 2; x++) {
      int widx = 0;
      for (int y1 = -wsize / 2; y1 < wsize / 2; y1++) {
        for (int x1 = -wsize / 2; x1 < wsize / 2; x1++) {
          A[widx][0] = E[y + y1][x + x1][0];
          A[widx][1] = E[y + y1][x + x1][1];
          B[widx][0] = -1 * E[y + y1][x + x1][2];
          widx++;
        }
      }

      vector<vector<double>> At = matrix_transform(A);
      vector<vector<double>> tmp = matrix_multiply(At, A);
      tmp = matrix_2dinverse(tmp);

      vector<vector<double>> tmp2 = matrix_multiply(At, B);
      vector<vector<double>> out = matrix_multiply(tmp, tmp2);

      motion[y][x][0] = out[0][0];
      motion[y][x][1] = out[1][0];
    }
  }
  ShowOpticalFlowField(dst, motion, width, height);
}
void opticalflowHS(uint8_t* dst, uint8_t* srcFirst, uint8_t* srcSecond,
                   int width, int height, int iterations, double alpha) {
  stackblur(srcFirst, width, height, 5);

  stackblur(srcSecond, width, height, 5);

  vector<vector<vector<double>>> E =
      GetPartialDerivative(srcFirst, srcSecond, width, height);

  vector<vector<vector<double>>> motion(
      height, vector<vector<double>>(width, vector<double>(2, 0)));

  double Vxbar, Vybar, temp;

  for (int i = 0; i < iterations; i++) {
    for (int y = 1; y < height - 1; y++) {
      for (int x = 1; x < width - 1; x++) {
        Vxbar = (motion[y - 1][x][0] + motion[y][x + 1][0] +
                 motion[y + 1][x][0] + motion[y][x - 1][0]) /
                    6.0 +
                (motion[y - 1][x + 1][0] + motion[y + 1][x + 1][0] +
                 motion[y + 1][x + 1][0] + motion[y + 1][x - 1][0]) /
                    12.0;
        Vybar = (motion[y - 1][x][1] + motion[y][x + 1][1] +
                 motion[y + 1][x][1] + motion[y][x - 1][1]) /
                    6.0 +
                (motion[y - 1][x + 1][1] + motion[y + 1][x + 1][1] +
                 motion[y + 1][x + 1][1] + motion[y + 1][x - 1][1]) /
                    12.0;
        temp =
            (E[y][x][0] * Vxbar + E[y][x][1] * Vybar + E[y][x][2]) /
            (alpha * alpha + E[y][x][0] * E[y][x][0] + E[y][x][1] * E[y][x][1]);
        motion[y][x][0] = Vxbar - E[y][x][0] * temp;
        motion[y][x][1] = Vybar - E[y][x][1] * temp;
      }
    }
  }

  ShowOpticalFlowField(dst, motion, width, height);
}
void ShowOpticalFlowField(uint8_t* dst, vector<vector<vector<double>>>& motion,
                          int width, int height) {
  double* HSV = new double[width * height * 3];
  double max_mag = 1;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      double mag = sqrt(motion[y][x][0] * motion[y][x][0] +
                        motion[y][x][1] * motion[y][x][1]);

      if (mag >= max_mag) {
        max_mag = mag;
      }

      double angle = 0;

      angle = atan2(-motion[y][x][1], motion[y][x][0]) * 180.0 / PI;

      if (angle < 0) {
        angle += 360;
      }

      double H = angle;
      double S = 0;
      double V = mag;

      HSV[(y * width + x) * 3 + 0] = V;
      HSV[(y * width + x) * 3 + 1] = S;
      HSV[(y * width + x) * 3 + 2] = H;
    }
  }
  // cout << "max_mag: " << max_mag << endl;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      double mag = HSV[(y * width + x) * 3 + 0];

      double S = mag / max_mag;

      HSV[(y * width + x) * 3 + 0] = 1;
      HSV[(y * width + x) * 3 + 1] = S;  //          (mag / max_mag) *
      // (mag / max_mag);  //* (mag /
    }
  }

  hsv2rgb(dst, HSV, height, width);
  delete[] HSV;
}