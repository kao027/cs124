#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> 
#include "mst.h"
#include <utility>

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


// Priority Queue
// Heapify is a helper function to mantain heap
void Heap::heapify(int n){
    int smallest = n;
    int left = 2 * n + 1;
    int right = 2 * n + 2;
    int size = array.size();
    if (left < size && array[left].first < array[smallest].first){
        smallest = left;
    }
    if (right < size && array[right].first < array[smallest].first){
        smallest = right;
    }
    if (smallest != n){
        //Swap
        w_pair temp = array[n];
        array[n] = array[smallest];
        array[smallest] = temp;
        heapify(smallest);
    }
}   

w_pair Heap::peek(){
    if (array.size() == 0) {
        throw std::out_of_range("Heap is empty");
    }
    return array.front();
}

w_pair Heap::extractMin(){
    size_t size = array.size();
    if (size <= 0){
        throw std::out_of_range("Empty Heap");
    }
    w_pair min = array[0];
    // replace root with last element of a vector
    array[0] = array[size - 1];
    array.pop_back();
    if (array.size() > 0) {
        heapify(0);
    }
    return min;
}

void Heap::insert(w_pair key){
    array.push_back(key);
    // Keep track of the index of v
    int n = array.size()-1;
    while (n != 0){
        int parent = (n-1) / 2;
        if (array[parent].first <= array[n].first) {
            break;
        }
        //Swap
        w_pair temp = array[parent];
        array[parent] = array[n];
        array[n] = temp;

        n = parent; 
    }
}

// Print the heap elements
void Heap::printHeap() const
{
    for (const auto& [weight, val] : array)
        cout << "(" << weight << ", " << val << ")";
    cout << endl;
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

    Heap pq;
    
    int size = adj.size(); 
    // keep track of visited vertices
    vector<bool> visited(size, false);

    // variable to store the result
    float res = 0;

    pq.insert({0,0});

    // Perform Prim's algorithm to find the Minimum Spanning Tree
    while(!pq.empty()){
        auto p = pq.peek();
        pq.extractMin();
        
        float wt = p.first;  // Weight of the edge
        int u = p.second;  // Vertex connected to the edge
        
        if(visited[u] == true){
            continue;  
        }
        
        res += wt; 
        visited[u] = true;  
        
        // Explore the adjacent vertices
        for(auto v : adj[u]){
            int vertex = v.first;
            float weight = v.second;
            if(!visited[vertex]){
                pq.insert({weight, vertex});
            }
        }
    }
    return res; 

}

