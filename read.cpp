#include "read.h"

#include <iostream>

void skipWhitespace(unsigned long& p, string& src) {
    char c = src[p];
    while(c == ' ') {
        p++;
        c = src[p];
    }
}

bool isSymbolChar(char c) {
    if(isalpha(c)) return true;
    if(isnumber(c)) return true;
    if(c == '+' || c == '-' || c == '*' || c == '?' || c == '!' || c == '=' || c == '<' || c == '>') return true;
    return false;
}

Obj read(unsigned long& p, string& src) {
    char c = src[p];
    if(c == '(') {
        Obj list;
        list.tag = List;
        p++;
        while(true) {
            if(p >= src.length()) {
                //throw runtime_error("Missing ')");
                cerr << "Missing ')'." << endl;
                exit(1);
            }
            c = src[p];
            if(c == ')') {
                break;
            }
            Obj o = read(p, src);
            list.list.push_back(o);
            skipWhitespace(p, src);
        }
        return list;
    }
    else if(isnumber(c)) {
        Obj number;
        number.tag = Number;
        string s = "";
        while(true) {
            c = src[p];
            if(!isnumber(c)) {
                break;
            }
            s += c;
            p++;
        }
        number.number = stoi(s);
        return number;
    }
    else {
        Obj symbol;
        symbol.tag = Symbol;
        symbol.symbol = "";
        while(true) {
            c = src[p];
            if(!isSymbolChar(c)) {
                break;
            }
            symbol.symbol += c;
            p++;
        }
        return symbol;
    }
}
