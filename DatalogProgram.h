#include <vector>
#include "Predicate.h"
#include "Rule.h"
#include <set>
#include <iostream>
using namespace std;
#pragma once

class DatalogProgram {
    private:
        vector<Predicate> schemes;
        vector<Predicate> facts;
        vector<Rule> rules;
        vector<Predicate> queries;
        set<string> domain;
    public:
        DatalogProgram() {}

    vector<Predicate> getSchemes() {
        return schemes;
    }
    vector<Predicate> getFacts() {
        return facts;
    }
    vector<Predicate> getQueries() {
        return queries;
    }
    vector<Rule> getRules() {
        return rules;
    }
    void addToSet(string newString) {
        domain.insert(newString);
    }
    void addScheme(Predicate newPred) {
        schemes.push_back(newPred);
    }
    void addFact(Predicate newPred) {
        facts.push_back(newPred);
    }
    void addRule(Rule newRule) {
        rules.push_back(newRule);
    }
    void addQuery(Predicate newPred) {
        queries.push_back(newPred);
    }
    void addSchemeList(vector<Predicate> newPred) {
        schemes.insert(schemes.end(), newPred.begin(), newPred.end());
    }
    void addFactList(vector<Predicate> newPred) {
        facts.insert(facts.end(), newPred.begin(), newPred.end());
    }
    void addRuleList(vector<Rule> newPred) {
        rules.insert(rules.end(), newPred.begin(), newPred.end());
    }
    void addQueryList(vector<Predicate> newPred) {
        queries.insert(queries.end(), newPred.begin(), newPred.end());
    }
    void print() {
        cout << "Schemes (" << schemes.size() << ") :" << endl;
        for(int i = 0; i < schemes.size(); i++) {
            cout << "  " << schemes.at(i).toString() << endl;
        }
        cout << "Facts (" << facts.size() << ") :" << endl;
        for(int i = 0; i < facts.size(); i++) {
            cout << "  " << facts.at(i).toString() << "." << endl;
        }
        cout << "Rules (" << rules.size() << ") :" << endl;
        for(int i = 0; i < rules.size(); i++) {
            cout << "  " << rules.at(i).toString() << endl;
        }
        cout << "Queries (" << queries.size() << ") :" << endl;
        for(int i = 0; i < queries.size(); i++) {
            cout << "  " << queries.at(i).toString() << "?" << endl;
        }
        cout << "Domain (" << domain.size() << ") :" << endl;
        for (const string& element : domain) {
            cout << "  " << element << endl;
        }
        //print out all the values of Facts in a set? in a for loop go through facts, add parameters to a set -> gets rid of duplicates and orders them
    }
};