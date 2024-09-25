// Project: 2
// Date: 2/9/24
// Token.h

#include <sstream> //needed for toString function
#include <string> //needed for toString function

using namespace std;

#pragma once //avoids mulitple includes of the file

enum TokenType {
    COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COMMENT, UNDEFINED, EOTF
}; //have to change EOF to EOTF bc EOF is a key word in c++

class Token { //Token class, should include all public and private declarations
    private:
        TokenType type; //type
        string value; //value
        int line; //line number

    public: //Token class constructor
        Token(TokenType type, string value, int line) : type(type), value(value), line(line) {}

    string toString() const { //toString function
        stringstream out;
        out << "(" << getName() << "," << "\"" << value << "\"" << "," << line << ")";
        return out.str();
    }

    string getValue() const {
        return value;
    }

    TokenType getType() const {
        return type;
    }

    string getName() const {
        switch (type) {
            case COMMA: return "COMMA";
            case PERIOD: return "PERIOD";
            case Q_MARK: return "Q_MARK";
            case LEFT_PAREN: return "LEFT_PAREN";
            case RIGHT_PAREN: return "RIGHT_PAREN";
            case COLON: return "COLON";
            case COLON_DASH: return "COLON_DASH";
            case MULTIPLY: return "MULITPLY";
            case ADD: return "ADD";
            case SCHEMES: return "SCHEMES";
            case FACTS: return "FACTS";
            case RULES: return "RULES";
            case QUERIES: return "QUERIES";
            case ID: return "ID";
            case STRING: return "STRING";
            case COMMENT: return "COMMENT";
            case UNDEFINED: return "UNDEFINED";
            case EOTF: return "EOF";
        }
    }
};
