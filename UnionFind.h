#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include <iostream>
#include "DynamicArray.h"

class UnionFind {
private:
    DynamicArray<int> parent;
    DynamicArray<int> size;
    int count;

public:
    UnionFind() : count(0) {} //(?)

    void Makeset(int element) {
        parent.push_back(element);
        size.push_back(0);
    }

    int Find(int element) {
        if (parent[element] != element) {
            parent[element] = Find(parent[element]);
        }
        return parent[element];
    }

    void Union(int x, int y) {
        int rootX = Find(x);
        int rootY = Find(y);

        parent[rootY] = rootX;
        size[rootX] += size[rootY];
    } //edited Union to add y to x.

    bool connected(int x, int y) {
        return Find(x) == Find(y);
    }
};

#endif // WET2_UNIONFIND_H