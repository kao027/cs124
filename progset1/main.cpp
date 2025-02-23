#include <chrono>
#include <vector>
#include <iostream>
#include "mst.h"
#include <math.h>

int n = 3; // number of nodes
void addEdge(Graph &adj, int u, int v, float w){
    adj[u].push_back({v,w});
    adj[v].push_back({u,w}); // because undirected
}

//graph type #1
void generateComplete(Graph &adj){
    srand(time(0));
    for (int u = 0; u < n; u++){
        for (int v = u+1; v < n; v++){
            float newWeight = (float)rand() / RAND_MAX;
            addEdge(adj, u, v, newWeight);
        }
    }
}

//graph type #2
void generateHypercube(Graph &adj){
    srand(time(0));
    for (int u = 0; u < n; u++){
        for (int v = u+1; v<n; v++){
            if (((v-u) & (v-u-1))==0){
                float newWeight = (float)rand() / RAND_MAX;
                addEdge(adj, u, v, newWeight);
            }
        }
    }
}

//graph type #3
void generateUnitSquare(Graph &adj){
    srand(time(0));
     std::vector<std::pair<float,float>> coordinates(n);
    for (int i = 0; i < n; i++){
       float x = (float)rand() / RAND_MAX;
       float y = (float)rand() / RAND_MAX;
       coordinates[i] = {x,y};
       //std::cout << "(" << coordinates[i].first << ", " << coordinates[i].second << ") " << "\n";
    }
    for (int u = 0; u < n; u++){
        float ux = coordinates[u].first;
        float uy = coordinates[u].second;
        for (int v = u+1; v < n; v++){
            float vx = coordinates[v].first;
            float vy = coordinates[v].second;
            float euclidWeight = sqrt(pow(vx-ux, 2)+pow(vy-uy, 2));
            addEdge(adj, u, v, euclidWeight);
        }
    }

}

//graph type #4


void displayAdjList(Graph &adj) {
    for (size_t i = 0; i < adj.size(); i++) {
        std::cout << i << ": "; 
        for (auto &j : adj[i]) {
           std::cout << "{"<<j.first << ", "<<j.second<<"} "; 
        }
        std::cout << std::endl; 
    }
}

// Compile with make then ./main

int main(){
    Graph graph(n);
    generateComplete(graph);
    float mstWeight = kruskal(graph); 
    std::cout << "Minimum Spanning Tree Weight (Kruskal): " << mstWeight << std::endl;
    mstWeight = prim(graph); 
    std::cout << "Minimum Spanning Tree Weight (Prim): " << mstWeight << std::endl;
    displayAdjList(graph);
}
