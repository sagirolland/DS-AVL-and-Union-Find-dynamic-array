#ifndef WET1_AVLTREE_H
#define WET1_AVLTREE_H

#include <iostream>
#include <memory>
#include "DynamicArray.h"

class Song; 
class Genre; 

struct Node
{
    int songID;
    Song* songPtr;
    Genre* genrePtr;
    int height;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    Node(int k, Song *s, Genre *g)
        : songID(k), songPtr(s), genrePtr(g), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree
{
public:
    std::shared_ptr<Node> root;

    int height(std::shared_ptr<Node> node)
    {
        return node ? node->height : -1;
    }

    int balanceFactor(std::shared_ptr<Node> node)
    {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void updateHeight(std::shared_ptr<Node> node)
    {
        if (node)
        {
            node->height = 1 + std::max(height(node->left), height(node->right));
        }
    }

    std::shared_ptr<Node> rotateRight(std::shared_ptr<Node> y)
    {
        std::shared_ptr<Node> x = y->left;
        std::shared_ptr<Node> T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;

        return x;
    }

    std::shared_ptr<Node> rotateLeft(std::shared_ptr<Node> x)
    {
        std::shared_ptr<Node> y = x->right;
        std::shared_ptr<Node> T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;

        return y;
    }

    std::shared_ptr<Node> balance(std::shared_ptr<Node> node)
    {
        updateHeight(node);
        int bf = balanceFactor(node);
        if (bf > 1)
        {
            if (balanceFactor(node->left) < 0)
            {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        if (bf < -1)
        {
            if (balanceFactor(node->right) > 0)
            {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        return node;
    }

    std::shared_ptr<Node> insert(std::shared_ptr<Node> node, int songID, Song *songPtr, Genre *genrePtr)
    {
        if (!node)
        {
            return std::make_shared<Node>(songID, songPtr, genrePtr);
        }
        if (songID < node->songID)
        {
            node->left = insert(node->left, songID, songPtr, genrePtr);
        }
        else if (songID > node->songID)
        {
            node->right = insert(node->right, songID, songPtr, genrePtr);
        }
        node->height = 1 + std::max(height(node->left), height(node->right));
        int balances = balanceFactor(node);

        return balance(node);
    }
    std::shared_ptr<Node> minValueNode(std::shared_ptr<Node> node)
    {
        std::shared_ptr<Node> current = node;
        while (current->left != nullptr)
        {
            current = current->left;
        }
        return current;
    }

    std::shared_ptr<Node> maxValueNode(std::shared_ptr<Node> node)
    {
        std::shared_ptr<Node> current = node;
        if (!current)
        {
            return current;
        }
        while (current->right != nullptr)
        {
            current = current->right;
        }
        return current;
    }

    std::shared_ptr<Node> remove(std::shared_ptr<Node> root, int songID)
    {
        if (!root)
        {
            return root;
        }
        if (songID < root->songID)
        {
            root->left = remove(root->left, songID);
        }
        else if (songID > root->songID)
        {
            root->right = remove(root->right, songID);
        }
        else
        {
            if (!root->left || !root->right)
            {
                std::shared_ptr<Node> temp = root->left ? root->left : root->right;
                if (!temp)
                {
                    temp = root;
                    root = nullptr;
                }
                else
                {
                    root = temp;
                }
            }
            else
            {
                std::shared_ptr<Node> temp = minValueNode(root->right);
                root->songID = temp->songID;
                root->songPtr = temp->songPtr;
                root->genrePtr = temp->genrePtr;
                root->right = remove(root->right, temp->songID);
            }
        }
        if (!root)
        {
            return root;
        }
        return balance(root);
    }

    std::shared_ptr<Node> search(std::shared_ptr<Node> node, int songID)
    {
        if (!node || node->songID == songID)
        {
            return node;
        }
        if (songID < node->songID)
        {
            return search(node->left, songID);
        }
        return search(node->right, songID);
    }

    void clearTree(std::shared_ptr<Node> &node)
    {
        if (node)
        {
            clearTree(node->left);
            clearTree(node->right);
            node = nullptr; // This will decrement the reference count and delete the node if it was the last reference
        }
    }

    void rehashTreeElements(std::shared_ptr<Node> node, DynamicArray<int> &elements)
    {
        if (node != nullptr)
        {
            rehashTreeElements(node->left, elements);
            elements.push_back(node->songID);
            rehashTreeElements(node->right, elements);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree()
    {
        clear();
    }

    std::shared_ptr<Node> getRoot()
    {
        return root;
    }

    std::shared_ptr<Node> getLeft(std::shared_ptr<Node> node)
    {
        return node ? node->left : nullptr;
    }

    std::shared_ptr<Node> getRight(std::shared_ptr<Node> node)
    {
        return node ? node->right : nullptr;
    }

    void setLeft(std::shared_ptr<Node> parent, std::shared_ptr<Node> leftChild)
    {
        if (parent)
        {
            parent->left = leftChild;
        }
    }

    void setRight(std::shared_ptr<Node> parent, std::shared_ptr<Node> rightChild)
    {
        if (parent)
        {
            parent->right = rightChild;
        }
    }

    int getsongID(std::shared_ptr<Node> node)
    {
        return node ? node->songID : 0;
    }

    void setsongID(std::shared_ptr<Node> node, int songID)
    {
        if (node)
        {
            node->songID = songID;
        }
    }

    void insert(int songID, Song *songPtr, Genre *genrePtr)
    {
        root = insert(root, songID, songPtr, genrePtr);
    }

    void remove(int songID)
    {
        root = remove(root, songID);
    }



    bool isEmpty() const
    {
        return root == nullptr;
    }

    void clear()
    {
        clearTree(root);
    }

    void collectElements(DynamicArray<int> &elements)
    {
        rehashTreeElements(root, elements);
    }
};

#endif