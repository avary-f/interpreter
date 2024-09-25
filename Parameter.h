#include <iostream>
#pragma once
using namespace std;
class Parameter {
    private:
        string name;
    public:
        Parameter(const string paramName) : name(paramName) {}

    string toString() const {
        return name;
    }

    string getName(){
        return name;
    }
};