#include <chrono>
#include <vector>
#include <iostream>
#include "mst.h"
#include <math.h>
#include <cstdlib>

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

// Experiment Functions
void runExperiment(int numpoints, int numtrials, int dimension, int algo) {

    float totalWeight = 0.0;

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numtrials; i++) {
        Graph graph(numpoints);
        generateGraph(graph, numpoints, dimension);
        if (algo == 0){
            totalWeight += prim(graph);
        }
        if (algo == 1){
            totalWeight += kruskal(graph);
        }
    }

    // End timer
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    float avgWeight = totalWeight / numtrials;
    std::cout << "n=" << numpoints << " trials=" << numtrials
              << " dimension=" << dimension 
              << " avg_weight=" << avgWeight 
              << " time_taken=" << elapsed.count() << " seconds" << std::endl;
}

void completeExperiment(int numtrials, int dimension) {
    std::vector<int> complete_graph_sizes = {128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
    for (int n : complete_graph_sizes) {
        // Run prim's algorithm
        runExperiment(n, numtrials, dimension, 1); 
    }
}

void hypercubeExperiment(int numtrials) {
    std::vector<int> hypercube_sizes = {128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144};
    for (int n : hypercube_sizes) {
        // Run kruskal
        runExperiment(n, numtrials, 1, 1); 
    }
}

// Compile with make then ./randmst 0 numpoints numtrials dimension
// To run experiments ./randmst 1 numtrials dimension

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: ./randmst <mode> <numpoints> <numtrials> <dimension>" << std::endl;
        return 1;
    }

    int mode = std::atoi(argv[1]);
    // Experiments
    if (mode == 1) {
        if (argc != 4) {
            std::cerr << "Usage: ./randmst 1 <numtrials> <dimension>" << std::endl;
            return 1;
        }
        
        int numtrials = std::atoi(argv[2]);
        int dimension = std::atoi(argv[3]);
        
        if (dimension == 1) {
            std::cout << "Running hypercube graph experiments..." << std::endl;
            hypercubeExperiment(numtrials);
        } else if (dimension >= 0 && dimension <= 4) {
            std::cout << "Running complete graph experiments..." << std::endl;
            completeExperiment(numtrials, dimension);
        } else{
            std::cerr << "Invalid dimension! Use 0-4" << std::endl;
            return 1;
        }
    // MST calculation  
    } else if (mode == 0) {
        if (argc != 5) {
            std::cerr << "Usage: ./randmst 0 <numpoints> <numtrials> <dimension>" << std::endl;
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
            //displayAdjList(graph);
            kruskalWeight += kruskal(graph);
        }
        std::cout << (kruskalWeight / numtrials) << " " << numpoints << " " <<  numtrials << " " << dimension << std::endl;
        std::fflush;

    } else {
        std::cerr << "Invalid mode! Use 0 for MST calculation or 1 for experiments." << std::endl;
        return 1;
    }

    return 0;
}
