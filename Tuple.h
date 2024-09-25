#include <vector>
#include <string>
using namespace std;
#pragma once

#include "Scheme.h"

class Tuple {

private:

  vector<string> values;

public:

  Tuple(vector<string> values) : values(values) { }

  unsigned size() const {
    return values.size();
  }

  const string& at(int index) const {
    return values.at(index);
  }

  vector<string> getValues() const {
    return values;
  }

  bool operator<(const Tuple t) const {
    return values < t.values;
  }

  string toString(const Scheme& s) const {
    string str;
    const Tuple& tuple = *this;  
    if (s.size() == 0) {
        return "";
    }

    str = s.at(0) + "=" + tuple.at(0);

    for (int i = 1; i < s.size(); i++) {
        str += " , ";
        
        if (i < tuple.size()) {
            str += s.at(i) + "=" + tuple.at(i);
        } 
    }
    return str;
}


};

//you can also just inheirit from the string class instead