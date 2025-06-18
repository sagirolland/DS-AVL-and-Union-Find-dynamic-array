#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H

#include <iostream>
#include "DynamicArray.h"
#include "AVLTree.h"

class HashTable
{
private:
    std::shared_ptr<AVLTree[]> table;
    int table_size;
    int num_elements;

    float loadFactor() const
    {
        return static_cast<float>(num_elements) / table_size;
    }

    void resizeAndRehash()
    {
        int new_size = table_size * 2;
        auto new_table = std::shared_ptr<AVLTree[]>(new AVLTree[new_size], std::default_delete<AVLTree[]>());

        for (int i = 0; i < table_size; ++i)
        {
            DynamicArray<int> elements;
            table.get()[i].collectElements(elements);
            for (int j = 0; j < elements.size(); j++)
            {
                int hashValue = elements[j] % new_size;
                new_table.get()[hashValue].insert(elements[j], table.get()[i].search(elements[j]));
            }
        }

        table = new_table;
        table_size = new_size;
    }

public:
    HashTable(int initial_size = 10) : table_size(initial_size), num_elements(0)
    {
        table = std::shared_ptr<AVLTree[]>(new AVLTree[table_size], std::default_delete<AVLTree[]>());
    }

    void insertItem(int songID, int value)
    {
        if (loadFactor() > 1)
        {
            resizeAndRehash();
        }
        int hashValue = songID % table_size;
        table.get()[hashValue].insert(songID, value);
        num_elements++;
    }

    void removeItem(int songID)
    {
        int hashValue = songID % table_size;
        if (table.get()[hashValue].search(songID))
        {
            table.get()[hashValue].remove(songID);
            num_elements--;
        }
    }

    int findItem(int songID) const
    {
        int hashValue = songID % table_size;
        return table.get()[hashValue].search(songID);
    }

    int hash(int songID) const
    {
        return songID % table_size;
    }
};

#endif // WET2_HASHTABLE_H