
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

# details

I used to implenment with DFS skill.