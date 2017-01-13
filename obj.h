#pragma once

#include <vector>
#include <iostream>

using namespace std;

enum ObjTag {
    List,
    Number,
    Symbol,
    Invalid,
};

struct Obj {
    Obj();
    ObjTag tag;
    vector<Obj> list;
    double number;
    string symbol;
    string toString();
};
