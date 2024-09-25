// Project: 2
// Date: 2/9/24
// Scanner.h - 

#include <iostream>
#include <cctype> //needed for using the isspace function
#include "Token.h"
#include <vector>
#include <cctype>

using namespace std;

#pragma once

class Scanner {
    private:
        string input;
        int count; //total token count
        vector<Token> token_list;

    public:
        Scanner(const string& input, int count = 0) : input(input), count(count) {}    

     vector<Token> scanToken() {
        int line = 1;
        string input_original = input;

        for(int j = 0; j < input.size(); j++) {
            if(isspace(input[j])) {
                if(input[j] == '\n') {
                    line++;
                }
                continue;
            }
            else if(input[j] == ',') {
                token_list.push_back(Token(COMMA, ",", line));
                continue;
            }
            else if(input[j] == '.') {        
                token_list.push_back(Token(PERIOD, ".", line));
                continue;

            }
            else if(input[j] == '?') {
                token_list.push_back(Token(Q_MARK, "?", line));
                continue;

            }
            else if(input[j] == '(') {
                token_list.push_back(Token(LEFT_PAREN, "(", line));
                continue;
            }
            else if(input[j] == ')') {
                token_list.push_back(Token(RIGHT_PAREN, ")", line));
                continue;
            }
            else if(input[j] == ':') {
                if(input[j+1] == '-') {
                    token_list.push_back(Token(COLON_DASH, ":-", line));
                    j++;
                    continue;
                }
                token_list.push_back(Token(COLON, ":", line));
                continue;
            }
            else if(input[j] == '*') {
                token_list.push_back(Token(MULTIPLY, "*", line));
                continue;
            }
            else if(input[j] == '+') {
                token_list.push_back(Token(ADD, "+", line));
                continue;
            }         
            else if(input.substr(j, 7) == "Schemes" && (input[j+7] == ':' || input[j+7] == ' ' || input[j+7] == '\n')) {
                token_list.push_back(Token(SCHEMES, input_original.substr(j, 7), line));
                j = j + 6;
                continue;
            }
            else if(input.substr(j, 5) == "Facts" && (input[j+5] == ':' || input[j+5] == ' ' || input[j+5] == '\n')) {
                token_list.push_back(Token(FACTS, input_original.substr(j, 5), line));
                j = j + 4;
                continue;
            }
            else if(input.substr(j, 5) == "Rules" && (input[j+5] == ':' || input[j+5] == ' ' || input[j+5] == '\n')) {
                token_list.push_back(Token(RULES, input_original.substr(j, 5), line));
                j = j + 4;
                continue;
            }
            else if(input.substr(j, 7) == "Queries" && (input[j+7] == ':' || input[j+7] == ' ' || input[j+7] == '\n')) {
                token_list.push_back(Token(QUERIES, input_original.substr(j, 7), line));
                j = j + 6;
                continue;
            }
        
            // IDS
            else if(isalpha(input[j])) {
                int i = 1;
                while(!isspace(input[j+i]) && (isdigit(input[j+i]) || isalpha(input[j+i]))) {
                    i++;
                }
                token_list.push_back(Token(ID, input_original.substr(j, i), line));
                    j = j + i - 1;
                    continue;
            }

            // STRINGS
            else if(input[j] == '\'') {
                int line_count = line;
                int i = 1;
                while(!(input[j+i] == '\'')) {
                    if(j + i == input.size() - 1) {
                        token_list.push_back(Token(UNDEFINED, input_original.substr(j, i), line));
                        j = j + i - 1;
                        line = line_count;
                        continue;
                    }
                    else if(input[j+i] == '\n') {
                        line_count++;
                    }
                    i++;
                }
                if(input[j] == '\'') {
                    token_list.push_back(Token(STRING, input_original.substr(j, i + 1), line));
                    line = line_count;
                    j = j + i;
                    continue;
                }
            }
            
            // COMMENTS -> Skip all comments
            else if(input[j] == '#') {
                int line_count = line;
                
                if(input[j+1] == '|') { // COMMENT BLOCK
                    int i = 1;

                    if(j + i + 1 != input.size()) { // checks to make sure it an enter the while statement
                        while(!(input[j+i] == '|' && input[j+i+1] == '#')) { //checks that 
                            if(j + i == input.size() - 1) { //UNDEFINED
                                //token_list.push_back(Token(UNDEFINED, input_original.substr(j, i), line));
                                j = j + i - 1;
                                line = line_count;
                                continue;
                            }
                            if(input[j + i] == '\n') {
                                line_count++;
                            }
                            i++;
                        }
                    }
                    else { 
                        //token_list.push_back(Token(UNDEFINED, input_original.substr(j, i + 1), line));
                        line = line_count;
                        j = j + i;
                        continue;
                    }
                    //token_list.push_back(Token(COMMENT, input.substr(j, i + 2), line));
                    line = line_count;
                    j = j + i + 1;
                    continue;
                }

                else { //COMMENT LINE
                    int i = 1;
                    while(!(input[j+i] == '\n')) {  //potentially use for the future-->|| input[j+i] == '#')
                        if(j + i == input.size() - 1) { //UNDEFINED
                            //token_list.push_back(Token(UNDEFINED, input_original.substr(j, i), line));
                            j = j + i - 1;
                            continue;
                        }
                        i++;
                    }
                    //cout << input.substr(j, i) << endl; //testing
                    //token_list.push_back(Token(COMMENT, input.substr(j, i), line));
                    j = j + i - 1;
                    continue;
                }
            }

            // UNDEFINED CATCH ALL
            else {
                token_list.push_back(Token(UNDEFINED, input.substr(j, 1), line));
                continue;
            }
                      
        }
        token_list.push_back(Token(EOTF, "", line));
        return token_list;
    }
};