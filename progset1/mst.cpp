#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> 

using namespace std;



//union find

struct UnionFind {
    vector<int> parent, rank;

    // Constructor
    UnionFind(int size) {
        parent.resize(size);
        rank.resize(size, 0);
      
        // Initialize parent array
        for (int i = 0; i < size; i++) {
            parent[i] = i;
        }
    }


    // Find the representative (root) of the
    // set that includes element i
    int find(int i) {
      
        if (parent[i] != i) {
            parent[i] = find(parent[i]); 
        }
        return parent[i];
    }
    // Unite (merge) the set that includes element 
    // i and the set that includes element j
    void merge(int i, int j) {
      
        // Representative of set containing i
        int rootI = find(i);
      
        // Representative of set containing j
        int rootJ = find(j);

        if (rank[rootI] > rank[rootJ]){
            parent[rootJ ] = rootI;
        }
        else{
            parent[rootI] = rootJ;
        }

        if (rank[rootI] == rank[rootJ]){
            rank[rootJ ]++;
        }
    }
};

//Kruskal Algorithm

int Kruskal(int size, vector<vector<pair<int, int>>>& adj){
    //  Initialize result
    int mst_wt = 0; 

    // Create V single item sets
    UnionFind set(size);

    vector<pair<int, pair<int, int>>> edges;

    // Extract edges from adjacency list
    for (int u = 0; u < size; u++) {
        for (auto& [v, w] : adj[u]) {
            if (u < v) // Avoid duplicate edges (since it's undirected)
                edges.push_back({w, {u, v}});
        }
    }

    //Sort all edges into non decreasing order by weight w
    sort(edges.begin(), edges.end());

    // iterate through the edges
    for (auto& edge : edges) {
        int weight = edge.first;
        int u = edge.second.first;
        int v = edge.second.second;

        int set_u = set.find(u);
        int set_v = set.find(v);

        // If u and v are in different sets, add edge to MST
        if (set_u != set_v) { 
            mst_wt += weight;
            set.merge(u, v);
        }
    }

    return mst_wt;

}

// Prims
int prim(int V_size, int E_size,  vector<vector<pair<int, int>>>& adj){

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    
    // keep track of visited vertices
    vector<bool> visited(V_size, false);

    // variable to store the result
    int res = 0;

    pq.push({0,0});

    // Perform Prim's algorithm to find the Minimum Spanning Tree
    while(!pq.empty()){
        auto p = pq.top();
        pq.pop();
        
        int wt = p.first;  // Weight of the edge
        int u = p.second;  // Vertex connected to the edge
        
        if(visited[u] == true){
            continue;  
        }
        
        res += wt; 
        visited[u] = true;  
        
        // Explore the adjacent vertices
        for(auto v : adj[u]){
            int vertex = v.first;
            int weight = v.second;
            if(!visited[vertex]){
                pq.push({weight, vertex});
            }
        }
    }
    return res; 

}

int main(){
    return 0;
}