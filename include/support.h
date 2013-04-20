#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <list>
#include <map>

typedef unsigned int ID;

extern void makebox(int* target, int left, int top, int right, int bottom);
extern std::string truncate_string(const char* s);
extern std::vector <std::string> txt2lines(const char* filename, const std::string& ignore, bool quiet = true);
extern std::vector <std::string> string2tokens(const std::string& source, const std::string& delimiters);

#endif
