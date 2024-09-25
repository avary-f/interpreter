#include <vector>
#include "Parameter.h"
#include <iostream>
#include <sstream>
using namespace std;
#pragma once
class Predicate {
    private:
        string ID;
        vector<Parameter> parameters;
    public:
        Predicate(const string& paramName, const vector<Parameter>& params) : ID(paramName), parameters(params) {}
        Predicate(const string& paramName) : ID(paramName) {}

    string getID() const{
        return ID;
    }
    vector<string> getParameters(){
        vector<string> paramNames;
        for(int i = 0; i < parameters.size(); i++){
            paramNames.push_back(parameters.at(i).toString());
        }
        return paramNames;
    }

    string toString() const {
        stringstream pred;
        pred << ID << "(" << parameters.at(0).toString();
        for(int i = 1; i < parameters.size(); i++){
            pred << "," << parameters.at(i).toString();
        }
        pred << ")";
        return pred.str();
    }

    string toStringInterp() const {
        stringstream pred;
        pred << ID << "(" ;
        pred << ")";
        return pred.str();
    }
};