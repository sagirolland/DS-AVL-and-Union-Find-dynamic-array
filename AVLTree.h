#ifndef WET1_AVLTREE_H
#define WET1_AVLTREE_H

#include <iostream>
#include <memory>
#include "DynamicArray.h"

template <typename K, typename V>
class AVLTree {
private:
    struct Node {
        K key;
        V value;
        int height;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        Node(K k, V v)
            : key(k), value(v), height(0), left(nullptr), right(nullptr) {}
    };

    std::shared_ptr<Node> root;

    int height(std::shared_ptr<Node> node) {
        return node ? node->height : -1;
    }

    int balanceFactor(std::shared_ptr<Node> node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void updateHeight(std::shared_ptr<Node> node) {
        if (node) {
            node->height = 1 + std::max(height(node->left), height(node->right));
        }
    }

    std::shared_ptr<Node> rotateRight(std::shared_ptr<Node> y) {
        std::shared_ptr<Node> x = y->left;
        std::shared_ptr<Node> T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    std::shared_ptr<Node> rotateLeft(std::shared_ptr<Node> x) {
        std::shared_ptr<Node> y = x->right;
        std::shared_ptr<Node> T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    std::shared_ptr<Node> balance(std::shared_ptr<Node> node) {
        updateHeight(node);
        int bf = balanceFactor(node);
        if (bf > 1) {
            if (balanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        if (bf < -1) {
            if (balanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        return node;
    }

    std::shared_ptr<Node> insert(std::shared_ptr<Node> node, K key, V value) {
        if (!node) {
            return std::make_shared<Node>(key, value);
        }
        if (key < node->key) {
            node->left = insert(node->left, key, value);
        } else if (key > node->key) {
            node->right = insert(node->right, key, value);
        } else {
            node->value = value; // Update value if key already exists
            return node;
        }
        return balance(node);
    }

    std::shared_ptr<Node> minValueNode(std::shared_ptr<Node> node) {
        std::shared_ptr<Node> current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    std::shared_ptr<Node> maxValueNode(std::shared_ptr<Node> node) {
        std::shared_ptr<Node> current = node;
        if(!current){
            return current;
        }
        while (current->right != nullptr) {
            current = current->right;
        }
        return current;
    }

    std::shared_ptr<Node> remove(std::shared_ptr<Node> root, K key) {
        if (!root) {
            return root;
        }
        if (key < root->key) {
            root->left = remove(root->left, key);
        } else if (key > root->key) {
            root->right = remove(root->right, key);
        } else {
            if (!root->left || !root->right) {
                std::shared_ptr<Node> temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else {
                    root = temp;
                }
            } else {
                std::shared_ptr<Node> temp = minValueNode(root->right);
                root->key = temp->key;
                root->value = temp->value;
                root->right = remove(root->right, temp->key);
            }
        }
        if (!root) {
            return root;
        }
        return balance(root);
    }

    std::shared_ptr<Node> search(std::shared_ptr<Node> node, K key) {
        if (!node || node->key == key) {
            return node;
        }
        if (key < node->key) {
            return search(node->left, key);
        }
        return search(node->right, key);
    }

    void clearTree(std::shared_ptr<Node>& node) {
        if (node) {
            clearTree(node->left);
            clearTree(node->right);
            node = nullptr; // This will decrement the reference count and delete the node if it was the last reference
        }
    }

    void printInOrderHelper(std::shared_ptr<Node> node) const {
        if (!node) {
            return;
        }
        printInOrderHelper(node->left);
        std::cout << node->key << ": " << node->value << std::endl;
        printInOrderHelper(node->right);
    }

    void rehashTreeElements(std::shared_ptr<Node> node, DynamicArray<K>& elements) {
        if (node != nullptr){
            rehashTreeElements(node->left, elements);
            elements.push_back(node->key);
            rehashTreeElements(node->right, elements);
        }
        
    }
    
public:
    AVLTree() : root(nullptr) {}
    
    ~AVLTree(){
        clear();
    }

    std::shared_ptr<Node> getRoot() {
        return root;
    }

    std::shared_ptr<Node> getLeft(std::shared_ptr<Node> node) {
        return node ? node->left : nullptr;
    }

    std::shared_ptr<Node> getRight(std::shared_ptr<Node> node) {
        return node ? node->right : nullptr;
    }

    void setLeft(std::shared_ptr<Node> parent, std::shared_ptr<Node> leftChild) {
        if (parent) {
            parent->left = leftChild;
        }
    }

    void setRight(std::shared_ptr<Node> parent, std::shared_ptr<Node> rightChild) {
        if (parent) {
            parent->right = rightChild;
        }
    }

    K getKey(std::shared_ptr<Node> node) {
        return node ? node->key : nullptr;
    }

    V getValue(std::shared_ptr<Node> node) {
        return node ? node->value : nullptr;
    }

    void setKey(std::shared_ptr<Node> node, K key) {
        if (node) {
            node->key = key;
        }
    }

    void setValue(std::shared_ptr<Node> node, V value) {
        if (node) {
            node->value = value;
        }
    }

    void insert(K key, V value) {
        root = insert(root, key, value);
    }

    void remove(K key) {
        root = remove(root, key);
    }

    V search(K key) {
        std::shared_ptr<Node> node = search(root, key);
        return node ? node->value : NULL;
    }

    K minKey () {
        return minValueNode(root)->key;
    }

    V minValue () {
        return minValueNode(root)->value;
    }

    V maxValue () {
        std::shared_ptr<Node> maxv = maxValueNode(root);
        return  maxv ? maxv->value : NULL;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    void clear() {
        clearTree(root);
    }

    void printInOrder() const {
        printInOrderHelper(root);
    }

    void collectElements(DynamicArray<K>& elements) {
        rehashTreeElements(root, elements);
    }

};



#endif