#include "../include/CCL.hpp"

void dfs_ccl(uint8_t* dst, uint8_t* gray, int x, int y, int id, int width,
             int height) {
  int dirs[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0},
                    {-1, 0},  {1, -1}, {1, 0},  {1, 1}};

  for (int d = 0; d < 8; d++) {
    int newx = x + dirs[d][0];
    int newy = y + dirs[d][1];
    if (x < 0 || x >= width || y < 0 || y >= height ||
        gray[newy * width + newx] != 255 || dst[newy * width + newx] != 0)
      continue;

    dst[newy * width + newx] = id;
    dfs_ccl(dst, gray, newx, newy, id, width, height);
  }
}

void CCL_dfs(uint8_t* dst, uint8_t* src, int width, int height) {
  uint8_t* gray = new uint8_t[width * height];
  uint8_t* id = new uint8_t[width * height];
  // uint8_t* groupID = new uint8_t[10];  // max 10 group
  colorimage2grayimage(src, gray, width, height);
  uint8_t curID = 1;

  for (int i = 0; i < width * height; i++) {
    id[i] = 0;
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (gray[y * width + x] == 255 && id[y * width + x] == 0) {
        id[y * width + x] = curID;
        dfs_ccl(id, gray, x, y, curID, width, height);
        curID++;
      }

      if (curID > 10) {
        cout << "curID >10" << endl;
        return;  // max 10 group
      }
    }
  }
  // put color to dst
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (id[y * width + x] != 0) {
        dst[3 * (y * width + x) + 2] = COLORBAGS[id[y * width + x] - 1][0];
        dst[3 * (y * width + x) + 1] = COLORBAGS[id[y * width + x] - 1][1];
        dst[3 * (y * width + x) + 0] = COLORBAGS[id[y * width + x] - 1][2];
      }
    }
  }
}