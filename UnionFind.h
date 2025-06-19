#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include <iostream>
#include "DynamicArray.h"
#include "AVLTree.h"

class Node; 

class Song
{
    public:
    int songID;
    int parent; 
    int numberofgenrechanges = 1;
    std::shared_ptr<Node> avlNode; // Pointer to the AVL node
    std::shared_ptr<Node> findGenreNode; // Pointer to the genre node in AVL tree
    Song(int songid) : songID(songid), parent(-1),numberofgenrechanges(1),  avlNode(nullptr), findGenreNode(nullptr) {}
    Song() = default; // Use default constructor
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
    Genre() = default; // Use default constructor

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
public:
    DynamicArray<Song> songs;
    DynamicArray<Genre> genres;

    UnionFind() {}
    bool Makeset(int songid, int genreid)
    {   
        Genre* gen = findobjectGenre(genreid);
        if (gen == nullptr){return false;}
        songs.push_back(Song(songid));
        auto avlNode = std::make_shared<Node>(songid, &songs[songs.size() - 1], gen);
        songs[songs.size() - 1].avlNode = avlNode;
        gen->avlNode = avlNode;
        songs[songs.size() - 1].findGenreNode = avlNode;
        songs[songs.size() - 1].parent = songs.size() - 1;
        gen->incrementSize();
        return true; // Successfully added song
    }
    int Find(int songId)
    {
        int songind = findSongIndex(songId);
        if (songind == -1)
        {
            return -1; // Song not found
        }
        if (songs[songind].parent != songind)
        {
            songs[songind].parent = Find(songs[songs[songind].parent].songID); // recuesive call to the index of the parent song
        }
        return songs[songind].parent; // Return the root of the set
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

    int findGenreIndex(int genreId)
    {
        for (int i = 0; i < genres.size(); i++)
        {
            if (genres[i].getGenreID() == genreId)
            {
                return i; // Genre already exists
            }
        }
        return -1;
    }
    Genre* findobjectGenre(int genreId)
    {
        for (int i = 0; i < genres.size(); i++)
        {
            if (genres[i].getGenreID() == genreId)
            {
                return &genres[i]; // Genre already exists
            }
        }
        return nullptr;
    }
    int findSongIndex(int songId)
    {
        for (int i = 0; i < songs.size(); i++)
        {
            if (songs[i].getSongID() == songId)
            {
                return i; 
            }
        }
        return -1;
    }
    Song* findobjectSong(int songId)
    {
        for (int i = 0; i < songs.size(); i++)
        {
            if (songs[i].getSongID() == songId)
            {
                return &songs[i]; // Song already exists
            }
        }
        return nullptr;
    }
};

#endif // WET2_UNIONFIND_H