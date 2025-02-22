#include <iostream>
#include <vector>
#include <cassert>
#include "mst.h"  

using namespace std;

// Test inserting and extracting elements
void testInsertAndExtract() {
    Heap heap;
    heap.insert({5.0, 1});
    heap.insert({3.0, 2});
    heap.insert({8.0, 3});

    assert(heap.extractMin() == (w_pair{3.0, 2}));
    assert(heap.extractMin() == (w_pair{5.0, 1}));
    assert(heap.extractMin() == (w_pair{8.0, 3}));
}

// Test extracting from an empty heap
void testExtractFromEmptyHeap() {
    Heap heap;
    if (!heap.empty()) {
        heap.extractMin();  // Should work
    }
    
    // Make sure the heap is empty now
    assert(heap.empty());
}

// Test peeking at the minimum element
void testPeek() {
    Heap heap;
    heap.insert({10.0, 1});
    heap.insert({15.0, 2});
    heap.insert({5.0, 3});

    assert(heap.peek() == (w_pair{5.0, 3}));  // Smallest element should be at the top

    heap.extractMin();  // Remove 5.0
    assert(heap.peek() == (w_pair{10.0, 1}));  // Next min should be 10.0
}

// Test inserting multiple elements and checking order
void testInsertMultiple() {
    Heap heap;
    heap.insert({20.0, 1});
    heap.insert({10.0, 2});
    heap.insert({30.0, 3});
    heap.insert({5.0, 4});
    heap.insert({15.0, 5});

    assert(heap.extractMin() == (w_pair{5.0, 4}));
    assert(heap.extractMin() == (w_pair{10.0, 2}));
    assert(heap.extractMin() == (w_pair{15.0, 5}));
    assert(heap.extractMin() == (w_pair{20.0, 1}));
    assert(heap.extractMin() == (w_pair{30.0, 3}));
}

// Test that heap maintains its min-heap property
void testHeapProperty() {
    Heap heap;
    heap.insert({3.0, 1});
    heap.insert({1.0, 2});
    heap.insert({6.0, 3});
    heap.insert({5.0, 4});
    heap.insert({2.0, 5});
    
    std::vector<w_pair> sorted;
    while (!heap.empty()) {
        sorted.push_back(heap.extractMin());
    }

    for (size_t i = 1; i < sorted.size(); i++) {
        assert(sorted[i - 1].first <= sorted[i].first);  // Ensure sorted order
    }
}

// Test inserting and extracting elements in between
void testMultipleInsertAndExtract() {
    Heap heap;
    heap.insert({9.0, 1});
    heap.insert({5.0, 2});
    heap.insert({7.0, 3});
    heap.insert({1.0, 4});

    assert(heap.extractMin() == (w_pair{1.0, 4}));
    heap.insert({2.0, 5});
    assert(heap.extractMin() == (w_pair{2.0, 5}));
    assert(heap.extractMin() == (w_pair{5.0, 2}));
    assert(heap.extractMin() == (w_pair{7.0, 3}));
    assert(heap.extractMin() == (w_pair{9.0, 1}));
}
// Utility function to create a simple test graph
Graph createGraph(int V, vector<tuple<int, int, float>> edges) {
    Graph adj(V);
    for (auto [u, v, w] : edges) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});  // Since the graph is undirected
    }
    return adj;
}

// Test Kruskal's and Prim's algorithms on a small graph
void testSmallGraph() {
    Graph adj = createGraph(4, {
        {0, 1, 1.0}, {1, 2, 2.0}, {0, 2, 2.5}, {2, 3, 1.5}
    });

    float kruskalResult = kruskal(adj);
    float primResult = prim(adj);

    assert(kruskalResult == 4.5);
    assert(primResult == 4.5);
}

// Test a single-node graph (MST weight should be 0)
void testSingleNodeGraph() {
    Graph adj = createGraph(1, {});  // One node, no edges

    float kruskalResult = kruskal(adj);
    float primResult = prim(adj);

    assert(kruskalResult == 0);
    assert(primResult == 0);
}

// Test a cycle graph to ensure algorithms correctly avoid cycles
void testCycleGraph() {
    Graph adj = createGraph(4, {
        {0, 1, 1.0}, {1, 2, 2.0}, {2, 3, 1.5}, {3, 0, 2.5}
    });

    float kruskalResult = kruskal(adj);
    float primResult = prim(adj);

    assert(kruskalResult == 4.5);
    assert(primResult == 4.5);
}

// Test a fully connected graph
void testFullyConnectedGraph() {
    Graph adj = createGraph(4, {
        {0, 1, 1.0}, {0, 2, 3.0}, {0, 3, 4.0},
        {1, 2, 2.0}, {1, 3, 5.0}, {2, 3, 1.5}
    });

    float kruskalResult = kruskal(adj);
    float primResult = prim(adj);

    assert(kruskalResult == 4.5);
    assert(primResult == 4.5);
}


int main() {
    testInsertAndExtract();
    testExtractFromEmptyHeap();
    testPeek();
    testInsertMultiple();
    testHeapProperty();
    testMultipleInsertAndExtract();
    
    std::cout << "Heap tests passed!" << std::endl;

    testSmallGraph();
    testSingleNodeGraph();
    testCycleGraph();
    testFullyConnectedGraph();

    cout << "All MST tests passed!" << endl;
    return 0;

}
