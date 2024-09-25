#include <vector>
#include <string>
using namespace std;
#pragma once
#include "Scheme.h"
#include "Tuple.h"
#include <set> // needed to include set class
#include <iostream>
#include <algorithm>

class Relation {

private:
    string name;
    Scheme scheme;
    set<Tuple> tuples;

public:
    Relation(const string name, const Scheme& scheme) : name(name), scheme(scheme) {} //constructor
    Relation(const string name, const Scheme& scheme, const set<Tuple>& tuples) : name(name), scheme(scheme), tuples(tuples) {}
    Relation(const string name, const set<Tuple>& tuples) : name(name), tuples(tuples), scheme(initializeScheme(tuples)){}
    Relation(){}

    Scheme initializeScheme(const set<Tuple>& tuples) {
        if (!tuples.empty()) {
            vector<string> schemes = (*tuples.begin()).getValues();
            return Scheme(schemes);
        }
        // Handle the case when tuples is empty
        return Scheme(); 
    }

bool addTuple(const Tuple& tuple) {
    return tuples.insert(tuple).second; // Return true if the insertion was successful, false otherwise
}
set<Tuple> getTuples() {
    return tuples;
}

vector<string> getScheme() const{
    return scheme.getNames();
}

string getName(){
    return name;
}

int sizeTuples() const{
    return tuples.size();
}

void setScheme(Scheme s){
    scheme = s;
}

void setName(string n){
    name = n;
}

string toString() const {
    string str = "";
    if (tuples.empty()) {
        //cout << "says its empty" << endl;
        return "";
    }
    auto current = tuples.begin(); // will be an iterator pointing to the first element of tuples set
    //str = current->toString(scheme); // the -> is used to dereference the current object
    for (int i = 0; i < tuples.size(); i++) { // use ++current in the loop header
        str = str + "  " + current->toString(scheme) + "\n";
        current++;
    }
    return str;
    
}

Relation select2(int index, int index2) const { //compares two indexes
    Relation rel(name, scheme);
    auto current = tuples.begin();
    for(int i = 0; i < tuples.size(); i++){
        if(current->at(index) == current->at(index2)){
            rel.addTuple(*current);
        }
        current++;
    }
    return rel;
}

Relation select(int index, const string& value) const { //compares index and passed value
    Relation rel(name, scheme);
    auto current = tuples.begin();
    for(int i = 0; i < tuples.size(); i++){
        if(current->at(index) == value){
            rel.addTuple(*current);
        }
        current++;
    }
    return rel;
}

Relation rename(vector<string> newNames) const {
    Scheme newScheme(newNames);
    Relation rel(name, newScheme);
    auto current = tuples.begin();
    for(int i = 0; i < tuples.size(); i++){
        rel.addTuple(*current);
        current++;
    }    
    return rel;
}

Relation project(vector<int> indexes) const { 
    vector<string> newSchemeList;
    for(int j = 0; j < indexes.size(); j++){
        for(int i = 0; i < scheme.size(); i++){
            if(i == indexes.at(j)){
                newSchemeList.push_back(scheme.at(i));
            }
        }
    }

    Scheme newScheme(newSchemeList);
 
    //Create new relation
    Relation rel(name, newScheme); //uses current scheme name & adds new Scheme list

    //Add tuples to relation
    auto current = tuples.begin();
    vector<string> newValues;
    for(Tuple element : tuples) { //iterate through each tuple
        for(int j = 0; j < indexes.size(); j++){ //loops through the bad indexes
            for(int i = 0; i < current->size(); i++){ //loops through the current's value indexes
                if(i == indexes.at(j)){
                    newValues.push_back(current->at(i));
                }
            }
        }
        Tuple n(newValues);
        rel.addTuple(n);
        newValues = {};
        current++;
    }
    return rel;
}

bool joinable(const Scheme& leftScheme, const Scheme& rightScheme, const Tuple& leftTuple, const Tuple& rightTuple) {
    vector<int> sharedLeftIndexes = sharedLeft(leftScheme, rightScheme);
    vector<int> sharedRightIndexes = sharedRight(leftScheme, rightScheme);
    vector<int> uniqueRightIndexes = uniqueRight(leftScheme, rightScheme);

    for (int i = 0; i < sharedLeftIndexes.size(); i ++) {
        if (rightTuple.at(sharedRightIndexes.at(i)) != leftTuple.at(sharedLeftIndexes.at(i))) {
            return false; // Unique right column values are not consistent, tuples are not joinable
        }
    }
    return true;
}

Tuple getCombinedTuple(vector<int> indexes, vector<string> left, vector<string> right){
    vector<string> values = left;
    if(indexes.size()== 0){
        for(int i = 0; i < right.size(); i++){
            values.push_back(right.at(i));
        }
        return Tuple(values);
    }
    for(int i = 0; i < right.size(); i++){
        auto it = find(indexes.begin(), indexes.end(), i);
        if(it == indexes.end()){ //if left doesn't have that same scheme
            values.push_back(right.at(i));
        }
    }
    Tuple result = Tuple(values);
    return result;
}

Scheme getCombinedScheme(Scheme left, Scheme right){
    vector<string> names = left.getNames();
    for(int i = 0; i < right.getNames().size(); i++){
        auto it = find(names.begin(), names.end(), right.at(i));
        if(it == names.end()){ //if left doesn't have that same scheme
            names.push_back(right.at(i));
        }
    }
    Scheme scheme = Scheme(names);
    return scheme;
}

vector<int> sharedLeft(Scheme left, Scheme right){
    vector<int> indexes_left;
    for(int i = 0; i < left.getNames().size(); i++){
        for(int j = 0; j < right.getNames().size(); j++){
            if(left.getNames().at(i) == right.getNames().at(j)){
                indexes_left.push_back(i);
            }
        }
    }
    return indexes_left;
}

vector<int> sharedRight(Scheme left, Scheme right){
    vector<int> indexes_right;
    for(int i = 0; i < left.getNames().size(); i++){
        for(int j = 0; j < right.getNames().size(); j++){
            if(left.getNames().at(i) == right.getNames().at(j)){
                indexes_right.push_back(j);
            }
        }
    }
    return indexes_right;
}

vector<int> uniqueRight(Scheme left, Scheme right){
    vector<int> indexes_right_u;
    vector<string> names = left.getNames();
    for(int i = 0; i < right.getNames().size(); i++){
        auto it = find(names.begin(), names.end(), right.at(i));
        if(it == names.end()){ //if left does not have that right value
            indexes_right_u.push_back(i);
        }
    }
    return indexes_right_u;
}

Relation join(Relation& right) {
    Relation& left = *this;

    Scheme s = Scheme(getCombinedScheme(left.scheme, right.scheme));
    Relation result;
    result.setScheme(s);
    if(left.tuples.empty()){
        if(right.tuples.empty()){
            return result;
        }
        return right;
    }
    if(right.tuples.empty()){
        if(left.tuples.empty()){
            return result;
        }
        return left;
    }

    
    for(Tuple leftTuple: left.tuples){
        for(Tuple rightTuple: right.tuples){
            if(joinable(left.getScheme(), right.getScheme(), leftTuple, rightTuple)){ 
                vector<int> sharedIndexes = sharedLeft(left.scheme, right.scheme);
                vector<int> uniqueIndexes = uniqueRight(left.scheme, right.scheme);

                vector<string> combinedValues = leftTuple.getValues();
                for(auto index : uniqueIndexes) {
                    combinedValues.push_back(rightTuple.at(index));
                }
                Tuple combinedTuple = Tuple(combinedValues);
                result.addTuple(combinedTuple);
            }
        }
    }
    return result;
}
};
