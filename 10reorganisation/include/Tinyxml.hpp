#ifndef TINYXML_HPP
#define TINYXML_HPP
#include <bits/stdc++.h>
using namespace std;

struct XmlNode {
  string name;                //
  string val;                 //
  vector<XmlNode*> children;  //

  XmlNode(string name = "", string val = "") : name(name), val(val) {}
};
enum {
  TAG_HEAD = 0,
  TAG_START = 1,
  TAG_END = 2,
  TAG_ANNOTATION = 3,
  TAG_UNKNOW = 4
};
void deleteXml(XmlNode* node);  // áŒ·ÅÓ›‘›ów
XmlNode* readXml(string filename);
XmlNode* findxmlnode(XmlNode* cur, const string& s);     // find depth first
XmlNode* findxmlNxtnode(XmlNode* cur, const string& s);  // find neighbor first
void printXml(XmlNode* node, int depth);
XmlNode* findfullxmlnode(XmlNode* cur, const string& s);
#endif