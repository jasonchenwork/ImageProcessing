
# Tiny XML

This is a lightweight C++ XML parser implementation that reads XML files and converts them into a tree structure (DOM tree), allowing for easy traversal, searching, and printing within a program

# How to use


```C++

#include "../include/Tinyxml.hpp"

void testtinyxml() {
   XmlNode* root = readXml("data/your.xml");
   printXml(root, 0);
   deleteXml(root);
  }

```

# Cascade Filter XML

I used to implenment with DFS skill.  

![haarXMLstruct](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/haarXMLstruct.bmp)  

Cascade Filter 的檔案結構為N Way tree
root 擁有 許多 stages,每個stage擁有許多顆樹,每個樹有兩個decision nodes
每個stage有一個stage thr 來判斷此input是否能通過至下個stage
每個decision node有2~3 特徵資訊(點座標與長寬權重值), node thr, left val, right val
當計算出的特徵大於等於node thr 可以獲得 right val 或 小於則獲得 left val
每經過一顆decision node會不斷累積積分最後與stage thr 比較積分是否到下一個stage