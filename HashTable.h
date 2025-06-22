#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H

#include <iostream>
#include "DynamicArray.h"
#include "AVLTree.h"

template<typename K, typename V>
struct HashPair {
    K key;
    V value;

    HashPair() : key(), value() {}
    HashPair(K k, V v) : key(k), value(v) {}
};

template<typename K, typename V>
class HashTable {
private:
    using Bucket = DynamicArray< HashPair<K,V> >;

    std::shared_ptr<Bucket[]> table;
    //std::shared_ptr<AVLTree<K, V>[]> table;
    int table_size;
    int num_elements;

    float loadFactor() const {
        return static_cast<float>(num_elements) / table_size;
    }

    int hashFunc(K key, int mod) const {
        return key % mod;
    }

    static std::shared_ptr<Bucket[]> allocateBuckets(int size) {
        return std::shared_ptr<Bucket[]>(new Bucket[size], std::default_delete<Bucket[]>());
    }

    void resizeAndRehash() {
        int new_size = table_size * 2;
        std::shared_ptr<Bucket[]> new_table = allocateBuckets(new_size);

        for (int i = 0; i < table_size; ++i) {
            Bucket &oldB = table[i];
            for (int j = 0; j < oldB.size(); ++j) {
                const HashPair<K,V> &p = oldB[j];
                int idx = hashFunc(p.key, new_size);
                new_table[idx].push_back(HashPair<K,V>(p.key, p.value));
            }
        }

        table_size = new_size;
        table = std::move(new_table);
    }

public:
    HashTable(int initial_size = 10) : table(allocateBuckets(initial_size)), table_size(initial_size), num_elements(0) { }

    ~HashTable() = default;

    void insertItem(K key, V value) {
        if (loadFactor() > 1) {
            resizeAndRehash();
        }
        int idx = hashFunc(key, table_size);
        Bucket &b = table[idx];
        for (int i = 0; i < b.size(); ++i) {
            if (b[i].key == key) {
                b[i].value = value;
                return;
            }
        }
        b.push_back(HashPair<K,V>(key, value));
        ++num_elements;
    }

    void removeItem(K key) {
        int idx = hashFunc(key, table_size);
        Bucket &b = table[idx];
        for (int i = 0; i < b.size(); ++i) {
            if (b[i].key == key) {
                b.remove(i);
                --num_elements;
                return;
            }
        }
    }

    V findItem(K key) const {
        int idx = hashFunc(key, table_size);
        Bucket &b = table[idx];
        for (int i = 0; i < b.size(); ++i) {
            if (b[i].key == key) {
                return b[i].value;
            }
        }
        return NULL;
    }

    int bucketIndex(K key) const { return hashFunc(key, table_size); }

    int size() const { return num_elements; }
};

#endif // WET2_HASHTABLE_H