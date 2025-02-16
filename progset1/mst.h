#ifndef PG1_H
#define PG1_H
typedef std::vector<std::vector<std::pair<int, float>>> Graph;

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

float kruskal(Graph &adj);
float prim(Graph &adj);

#endif
