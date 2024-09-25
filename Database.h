#include <vector>
#include <string>
using namespace std;
#pragma once
#include "Relation.h"
#include "Tuple.h"
#include <map> // needed to include set class
#include <iostream>

class Database {

private:
    string name;
    map<string, Relation> relations;

public:
    Database(const string name, const map<string, Relation> relations) : name(name), relations(relations) {}
    Database(){}

void addRelation(string name, const Relation& relation) {
    relations.emplace(name, relation);
}
void addTupletoRelation(string key, const Tuple& tuple) {
    auto it = relations.find(key);
    if (it != relations.end()) {
        it->second.addTuple(tuple);
    }
}

set<Tuple> getRelationTuples(string key){
    auto it = relations.find(key);
    return it->second.getTuples();
}

Relation getRelation(string key){
    auto it = relations.find(key);
    if (it != relations.end()) {
        return it->second;
    }
}

void union_relation(Relation r) {
    auto key = relations.find(r.getName());
    if(key != relations.end()){
        // Check if any tuples are added
        for(const auto& tuple : r.getTuples()) {
            if (key->second.addTuple(tuple)) {
                cout << "  " << tuple.toString(key->second.getScheme()) << endl;
            }
        }
    }    
}

int getSizeofTuples(){
    int count = 0;
    for(auto relation: relations){
        for(auto tuple: relation.second.getTuples()){
            count++;
        }
    }
    return count;
}

string toString() const {
    string str;
    if (relations.empty()) {
        return "";
    }
    for (auto current = relations.begin(); current != relations.end(); ++current) {
        str = str + current->first + ": " + current->second.toString() + "\n";
    }
    return str;
}

};