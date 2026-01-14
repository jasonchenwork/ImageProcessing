#include "../include/StereoImage.hpp"

void stereoimageBM(uint8_t* dst, uint8_t* srcL, uint8_t* srcR, int width,
                   int height, int windows, int displacement) {
  int halfw = windows / 2;
  for (int y = halfw; y < (height - halfw); y++) {
    for (int x = halfw; x < (width - halfw); x++) {
      int mindisplacement = 0;
      int64_t minsum = INT_MAX;

      ;
      for (int d = 0; d < displacement; d++) {
        int64_t sum = 0;
        int tmp = 0;

        if ((x + halfw + d) >= width) continue;
        for (int wy = y - halfw; wy <= y + halfw; wy++) {
          for (int wx = x - halfw; wx <= x + halfw; wx++) {
            sum += abs(srcL[wy * width + wx] - srcR[wy * width + wx + d]);
            tmp++;
          }
        }

        if (sum < minsum) {
          minsum = sum;
          mindisplacement = d;
        }
      }

      dst[y * width + x] =
          (uint8_t)(255 * mindisplacement / (displacement - 1));
    }
  }
}

void stereoimageBMwithSGM(uint8_t* dst, uint8_t* srcL, uint8_t* srcR, int width,
                          int height, int windows, int displacement, int P1,
                          int P2) {
  vector<vector<vector<int>>> cost(
      height, vector<vector<int>>(width, vector<int>(displacement, INT_MAX)));

  int halfw = windows / 2;
  // calculate cost
  for (int y = halfw; y < (height - halfw); y++) {
    for (int x = halfw; x < (width - halfw); x++) {
      int mindisplacement = 0;
      int minsum = INT_MAX;

      ;
      for (int d = 0; d < displacement; d++) {
        int sum = 0;

        if ((x + halfw + d) >= width) continue;
        for (int wy = y - halfw; wy <= y + halfw; wy++) {
          for (int wx = x - halfw; wx <= x + halfw; wx++) {
            sum += abs(srcL[wy * width + wx] - srcR[wy * width + wx + d]);
          }
        }
        cost[y][x][d] = sum;
      }
    }
  }

  // aggregatePath
  vector<vector<vector<int>>> sumAggr(
      height, vector<vector<int>>(width, vector<int>(displacement, 0)));
  int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1},  {0, -1},
                          {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

  for (int dir = 0; dir < 8; dir++) {
    vector<vector<vector<int>>> pathAggr(
        height, vector<vector<int>>(width, vector<int>(displacement)));

    int dx = directions[dir][0];
    int dy = directions[dir][1];

    int startX = (dx > 0) ? 0 : width - 1;
    int endX = (dx > 0) ? width : -1;
    int startY = (dy > 0) ? 0 : height - 1;
    int endY = (dy > 0) ? height : -1;
    int stepX = (dx > 0) ? 1 : -1;
    int stepY = (dy > 0) ? 1 : -1;

    for (int y = startY; y != endY; y += stepY) {
      for (int x = startX; x != endX; x += stepX) {
        int prevX = x - dx;
        int prevY = y - dy;

        if (prevX < 0 || prevX >= width || prevY < 0 || prevY >= height) {
          for (int d = 0; d < displacement; ++d) {
            pathAggr[y][x][d] = cost[y][x][d];
          }
          continue;
        }

        int minPrevTotal = 1e9;
        for (int d = 0; d < displacement; ++d) {
          if (pathAggr[prevY][prevX][d] < minPrevTotal) {
            minPrevTotal = pathAggr[prevY][prevX][d];
          }
        }

        // Lr(p, d) = C(p, d) + min( Lr(p-r, d), Lr(p-r, d-1)+P1, Lr(p-r,
        // d+1)+P1, min(Lr(p-r, k))+P2 ) - min(Lr(p-r, k))
        for (int d = 0; d < displacement; ++d) {
          int Lr_prev_d = pathAggr[prevY][prevX][d];

          int Lr_prev_minus = (d > 0) ? pathAggr[prevY][prevX][d - 1] : 1e9;
          int Lr_prev_plus =
              (d < displacement - 1) ? pathAggr[prevY][prevX][d + 1] : 1e9;

          int minPathCost = min({Lr_prev_d, Lr_prev_minus + P1,
                                 Lr_prev_plus + P1, minPrevTotal + P2});

          //  Normalization
          pathAggr[y][x][d] = cost[y][x][d] + minPathCost - minPrevTotal;
        }
      }
    }

    for (int y = 0; y < height; ++y)
      for (int x = 0; x < width; ++x)
        for (int d = 0; d < displacement; ++d)
          sumAggr[y][x][d] += pathAggr[y][x][d];
  }  // end for (int dir = 0; dir < 4; dir++)

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int minCost = 1e9;
      int bestDisparity = 0;

      for (int d = 0; d < displacement; ++d) {
        if (sumAggr[y][x][d] < minCost) {
          minCost = sumAggr[y][x][d];
          bestDisparity = d;
        }
      }

      dst[y * width + x] = (bestDisparity * (255.0 / displacement));
    }
  }
}