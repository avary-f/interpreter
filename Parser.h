#include <iostream>
#include <vector>
#include "Token.h"
#include "DatalogProgram.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"
#include "Interpreter.h"
using namespace std;
#pragma once
class Parser {
    private:
        vector<Token> tokens;
        DatalogProgram program;
    public:
        Parser(const vector<Token>& tokens) : tokens(tokens){}

    void parse() {
        try {
            datalogProgram();
            Interpreter interpreter = Interpreter(program);
            interpreter.evalSchemes();
            interpreter.evalFacts();
            //interpreter.evalRules(); //testing
            interpreter.SSC();
            interpreter.evalQueries(); 
        } catch (Token error) {
            cout << "  " << error.toString() << endl;
        }
    }
    TokenType tokenType() const {
        return tokens.at(0).getType();
    }
    void advanceToken() {
        tokens.erase(tokens.begin());
    }
    void throwError() { //not complete
        cout << "Failure!" << endl;
        throw tokens.at(0);
    }
    void match(TokenType t) {
        if(tokenType() == t) {
            advanceToken();
        }
        else {
            throwError();
        }        
    }
    void datalogProgram() {
        match(SCHEMES);
        match(COLON);
        program.addScheme(scheme());
        //scheme();
        program.addSchemeList(schemeList());
        match(FACTS);
        match(COLON);
        program.addFactList(factList());
        //factList();
        match(RULES);
        match(COLON);
        program.addRuleList(ruleList());
        //ruleList();
        match(QUERIES);
        match(COLON);
        program.addQuery(query());
        program.addQueryList(queryList());
        //queryList();
        match(EOTF); //return datalogprogrma object?
    }
    vector<Predicate> schemeList() {
        vector<Predicate> list;
        if (tokenType() == ID) {
            list.push_back(scheme());
            vector<Predicate> restOfList = schemeList();
            list.insert(list.end(), restOfList.begin(), restOfList.end());
            return list;
        } else {
            return {}; // lambda -> do nothing
        }
    }
    vector<Predicate> factList() {
        vector<Predicate> list;
        if (tokenType() == ID) {
            //program.addFact(facts());
            list.push_back(facts());
            //facts();
            vector<Predicate> restOfList = factList();
            list.insert(list.end(), restOfList.begin(), restOfList.end());
            //factList();
            return list;
        } else {
            // lambda -> do nothing
        }
    }
    vector<Rule> ruleList() {
        vector<Rule> list;
        if (tokenType() == ID) {
            //program.addRule(rule());
            list.push_back(rule());
            vector<Rule> restOfList = ruleList();
            list.insert(list.end(), restOfList.begin(), restOfList.end());
            //rule();
            //ruleList();
            return list;
        } else {
            // lambda -> do nothing
        }
    }
    vector<Predicate> queryList() {
        vector<Predicate> list;
        if (tokenType() == ID) {
            list.push_back(query());
            vector<Predicate> restOfList = queryList();
            list.insert(list.end(), restOfList.begin(), restOfList.end());
            return list;
        } else {
            // lambda -> do nothing
        }
    }
    Predicate scheme() {
        string name = tokens.at(0).getValue();
        match(ID); //name of the predicate
        match(LEFT_PAREN);
        vector<Parameter> param;
        param.push_back(Parameter(tokens.at(0).getValue())); //pushing the parameter object
        match(ID); //create parameter (vector)
        vector<Parameter> restOfParams = idList();
        param.insert(param.end(), restOfParams.begin(), restOfParams.end()); //Add idlist to the end of the vector that you just created
        match(RIGHT_PAREN);
        return Predicate(name, param);

    }
    Predicate facts() {
        string name = tokens.at(0).getValue();
        match(ID);
        match(LEFT_PAREN);
        vector<Parameter> param;
        string paramName = tokens.at(0).getValue();
        param.push_back(Parameter(paramName));
        match(STRING);
        program.addToSet(paramName);
        vector<string> restOfParams = stringList();
        match(RIGHT_PAREN);
        match(PERIOD);
        param.insert(param.end(), restOfParams.begin(), restOfParams.end());
        return Predicate(name, param);
    }
    Rule rule() { 
        Predicate head = headPredicate();
        match(COLON_DASH);
        vector<Predicate> body;
        body.push_back(predicate());
        vector<Predicate> restOfPreds = predicateList();
        body.insert(body.end(), restOfPreds.begin(), restOfPreds.end());
        match(PERIOD);
        return Rule(head, body);
    }
    Predicate query() { 
        Predicate pred = predicate();
        match(Q_MARK);
        return pred;
    }
    Predicate headPredicate() {
        string name = tokens.at(0).getValue();
        match(ID);
        match(LEFT_PAREN);
        vector<Parameter> param;
        param.push_back(Parameter(tokens.at(0).getValue()));
        match(ID);
        vector<Parameter> restOfParams = idList();
        param.insert(param.end(), restOfParams.begin(), restOfParams.end());
        //idList();
        match(RIGHT_PAREN);
        return Predicate(name, param);
    }
    Predicate predicate() {
        string name = tokens.at(0).getValue();
        match(ID);
        match(LEFT_PAREN);
        vector<Parameter> param;
        param.push_back(parameter());
        vector<Parameter> restOfParams = parameterList();
        param.insert(param.end(), restOfParams.begin(), restOfParams.end());
        match(RIGHT_PAREN);
        return Predicate(name, param);
    }
    vector<Predicate> predicateList() {
        vector<Predicate> list;
        if (tokenType() == COMMA) {
            match(COMMA);
            list.push_back(predicate());
            vector<Predicate> restOfList = predicateList();
            list.insert(list.end(), restOfList.begin(), restOfList.end());
            return list;
        } else {
            return {}; // lambda -> do nothing
        }
    }
    vector<Parameter> parameterList() {
        vector<Parameter> list;
        if (tokenType() == COMMA) {
            match(COMMA);
            list.push_back(parameter());
            vector<Parameter> restOfList = parameterList();
            list.insert(list.end(), restOfList.begin(), restOfList.end());
            return list;
        } else {
            return {}; // lambda -> do nothing
        }
    }
    vector<string> stringList() {
        vector<string> list;
        if (tokenType() == COMMA) {
            match(COMMA);
            string name = tokens.at(0).getValue();
            list.push_back(name);
            match(STRING);
            program.addToSet(name);
            vector<string> restOfList = stringList();
            list.insert(list.end(), restOfList.begin(), restOfList.end());
            return list;
        } else {
            return {}; // lambda -> do nothing
        }
    }
    vector<Parameter> idList() {
        vector<Parameter> list;
        if (tokenType() == COMMA) {
            match(COMMA);
            list.push_back(Parameter(tokens.at(0).getValue()));
            match(ID);
            vector<Parameter> restOfList = idList();
            list.insert(list.end(), restOfList.begin(), restOfList.end());
            return list;
        } else {
            return {}; // lambda -> do nothing
        }
    }
    Parameter parameter() {
        Parameter param = Parameter(tokens.at(0).getValue());
        if (tokenType() == STRING) {
            match(STRING);
            return param;
        } 
        if (tokenType() == ID) {
            match(ID);
            return param;
        }
        return Parameter(""); //not sure if this line is right
    }
};