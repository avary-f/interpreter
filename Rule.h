#include <vector>
#include "Predicate.h"
#include <iostream>
#include <sstream>
using namespace std;
#pragma once
class Rule {
    private:
        Predicate head;
        vector<Predicate> body;
    public:
        Rule(const Predicate& headParam, const vector<Predicate>& bodyParam) : head(headParam), body(bodyParam) {}
        Rule(const Predicate& headParam) : head(headParam) {}

    Predicate getHead() const{
        return head;
    }
    
    void addBodyPredicate(Predicate bodyPred){
        body.push_back(bodyPred);
    }

    vector<Predicate> getBody() const{
        return body;
    }
    string toString() const {
        stringstream out;
        out << head.toString() << " :- " << body.at(0).toString();
        for(int i = 1; i < body.size(); i++){
            out << "," << body.at(i).toString();
        }
        out << ".";
        return out.str();
    }
    string toStringInterp() const {
        stringstream out;
        out << head.toStringInterp() << " :- ";
        //cout << out.str();
        for(int i = 0; i < body.size(); i++){
            out << body.at(i).toStringInterp();
            if(i != body.size() - 1){
                out << ",";
                //cout << out.str();
            }
            // else{
            //     out << "\n";
            //     //cout << out.str();
            // }
        }
        return out.str();
    }
};