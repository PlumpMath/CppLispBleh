#pragma once

#include <vector>
#include <sstream>
#include <iterator>

#include "obj.h"

using namespace std;

void skipWhitespace(unsigned long& p, string& src);
Obj read(unsigned long& p, string& src);
