#include <chrono>
#include <vector>
#include <iostream>
#include "mst.h"
#include <math.h>

void addEdge(Graph &adj, int u, int v, float w){
    adj[u].push_back({v,w});
    adj[v].push_back({u,w}); // because undirected
}

//graph type #1
void generateComplete(Graph &adj, int n){
    srand(time(0));
    for (int u = 0; u < n; u++){
        for (int v = u+1; v < n; v++){
            float newWeight = (float)rand() / RAND_MAX;
            addEdge(adj, u, v, newWeight);
        }
    }
}

//graph type #2
void generateHypercube(Graph &adj, int n){
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
void generateUnitSquare(Graph &adj, int n){
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

void generate3D(Graph &adj, int n){
    srand(time(0));
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

void generate4D(Graph &adj, int n){
    srand(time(0));
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
            float vw = coordinates[v].w;
            float euclidWeight = sqrt((vx-ux)*(vx-ux)+(vy-uy)*(vy-uy)+(vz-uz)*(vz-uz)+(vw-uw)*(vw-uw));
            addEdge(adj, u, v, euclidWeight);
        }
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

void generateGraph(Graph &adj, int n, int dimension){
    if (dimension == 0){
        generateComplete(adj, n);
    } else if (dimension == 1){
        generateHypercube(adj, n);
    } else if (dimension == 2){
        generateUnitSquare(adj, n);
    } else if (dimension == 3){
        generate3D(adj, n);
    } else if (dimension == 4){
        generate4D(adj, n);
    } else {
        std::cerr << "ERROR: Invalid dimension." << std::endl;
        exit(1);
    }
}

// Compile with make then ./randmst 0 numpoints numtrials dimension

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: ./randmst 0 numpoints numtrials dimension" << std::endl;
        return 1;
    }

    int numpoints = std::atoi(argv[2]);
    int numtrials = std::atoi(argv[3]);
    int dimension = std::atoi(argv[4]); 

  float kruskalWeight = 0.0;
  float primWeight = 0.0;
  for (int i = 0; i < numtrials; i++) {
      Graph graph(numpoints);
      generateGraph(graph, numpoints, dimension);
      kruskalWeight += kruskal(graph);
      primWeight += prim(graph);
  }

  std::cout << "Average MST Weight using Kruskal: " << (kruskalWeight / numtrials) << "\n"
            << "Average MST Weight using Prim: " << (primWeight / numtrials) << "\n"
            << "Number of Points: " << numpoints << "\n"
            << "Number of Trials: " << numtrials << "\n"
            << "Graph Dimension: " << dimension << std::endl;
  return 0;
}
