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
    
    int Union(int genreid1 , int genreid2)
    {
        int genreIndex1 = findGenreIndex(genreid1);
        int genreIndex2 = findGenreIndex(genreid2);
        if (genreIndex1 == -1 || genreIndex2 == -1 || genreIndex1 == genreIndex2)
        {
            return -1; // Invalid genres or same genre
        }
        
        Genre& genre1 = genres[genreIndex1];
        Genre& genre2 = genres[genreIndex2];

        // Merge the two genres
        for (int i = 0; i < songs.size(); i++)
        {
            if (songs[i].findGenreNode && songs[i].findGenreNode->genrePtr->getGenreID() == genreid2)
            {
                songs[i].findGenreNode->genrePtr = &genre1; // Update the genre pointer
                songs[i].numberofgenrechanges++;
            }
        }
        
        // Update the AVL tree node
        if (genre1.avlNode && genre2.avlNode)
        {
            genre1.avlNode->left = genre2.avlNode;
            updateGenrePtrInTree(genre1.avlNode, &genre1);
            genre1.size += genre2.size;
            genre2.avlNode.reset();
        }
        for (int i = 0; i < songs.size(); i++)
        {
            if (songs[i].findGenreNode && songs[i].findGenreNode->genrePtr &&
                (songs[i].findGenreNode->genrePtr->getGenreID() == genreid1 ||
                 songs[i].findGenreNode->genrePtr->getGenreID() == genreid2))
            {
                songs[i].findGenreNode = genre1.avlNode;
                songs[i].avlNode = genre1.avlNode;
                songs[i].numberofgenrechanges++;
            }
        }
        return 0;
    }
    
    
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
    void updateGenrePtrInTree(const std::shared_ptr<Node> &node, Genre *newGenre)
    {
        if (!node)
            return;
        node->genrePtr = newGenre;
        updateGenrePtrInTree(node->left, newGenre);
        updateGenrePtrInTree(node->right, newGenre);
    }
};

#endif // WET2_UNIONFIND_H