#include <vector>
#include <string>
using namespace std;
#pragma once
#include <set> // needed to include set class
#include <iostream>
#include <algorithm>

class Node {

 private:

  set<int> adjacentNodeIDs;
  int index;

 public:

  Node() {}

  void addEdge(int adjacentNodeID) {
    adjacentNodeIDs.insert(adjacentNodeID);
  }


  vector<int> getAdjacentNodes() {
    vector<int> nodes(adjacentNodeIDs.begin(), adjacentNodeIDs.end());
    return nodes;
  }

  int getID(){
    return index;
  }

  void setID(int x){
    index = x;
  }

  string toString() const{
    string s;
    int count = 0;
    int size = adjacentNodeIDs.size();
    for (int index : adjacentNodeIDs) {
        if (size - 1 == count) {
            s += "R" + to_string(index);
        } else {
            s += "R" + to_string(index) + ",";
        }
        count++;
    }
    return s; // Add this line to return the constructed string
}


};