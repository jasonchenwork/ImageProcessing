#include <../include/Tinyxml.hpp>

//
string trim(const string& s) {
  size_t start = s.find_first_not_of(" \t\r\n");
  if (start == string::npos) return "";
  size_t end = s.find_last_not_of(" \t\r\n");
  return s.substr(start, end - start + 1);
}

string getTagLabel(const string& s, size_t& pos, int& attr) {
  attr = TAG_UNKNOW;
  string res = "";
  // skip space
  while (pos < s.size() && isspace(s[pos])) pos++;
  if (pos >= s.size()) {
    attr = TAG_UNKNOW;
    return "";
  }

  if (s[pos] == '<') {
    pos++;
    size_t tagStart = pos + 1;
    if (s[pos] == '/') {
      attr = TAG_END;
    } else if (s[pos] == '!') {
      attr = TAG_ANNOTATION;
    } else if (s[pos] == '?') {
      attr = TAG_HEAD;
    } else {
      tagStart--;
      attr = TAG_START;
    }

    while (pos < s.size() && s[pos] != '>') {
      pos++;
    }
    res = s.substr(tagStart, pos - tagStart);
    pos++;  // skip '>'
  }
  return res;
}
XmlNode* parseElement(const string& s, size_t& pos) {
  int attribute;
  string tagname;
  tagname = getTagLabel(s, pos, attribute);
  if (attribute == TAG_ANNOTATION || attribute == TAG_HEAD) {
    return parseElement(s, pos);
  }
  if (attribute != TAG_START) {
    return nullptr;
  }
  XmlNode* node = new XmlNode(tagname, "");

  size_t tagStart = pos;
  while (pos < s.size() && s[pos] != '<') pos++;
  string tagvalue = s.substr(tagStart, pos - tagStart);
  node->val = trim(tagvalue);

  size_t backupsize = pos;
  tagname = getTagLabel(s, pos, attribute);

  if (attribute == TAG_END) {
    return node;
  } else {
    pos = backupsize;
    XmlNode* child = parseElement(s, pos);
    while (child) {
      node->children.push_back(child);
      child = parseElement(s, pos);
    }
  }
  return node;
}
//  print xml inf
void printXml(XmlNode* node, int depth) {
  if (!node) return;
  if (node->name == "#text") {
    if (!node->val.empty()) cout << string(depth * 2, ' ') << node->val << endl;
    return;
  }

  cout << string(depth * 2, ' ') << "<" << node->name << ">" << endl;
  for (auto child : node->children) {
    printXml(child, depth + 1);
  }
  if (!node->children.empty() || !node->val.empty()) {
    cout << string(depth * 2, ' ') << node->val << endl;
  }
  cout << string(depth * 2, ' ') << "</" << node->name << ">" << endl;
}

//  release
void deleteXml(XmlNode* node) {
  if (!node) return;
  for (auto child : node->children) {
    deleteXml(child);
  }
  delete node;
}
XmlNode* parseXml(const string& xmlText) {
  size_t pos = 0;
  return parseElement(xmlText, pos);
}

XmlNode* readXml(string filename) {
  std::ifstream file(filename);  //
  if (!file.is_open()) {
    std::cout << "can not open file" << std::endl;
    return nullptr;
  }
  string line;
  string xml;
  while (std::getline(file, line)) {
    xml += line;
  }
  XmlNode* root = parseXml(xml);

  file.close();  //
  return root;
}
XmlNode* findfullxmlnode(XmlNode* cur, const string& s) {
  if (cur == nullptr) return nullptr;

  if (cur->name == s) {
    return cur;
  }
  for (auto child : cur->children) {
    XmlNode* result = findfullxmlnode(child, s);
    if (result) return result;
  }
  return nullptr;
}
XmlNode* findxmlnode(XmlNode* cur, const string& s) {
  if (cur == nullptr) return nullptr;

  if (cur->name == s) {
    return cur;
  }
  return cur->children.empty() ? nullptr
                               : findxmlnode(cur->children.front(), s);
}
XmlNode* findxmlNxtnode(XmlNode* cur, const string& s) {
  if (cur == nullptr) return nullptr;

  for (auto child : cur->children) {
    if (child->name == s) {
      return child;
    }
  }
  return nullptr;
}