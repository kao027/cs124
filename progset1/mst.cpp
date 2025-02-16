#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> 
#include "mst.h"

// UnionFind methods
UnionFind::UnionFind(int size) {
    parent.resize(size);
    rank.resize(size, 0);
    for (int i = 0; i < size; i++) {
        parent[i] = i;
    }
}

int UnionFind::find(int i) {
    if (parent[i] != i) {
        parent[i] = find(parent[i]);
    }
    return parent[i];
}

void UnionFind::merge(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);

    if (rootX != rootY) {
        if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}



//Kruskal Algorithm

float kruskal(Graph& adj){
    //  Initialize result
    float mst_wt = 0; 

    int size = adj.size(); 
    // Create V single item sets
    UnionFind set(size);

    vector<pair<float, pair<int, int>>> edges;

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
        float weight = edge.first;
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
float prim(Graph& adj){

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    
    int size = adj.size(); 
    // keep track of visited vertices
    vector<bool> visited(size, false);

    // variable to store the result
    float res = 0;

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

