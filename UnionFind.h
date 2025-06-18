#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include <iostream>
#include "DynamicArray.h"

class Song
{
    public:
    int songID;
    int parent; 
    int numberofgenrechanges = 1;
    std::shared_ptr<Node> avlNode; // Pointer to the AVL node

    Song(int songid) : songID(songid), parent(songid) {}
    virtual ~Song()= default; // Use default destructor
    int getSongID() const
    {
        return songID;
    }
    int getNumberOfGenreChanges() const
    {
        return numberofgenrechanges;
    }
};

class Genre
{
    public:
    int genreID;
    int size  = 0 ;
    std::shared_ptr<Node> avlNode = nullptr; // Pointer to the AVL node
    Genre(int genreid) : genreID(genreid) {}
    virtual ~Genre() = default; // Use default destructor
    int getGenreID() const
    {
        return genreID;
    }
    int getSize() const
    {
        return size;
    }
    void incrementSize()
    {
        size++;
    }
};

class UnionFind
{
private:
    DynamicArray<Song> songs;
    DynamicArray<Genre> genres;
    int count;

public:
    UnionFind() : count(0) {} //(?)
    void Makeset(int songid, int genreid)
    {
        songs.push_back(Song(songid));
        genres.push_back(Genre(genreid));
        // Create AVL node for the song
        auto avlNode = std::make_shared<Node>(songid, &songs[songs.size() - 1], &genres[genres.size() - 1]);
        songs[songs.size() - 1].avlNode = avlNode;
        genres[genres.size() - 1].avlNode = nullptr; // Genre points to the root of its AVL tree
        songs[songs.size() - 1].parent = songs.size() - 1;
        genres[genres.size() - 1].size = 1;
        count++;
    }
    int Find(int songIdx)
    {
        if (songs[songIdx].parent != songIdx)
        {
            songs[songIdx].parent = Find(songs[songIdx].parent);
        }
        return songs[songIdx].parent;
    }
    // int Find(int element)
    // {
    //     if (songs[element].getSongID() != element)
    //     {
    //         songs[element].songID = Find(songs[element].getSongID());
    //     }
    //     return songs[element].getSongID();
    // }
    void Union(int xIdx, int yIdx)
    {
        int rootX = Find(xIdx);
        int rootY = Find(yIdx);
        if (rootX == rootY)
            return;
        // Union by size/rank if you want
        songs[rootY].parent = rootX;
        genres[rootX].size += genres[rootY].size;
        // Point the genre's avlNode to the new root (for now, use rootX's node)
        genres[rootX].avlNode = songs[rootX].avlNode;
    }

    // void Union(int x, int y)
    // {
    //     int rootX = Find(x);
    //     int rootY = Find(y);

    //     songs[rootY] = rootX;
    //     genres[rootX].size += genres[rootY].size;
    // } // edited Union to add y to x.

    bool connected(int x, int y)
    {
        return Find(x) == Find(y);
    }
};

#endif // WET2_UNIONFIND_H