#include "../include/SimpleMat.hpp"

SimpleMat::SimpleMat() {

};
template <typename T>
SimpleMat::SimpleMat(uint32_t w, uint32_t h) {
  width = w;
  height = h;
  data = new vector<vector<T>>(w, vector<T>(h));
};
SimpleMat::~SimpleMat() {};
