#include <vector>
#include <string>
using namespace std;
#pragma once
#include <set> // needed to include set class
#include <iostream>
#include "Node.h"
#include <map> 

class Graph {

 private:

  map<int,Node> nodes;

 public:

  Graph(int size) {
    for (int nodeID = 0; nodeID < size; nodeID++)
      nodes[nodeID] = Node();
  }

  int size() const{
    return nodes.size();
  }

  map<int, Node> getNodes(){
    return nodes;
  }

  void addEdge(int fromNodeID, int toNodeID) {
    nodes[fromNodeID].addEdge(toNodeID);
  }

 vector<pair<int, int>> getReverseEdges() {
    //cout << "Getting Reverse Edges" << endl;
    vector<pair<int, int>> edges;
    for (auto& pair : nodes) {
        //cout << "test" << endl;
        int fromNodeID = pair.first;
        Node& node = pair.second;
        vector<int> adjacentNodes = node.getAdjacentNodes();
        //cout << "1" << endl;
        //cout << adjacentNodes.size() << endl;
        for (int i = 0; i < adjacentNodes.size(); i++) {
            //cout << "2" << endl;
            int toNodeID = adjacentNodes.at(i);
            edges.push_back({toNodeID, fromNodeID});
        }
    }
    //cout << "out of getting reverse edges" << endl;
    return edges;
}

  string toString(){
    string s;
    for (auto& pair: nodes) {
      int nodeID = pair.first;
      Node node = pair.second;
      s = s + "R" + to_string(nodeID) + ":" + node.toString() + "\n";
    }
    return s;
  }

};