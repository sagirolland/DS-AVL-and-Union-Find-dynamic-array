#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include <iostream>
#include "DynamicArray.h"

class UnionFind {
private:
    DynamicArray<int> parent;
    DynamicArray<int> size;
    DynamicArray<int> num_merges;
    DynamicArray<int> roots;
    DynamicArray<int> ndx_genre;

public:
    UnionFind() = default;

    int makeSet() {
        int i = parent.size();
        parent.push_back(i); 
        size.push_back(1);
        num_merges.push_back(0);
        roots.push_back(0);
        ndx_genre.push_back(-1);
        return i;
    }

    int find(int i) {
        if(i < 0) return -1;
        if(parent[i] == i)
            return i;
        int res = find(parent[i]); 
        num_merges[i] += num_merges[parent[i]];
        parent[i] = res;
        return res;
    }

    int Union(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == -1 && rootY == -1) 
            return -1;

        if (rootX == -1) {
            roots[rootY]++;
            return rootY;
        }
        if (rootY == -1) {
            roots[rootX]++;
            return rootX;
        }

        if (size[rootX] < size[rootY]) {
            int temp = rootY;
            rootY = rootX;
            rootX = temp;
        }
        parent[rootY] = rootX;
        size[rootX] += size[rootY];
        num_merges[rootY] = roots[rootY] - roots[rootX];
        //roots[rootX] += 1;
        return rootX;
    }

    int getNumChanges(int i) {
        int c = 1;
        while(parent[i] != i){
            c += num_merges[i];
            i = parent[i];
        }
        c += roots[i];
        return c;
    }

    void setGenreIndex(int index, int genreId) {
        if(index == -1) return;
        ndx_genre[index] = genreId;
    }

    int getGenre(int index) {
        int res = find(index);
        return ndx_genre[res];
    }
};

#endif // WET2_UNIONFIND_H