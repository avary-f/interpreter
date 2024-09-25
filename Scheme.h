#include <vector>
#include <string>
using namespace std;
#pragma once

class Scheme { //delegation 

private: 

  vector<string> names;

public:

  Scheme(vector<string> names) : names(names) { }
  Scheme() = default;

  unsigned size() const {
    return names.size();
  }

  const string& at(int index) const {
    return names.at(index);
  }

  const vector<string> getNames() const{
    return names;
  }

};