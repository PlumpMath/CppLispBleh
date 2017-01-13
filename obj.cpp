#include "obj.h"

#include <sstream>

Obj::Obj() {
    tag = Invalid;
}

string Obj::toString() {
    if(tag == List) {
        stringstream ss;
        ss << "(";
        for(Obj& o : list) {
            ss << o.toString();
            ss << " ";
        }
        ss << "\b";
        ss << ")";
        return ss.str();
    } else if(tag == Number) {
        ostringstream s;
        s << number;
        s << ".0";
        return s.str();
    } else if(tag == Symbol) {
        return symbol;
    } else {
        //throw logic_error("Unhandled tag.");
        cerr << "Unhandled tag." << endl;
        exit(1);
    }
}
