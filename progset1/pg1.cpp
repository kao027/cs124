#include <chrono>
#include <vector>
#include <iostream>
#include <cmath>

int n = 3; // number of nodes
void addEdge(std::vector<std::vector<std::pair<int,float>>> &adj, int u, int v, float w){
    adj[u].push_back({v,w});
    adj[v].push_back({u,w}); // because undirected
}

//graph type #1
void generateComplete(std::vector<std::vector<std::pair<int,float>>> &adj){
    srand(time(0));
    for (int u = 0; u < n; u++){
        for (int v = u+1; v < n; v++){
            float newWeight = static_cast<float>(rand()) / RAND_MAX; // generate number from 0 to 1
            addEdge(adj, u, v, newWeight);
        }
    }
}

void displayAdjList(std::vector<std::vector<std::pair<int,float>>>& adj) {
    for (int i = 0; i < adj.size(); i++) {
        std::cout << i << ": "; 
        for (auto &j : adj[i]) {
           std::cout << "{"<<j.first << ", "<<j.second<<"} "; 
        }
        std::cout << std::endl; 
    }
}


int main(){
    std::vector<std::vector<std::pair<int,float>>> graph(n);
    generateComplete(graph);
    displayAdjList(graph);
}
