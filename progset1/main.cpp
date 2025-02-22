#include <chrono>
#include <vector>
#include <iostream>
#include "mst.h"
#include <math.h>

int n = 5; // number of nodes
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
            if (((v-u) & (v-u-1))==0){ // if v-u is a power of 2
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
    for (int i = 0; i < n; i++){ // pick the points
       float x = (float)rand() / RAND_MAX;
       float y = (float)rand() / RAND_MAX;
       coordinates[i] = {x,y}; // store the points in a vector
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
struct unitPoint { // no tuples in C :(
    float x, y, z;
};

void generate3D(Graph &adj){
    std::vector<unitPoint> coordinates(n);
    for (int i = 0; i < n; i++){ // pick the points
        float x = (float)rand() / RAND_MAX;
        float y = (float)rand() / RAND_MAX;
        float z = (float)rand() / RAND_MAX;
        coordinates[i] = {x,y,z}; // store the points in a vector
    }
    for (int u = 0; u < n; u++){
        float ux = coordinates[u].x;
        float uy = coordinates[u].y;
        float uz = coordinates[u].z;
        for (int v = u+1; v < n; v++){
            float vx = coordinates[v].x;
            float vy = coordinates[v].y;
            float vz = coordinates[v].z;
            float euclidWeight = sqrt((vx-ux)*(vx-ux)+(vy-uy)*(vy-uy)+(vz-uz)*(vz-uz));
            addEdge(adj, u, v, euclidWeight);
        }
    }

}


struct hyperPoint { 
    float x,y,z,w;
};

void generate4D(Graph &adj){
    std::vector<hyperPoint> coordinates(n);
    for (int i = 0; i < n; i++){ // pick the points
        float x = (float)rand() / RAND_MAX;
        float y = (float)rand() / RAND_MAX;
        float z = (float)rand() / RAND_MAX;
        float w = (float)rand() / RAND_MAX;
        coordinates[i] = {x,y,z,w}; // store the points in a vector
    }
    for (int u = 0; u < n; u++){
        float ux = coordinates[u].x;
        float uy = coordinates[u].y;
        float uz = coordinates[u].z;
        float uw = coordinates[u].w;
        for (int v = u+1; v < n; v++){
            float vx = coordinates[v].x;
            float vy = coordinates[v].y;
            float vz = coordinates[v].z;
            float vw = coordinates[u].w;
            float euclidWeight = sqrt((vx-ux)*(vx-ux)+(vy-uy)*(vy-uy)+(vz-uz)*(vz-uz)+(vw-uw)*(vw-uw));
            addEdge(adj, u, v, euclidWeight);
        }
    }

}

void generateHyperUnit(Graph &adj, float dimen){
    srand(time(0));
    if (dimen==3){
        generate3D(adj);
    } else if (dimen==4){
        generate4D(adj);
    } else {
        std::cout << "  ERROR: Please select a 3D or 4D graph.\n";
    }
}




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
    //generateComplete(graph);
    generateHyperUnit(graph,4);
   float mstWeight = kruskal(graph); 
    std::cout << "Minimum Spanning Tree Weight (Kruskal): " << mstWeight << std::endl;
    mstWeight = prim(graph); 
    std::cout << "Minimum Spanning Tree Weight (Prim): " << mstWeight << std::endl;
    displayAdjList(graph);
}
