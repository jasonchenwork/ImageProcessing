#ifndef TINYXML_HPP
#define TINYXML_HPP
#include <bits/stdc++.h>
using namespace std;

struct XmlNode {
  string name;     //
  string val;      //
  XmlNode* next;   //
  XmlNode* child;  //

  XmlNode(string n = "", string v = "")
      : name(n), val(v), next(nullptr), child(nullptr) {}
};
enum {
  TAG_HEAD = 0,
  TAG_START = 1,
  TAG_END = 2,
  TAG_ANNOTATION = 3,
  TAG_UNKNOW = 4
};
XmlNode* readXml(string filename);
XmlNode* findxmlnode(XmlNode* cur, string& s);
#endif