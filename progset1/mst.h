#ifndef PG1_H
#define PG1_H
typedef std::vector<std::vector<std::pair<int, float>>> Graph;
// weight, vertex pair
using w_pair = std::pair<float, int>;
#include <vector>
using namespace std;

struct UnionFind {
    std::vector<int> parent, rank;
    // Constructor declaration
    UnionFind(int size);
    // Methods declaration
    int find(int i);
    void merge(int x, int y);
};

class Heap {
    private:

        // Helper function to maintain the heap property
        void heapify(int n);

    public:
        // Vector to store heap elements
        vector<w_pair> array;

        // Default constructor
        Heap() = default;

        // Check empty
        bool empty() const { return array.empty(); }

        // Return smallest element
        w_pair peek();

        // Remove smallest element
        w_pair extractMin();

        // Function to insert a new key into the heap
        void insert(w_pair key);

        void printHeap() const;
};


float kruskal(Graph &adj);
float prim(Graph &adj);

#endif
