#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H

#include <iostream>
#include "DynamicArray.h"
#include "AVLTree.h"


template<typename K, typename V>
class HashTable {
private:
    std::shared_ptr<AVLTree<K, V>[]> table;
    int table_size;
    int num_elements;

    float loadFactor() const {
        return static_cast<float>(num_elements) / table_size;
    }

    void resizeAndRehash() {
        int new_size = table_size * 2;
        auto new_table = std::shared_ptr<AVLTree<K, V>[]>(new AVLTree<K, V>[new_size], std::default_delete<AVLTree<K, V>[]>());

        for (int i = 0; i < table_size; ++i) {
            DynamicArray<K> elements;
            table.get()[i].collectElements(elements);
            for (int j = 0; j < elements.size(); j++) {
                int hashValue = elements[j] % new_size;
                new_table.get()[hashValue].insert(elements[j], table.get()[i].search(elements[j]));
            }
        }

        table = new_table;
        table_size = new_size;
    }

public:
    HashTable(int initial_size = 10) : table_size(initial_size), num_elements(0) {
        table = std::shared_ptr<AVLTree<K, V>[]>(new AVLTree<K, V>[table_size], std::default_delete<AVLTree<K, V>[]>());
    }

    void insertItem(K key, V value) {
        if (loadFactor() > 1) {
            resizeAndRehash();
        }
        int hashValue = key % table_size;
        table.get()[hashValue].insert(key, value);
        num_elements++;
    }

    void removeItem(K key) {
        int hashValue = key % table_size;
        if (table.get()[hashValue].search(key)) {
            table.get()[hashValue].remove(key);
            num_elements--;
        }
    }

    V findItem(K key) const {
        int hashValue = key % table_size;
        return table.get()[hashValue].search(key);
    }

    int hash(K key) const{
        return key % table_size;
    }
};

#endif // WET2_HASHTABLE_H