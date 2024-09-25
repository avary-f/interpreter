#include <vector>
#include <string>
using namespace std;
#pragma once
#include "Database.h"
#include "Relation.h"
#include "DatalogProgram.h"
#include <iostream>
#include <unordered_map>
#include <cctype>
#include <unordered_set>
#include "Graph.h"
#include <stack>
#include <functional>
#include <map>
#include "Node.h"
#include "Rule.h"

class Interpreter {

private:
    Database database;
    DatalogProgram program;
    set<int> visited_overall;

public:
    Interpreter(const DatalogProgram& program) : program(program) {}

void evalSchemes(){
    for(int i = 0; i < program.getSchemes().size(); i++){
        string name = program.getSchemes().at(i).getID();
        vector<string> attributes = program.getSchemes().at(i).getParameters();
        database.addRelation(name, Relation(name, attributes));
    }
}

void evalFacts(){
    for(int i = 0; i < program.getFacts().size(); i++){
        string name = program.getFacts().at(i).getID();
        vector<string> values = program.getFacts().at(i).getParameters();
        database.addTupletoRelation(name, Tuple(values));
    }
}

void evalQueries() {
    cout << "Query Evaluation" << endl;
    for (int i = 0; i < program.getQueries().size(); i++) { //query
        string name = program.getQueries().at(i).getID();
        Relation relation = Relation(name, database.getRelationTuples(name));
        map<string, int> variables;
        vector<string> orderOfAppearance;  // Keep track of the order of appearance
        //cout << "a" << endl;
        for (int j = 0; j < program.getQueries().at(i).getParameters().size(); j++) { //param
            string param_name = program.getQueries().at(i).getParameters().at(j);

            if (param_name[0] == '\'') {
                relation = relation.select(j, param_name);
            } else {
                auto var = variables.find(param_name);
                if (var == variables.end()) {
                    variables[param_name] = j;
                    orderOfAppearance.push_back(param_name);  // Record the order
                } else {
                    relation = relation.select2(var->second, j);
                }
            }
        }
        //cout << "b" << endl;
        vector<int> indexes;  // Indexes of variables
        for (const auto& variable : orderOfAppearance) {
            indexes.push_back(variables[variable]);
        }

        // Testing
        // cout << "Variables order of appearance: ";
        // for (const auto& element : orderOfAppearance) {
        //     cout << element << " ";
        // }
        // cout << endl;

        // Project and rename
        relation = relation.project(indexes);
        relation = relation.rename(orderOfAppearance);
        //cout << "c" << endl;
        string result;
        if (relation.sizeTuples() != 0) {
            result = "Yes(" + to_string(relation.sizeTuples()) + ")";
        } else {
            result = "No";
        }
        //cout << "d" << endl;
        cout << program.getQueries().at(i).toString() << "? ";
        cout << result << endl;

        if (variables.size() != 0) {
            cout << relation.toString();
        }
    }
}

bool areEqualIgnoreCase(const std::string& str1, const std::string& str2) {
    // If lengths are different, strings are not equal
    if (str1.length() != str2.length()) {
        return false;
    }
    
    // Convert both strings to the same case (e.g., upper case)
    std::string upperStr1, upperStr2;
    for (char c : str1) {
        upperStr1 += std::toupper(c);
    }
    for (char c : str2) {
        upperStr2 += std::toupper(c);
    }
    
    // Compare the modified strings
    return upperStr1 == upperStr2;
}

static Graph makeGraph(const vector<Rule>& rules) {
    Graph graph(rules.size());
    cout << "Dependency Graph" << endl;
    // add code to add edges to the graph for the rule dependencies
    for(int i = 0; i < rules.size(); i++){
        //cout << "from rule " << "R" << i << ": " << rules.at(i).toStringInterp() << endl;
        for(int j = 0; j < rules.at(i).getBody().size(); j++){
            //cout << "from body predicate: " << rules.at(i).getBody().at(j).toStringInterp() << endl;
            for(int k = 0; k < rules.size(); k++){
                //cout << "to rule " << "R" << k << ": " << rules.at(k).toStringInterp() << endl;
                if(rules.at(i).getBody().at(j).getID() == rules.at(k).getHead().getID()){
                    graph.addEdge(i, k);
                    //cout << "dependency found: (R" << i << ",R" << k << ")" << endl;
                }
            }
        }
         
    }
    cout << graph.toString();
    return graph;

}

static Graph makeReverseGraph(Graph old_graph){
    //cout << "New graph" << endl;
    Graph new_graph(old_graph.size());
    //cout << old_graph.size() << endl;
    vector<pair<int, int>> edges = old_graph.getReverseEdges();
    for (int i = 0; i < edges.size(); i++) {
        int fromNodeID = edges[i].first;
        int toNodeID = edges[i].second;
        new_graph.addEdge(fromNodeID, toNodeID);
        // Do something with fromNodeID and toNodeID
    }
    //cout << new_graph.toString();
    return new_graph;
}

void dfs(int nodeID, set<int>& visited, stack<Node>& order, map<int, Node>& nodes) { 
    // Function to visit nodes in depth-first order and push them onto the stack
    // If the node has already been visited, return
    if (visited.count(nodeID) != 0) {
        return;
    }

    // Mark the node as visited
    visited.insert(nodeID);

    // Get the current node
    Node& currentNode = nodes[nodeID];
    currentNode.setID(nodeID);

    // Get the neighbors of the current node
    vector<int> neighbors = currentNode.getAdjacentNodes(); //need a way to keep track of the nodes that were previously visited vs those that are visited with current node

    // Visit each neighbor recursively
    for (int neighbor : neighbors) {
        //cout << "visited" << endl;
        // for(auto visit: visited){
        //     cout << visit << endl;
        // }
        if(visited_overall.find(neighbor) == visited_overall.end()){ //if neighbor hasn't been visited--is this statement necessary?
            dfs(neighbor, visited, order, nodes);
        }
    }

    // Push the current node onto the stack
    order.push(currentNode);

}


stack<Node> getOrder(map<int, Node> nodes) {
    stack<Node> order;
    // Create a set to keep track of visited nodes
    set<int> visited;
    
    // Iterate through all nodes and perform DFS
    for (const auto& pair : nodes) {
        int nodeID = pair.first;
        dfs(nodeID, visited, order, nodes);
    }

    return order;
}

void SSC() {
    // Make Graph
    Graph graph = makeGraph(program.getRules());
    Graph reverse_graph = makeReverseGraph(graph);
    stack<Node> order = getOrder(reverse_graph.getNodes());

    //cout << "Stack Visual" << endl;
    stack<Node> order_copy = order;
    // while (!order_copy.empty()) {
    //     cout << "R" << order_copy.top().getID() << endl;
    //     order_copy.pop();
    // }
    // cout << endl;

    // Rule Evaluation
    cout << endl;
    cout << "Rule Evaluation" << endl;
    map<int, Node> nodes = graph.getNodes(); // Call on original graph
    while (!order.empty()) {
        //cout << "test2" << endl;
        set<int> strong_connected; //Clear after every iteration
        vector<Rule> rules_to_eval;
        vector<int> values;
        stack<Node> order_testing;
        Node top_node = order.top(); // Get the top node of the stack
        order.pop();
        if(visited_overall.find(top_node.getID()) == visited_overall.end()){
            //cout << "Strong connected for: R" << top_node.getID() << endl;
            dfs(top_node.getID(), strong_connected, order_testing, nodes); //run this on the normal graph
            visited_overall.insert(strong_connected.begin(), strong_connected.end());
            //cout << "size: " << strong_connected.size() << endl;
            for(int index: strong_connected){
                //cout << "SSC: index " << index << endl;
                rules_to_eval.push_back(program.getRules().at(index)); //push strong connected components rule
                values.push_back(index); //push their rule number
            }
            
        
            evalRules(rules_to_eval, values); //evaluate rules
        }
        //cout << endl;

    }
    // for(Rule ssc: rules_to_eval){
    //     //cout << "test" << endl;
    //     //cout << ssc.toString() << endl;
    //     // rules_to_eval.push_back(program.getRules().at(ssc));
    // }
    cout << endl;

}



void evalRules(vector<Rule> rules, vector<int> values) {
    bool changed = true; // Initialize to true to enter the loop
    int count = 0;
    cout << "SCC: ";
    for(int g = 0; g < values.size(); g++){
        cout << "R" << values.at(g);
        if(g != values.size() -1){
            cout << ",";
        }
        else{
            cout << endl;
        }
    }
    //cout << rules.size() << endl;
    while (changed) { // Loop until there are no more changes
        count++;
        int initial = database.getSizeofTuples();
        for (int i = 0; i < rules.size(); i++) { //for each rule
            Rule current_rule = rules.at(i);
            cout << current_rule.toString() << endl;

            string head_rule = current_rule.getHead().getID();
            vector<Relation> relations_list;

            // Pred
            for (int j = 0; j < current_rule.getBody().size(); j++) { // for each pred in body
                // Retrieve relation
                string body_name = current_rule.getBody().at(j).getID();
                Relation relation_old = database.getRelation(body_name);

                // Check if relation is empty
                if (relation_old.getTuples().empty()) {
                    relations_list.clear(); // Clear any previously collected relations
                }

                map<string, int> variables;
                vector<string> orderOfAppearance;

                // Params
                for (int k = 0; k < current_rule.getBody().at(j).getParameters().size(); k++) { //for each param in pred
                    string param_name = current_rule.getBody().at(j).getParameters().at(k); // parameter name
                    if (param_name[0] == '\'') {
                        relation_old = relation_old.select(k, param_name);
                    } else { //variable
                        auto var = variables.find(param_name);
                        if (var == variables.end()) {
                            variables[param_name] = k;
                            orderOfAppearance.push_back(param_name);  // Record the order
                        } else {
                            relation_old = relation_old.select2(var->second, k);
                        }
                    }
                }

                // Project
                vector<int> indexes;
                for (string variable : orderOfAppearance) {
                    indexes.push_back(variables[variable]);
                }
                relation_old = relation_old.project(indexes);                
                relation_old = relation_old.rename(orderOfAppearance);
                relations_list.push_back(relation_old);
            }

            if (relations_list.empty()) {
                continue; // Move on to the next rule
            }

            // Join
            Relation relation_new = relations_list.at(0);
            for (int index = 1; index < relations_list.size(); index++) {
                relation_new = relation_new.join(relations_list.at(index));
                //cout << index << " " << relation_new.toString() << endl;
                
            }

            // Get Indexes
            vector<int> indexes;
            for(int n = 0; n < current_rule.getHead().getParameters().size(); n++){ //for each param n in head
                for(int m = 0; m < relation_new.getScheme().size(); m++){ // for each param in current scheme
                    string s = relation_new.getScheme().at(m);
                    if(areEqualIgnoreCase(current_rule.getHead().getParameters().at(n), s)){   
                        indexes.push_back(m); //add index of current scheme
                        //cout << "m: " << m << endl;
                    }
                }
            }

            relation_new = relation_new.project(indexes);
            relation_new.setName(head_rule);
            relation_new.setScheme(current_rule.getHead().getParameters());
            database.union_relation(relation_new);

        }
       if(database.getSizeofTuples() == initial){
            changed = false;
       }
       else{
            changed = true;
       }
        bool isDep = false;
        if(rules.size() != 0  && rules.size() == 1) {
            for(int j = 0; j < rules.at(0).getBody().size(); j++) { //body pred, checks if the rule is dependent on itself
                    if(rules.at(0).getBody().at(j).getID() == rules.at(0).getHead().getID()){
                        isDep = true;
                    }
            }
            if(!isDep){
                changed = false;
            }
        }

    }


    //Output - Moved outside the loop
    // for(auto element: program.getRules()){
    //     cout << element.toString() << endl;
    // }
    //cout << endl;
    cout << count << " passes: ";
    for(int g = 0; g < values.size(); g++){
        cout << "R" << values.at(g);
        if(g != values.size() -1){
            cout << ",";
        }
        else{
            cout << endl;
        }
    }
    //cout << endl;
}

};