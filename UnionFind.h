#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include <iostream>
#include "DynamicArray.h"

class UnionFind {
private:
    DynamicArray<int> size;
   
    DynamicArray<int> roots;
    DynamicArray<int> ndx_genre;

public:
    DynamicArray<int> parent;

    DynamicArray<int> num_merges;
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

    int find(int i)
    {
        if (i < 0)
            return -1;
        if (parent[i] == i)
            return i;
        int orig_parent = parent[i];
        parent[i] = find(parent[i]);
        num_merges[i] += num_merges[orig_parent]; // accumulate changes up the path
        return parent[i];
    }

    int Union(int x, int y, bool forceRootY = false)
    {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == -1 && rootY == -1)
            return -1;
        if (rootX == -1)
            return rootY;
        if (rootY == -1)
            return rootX;
        if (rootX == rootY)
            return rootX;

        // Always attach one root to the other and set num_merges[child] = 1
        if (forceRootY)
        {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
            num_merges[rootX] = num_merges[rootY] + 1;
            return rootY;
        }

        if (size[rootX] < size[rootY])
        {
            std::swap(rootX, rootY);
        }
        parent[rootY] = rootX;
        size[rootX] += size[rootY];
        num_merges[rootY] = num_merges[rootX] + 1;
        return rootX;
    }

    void setNumMerges(int index, int value)
    {
        if (index >= 0)
            num_merges[index] = value;
    }
    int getNumChanges(int i)
    {
        find(i);
        return num_merges[i];
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