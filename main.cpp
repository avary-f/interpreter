// Project: 1
// Date: 1/29/24
// main.cpp

#include <iostream>
#include <vector>
#include <fstream>

#include <string>
#include "Token.h" //needed to access the token class
#include "Scanner.h"
#include "Parser.h"
#include "Scheme.h"
#include "Tuple.h"
#include "Relation.h"
#include "Node.h"
#include "Graph.h"
#include "Rule.h"
#include "Predicate.h"
//#include "Interpreter.h"

using namespace std;


//int main() {
int main(int argc, char* argv[]) {
  ifstream inputFile(argv[1]); // Open file for reading
  //ifstream inputFile("text.txt"); // Testing

  if (!inputFile.is_open()) { // Check if file is open
      cerr << "Error opening the file" << endl;
      return 1; // Return an error code
  }

  string line;
  stringstream ss;
  while (getline(inputFile, line)) {
      ss << line << '\n';
  }
  string full_input = ss.str();

  inputFile.close(); // Close file

  Scanner s = Scanner(full_input);
  vector<Token> tokens = s.scanToken();
  //cout << "Size of tokens: " << tokens.size() << endl;
  Parser parser = Parser(tokens);
  parser.parse();

  // for (int i = 0; i < tokens.size(); i++) {
  //     cout << tokens[i].toString() << endl;
  // }
  // cout << "Total Tokens = " << tokens.size() << endl;

  return 0;
}


//------ LAB 5 ---------------------
// int main() {

//   // predicate names for fake rules
//   // first is name for head predicate
//   // second is names for body predicates
//   pair<string,vector<string>> ruleNames[] = {
//     { "A", { "B", "C" } },
//     { "B", { "A", "D" } },
//     { "B", { "B" } },
//     { "E", { "F", "G" } },
//     { "E", { "E", "F" } },
//   };

//   vector<Rule> rules;

//   for (auto& rulePair : ruleNames) {
//     string headName = rulePair.first;
//     Rule rule = Rule(Predicate(headName));
//     vector<string> bodyNames = rulePair.second;
//     for (auto& bodyName : bodyNames)
//       rule.addBodyPredicate(Predicate(bodyName));
//     rules.push_back(rule);
//   }

//   Graph graph = Interpreter::makeGraph(rules);
//   cout << graph.toString();

// }






//  int main() {

//   // Relation studentRelation("students", Scheme( {"ID", "Name", "Major"} ));

//   // vector<string> studentValues[] = {
//   //   {"'42'", "'Ann'", "'CS'"},
//   //   {"'64'", "'Ned'", "'EE'"},
//   // };

//   // for (auto& value : studentValues)
//   //   studentRelation.addTuple(Tuple(value));

//   // Relation s = studentRelation.join(studentRelation);
//   // cout << s.toString();

//   // cout << endl;
//   // cout << "Part 2" << endl;
//   // cout << endl;

  

// //Lab 4 Testing------------------------------------------
//   Relation left("left", Scheme());  //{"A", "B", "C"} 

//   // vector<string> values[] = {
//   //   {"2", "1", "1"},
//   // };

//   // for (auto& value : values)
//   //   left.addTuple(Tuple(value));
//   cout << "left: " << left.toString();
  
//   Relation right("rigth", Scheme( {"C", "D", "E"} ));

//   vector<string> values2[] = {
//     {"1", "3", "4"},
//   };

//   for (auto& value : values2)
//     right.addTuple(Tuple(value));

//   cout << "right: " << right.toString();
  
//   Relation r = left.join(right);
//   cout << "together:" << r.toString();

// }

// int main() {

//   vector<string> names = { "S", "N", "A", "P"};

//   Scheme scheme(names);

//   Relation relation("snap", scheme);

//   vector<string> values[] = {
//     {"'12345'", "'C. Brown'", "'12 Apple St.'", "'555-1234"},
//     {"'22222'", "'P.Patty'", "'56 Grape Bvd.'", "'555-9999"},
//     {"'33333'", "'Snoopy'", "'12 Apple St.'", "'555-1234"},
//   };

//   for (auto& value : values) {
//     Tuple tuple(value);
//     //cout << tuple.toString(scheme) << endl;
//     relation.addTuple(tuple);
//   }

//   vector<string> names2 = { "C", "S", "G" };

//   Scheme scheme2(names2);

//   Relation relation2("csg", scheme2);

//   vector<string> values2[] = {
//     {"'CS101'", "'22222'", "'B'"},
//     {"'CS101'", "'33333'", "'C'"},
//     {"'CS101'", "'12345'", "'A'"},
//     {"'EE200'", "'12345'", "'B+'"},
//     {"'EE200'", "'22222'", "'B'"}
// };

// // Loop over valid elements in values2
//   for (auto& value : values2) {
//       Tuple tuple(value);
//       // cout << tuple.toString(scheme2) << endl;
//       relation2.addTuple(tuple);
//   }

//   Relation result = relation.join(relation2);
//   result = result.project({0, 4, 5});

//   cout << result.toString();

// }
