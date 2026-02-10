#include "../include/CCL.hpp"

class unionfind {
 public:
  // unordered_map<int, int> id;
  vector<int> id;
  unionfind(int maxsize) {
    id.resize(maxsize);
    std::iota(id.begin(), id.end(), 0);
  }
  int find(int x) {
    if (id[x] != x) {
      id[x] = find(id[x]);
    }
    return id[x];
  }
  void _union(int a, int b) {
    int roota = find(a);
    int rootb = find(b);

    if (roota < rootb) {
      id[rootb] = roota;
    } else {
      id[roota] = rootb;
    }
  }
};
void dfs_ccl(uint8_t* dst, uint8_t* gray, int x, int y, int id, int width,
             int height) {
  int dirs[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0},
                    {-1, 0},  {1, -1}, {1, 0},  {1, 1}};

  for (int d = 0; d < 8; d++) {
    int newx = x + dirs[d][0];
    int newy = y + dirs[d][1];
    if (newx < 0 || newx >= width || newy < 0 || newy >= height ||
        gray[newy * width + newx] != 255 || dst[newy * width + newx] != 0)
      continue;

    dst[newy * width + newx] = id;
    dfs_ccl(dst, gray, newx, newy, id, width, height);
  }
}

void CCL_dfs(uint8_t* dst, uint8_t* src, int width, int height) {
  uint8_t* gray = new uint8_t[width * height];
  uint8_t* id = new uint8_t[width * height];
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
#if 0
      if (curID > 10) {
        cout << "curID >10" << endl;
        return;  // max 10 group  // Since the palette only supports 10 colors
      }
#endif
    }
  }
  // put color to dst
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (id[y * width + x] != 0) {
        dst[3 * (y * width + x) + 2] = COLORBAGS[id[y * width + x] % 10 - 1][0];
        dst[3 * (y * width + x) + 1] = COLORBAGS[id[y * width + x] % 10 - 1][1];
        dst[3 * (y * width + x) + 0] = COLORBAGS[id[y * width + x] % 10 - 1][2];
      }
    }
  }
  delete[] gray;
  delete[] id;
}

void CCL_2pass(uint8_t* dst, uint8_t* src, int width, int height) {
  int maxid = 100;  // Since the palette only supports 10 colors
  uint8_t* gray = new uint8_t[width * height];
  int* id = new int[width * height];
  unionfind* uf = new unionfind(maxid);

  colorimage2grayimage(src, gray, width, height);
  int curID = 1;

  int topmask[4][2] = {{-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

  for (int i = 0; i < width * height; i++) {
    id[i] = 0;
  }
  // first pass
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (gray[y * width + x] == 0) continue;
      bool isEmpty = true;
      int minID = INT_MAX;
      for (int i = 0; i < 4; i++) {
        int newx = x + topmask[i][0];
        int newy = y + topmask[i][1];
        if (newx < 0 || newx >= width || newy < 0 || newy >= height ||
            id[newy * width + newx] == 0)
          continue;

        isEmpty = false;
        minID = min(minID, (int)id[newy * width + newx]);
      }
      if (isEmpty) {
        uf->id[curID] = curID;
        id[y * width + x] = curID;
        curID++;
#if 1
        if (curID >= maxid) {
          cout << "curID >=" << maxid << endl;
          return;
        }
#endif
      } else {
        id[y * width + x] = minID;
        for (int d = 0; d < 4; d++) {
          int xx = x + topmask[d][0];
          int yy = y + topmask[d][1];
          if (xx < 0 || xx >= width || yy < 0 || yy >= height ||
              id[yy * width + xx] == 0)
            continue;

          uf->_union(id[y * width + x], id[yy * width + xx]);
        }
      }
    }
  }
  // second pass
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (id[y * width + x] != 0) {
        id[y * width + x] = uf->find(id[y * width + x]) % 10;
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
  delete uf;
  delete[] gray;
  delete[] id;
}