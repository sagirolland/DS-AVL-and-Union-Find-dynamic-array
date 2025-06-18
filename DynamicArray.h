#ifndef WET2_DYNAMICARRAY_H
#define WET2_DYNAMICARRAY_H

#include <iostream>
#include <memory>

template <typename T>
class DynamicArray
{
public:
    std::shared_ptr<T[]> data;
    int capacity;
    int length;
    void regeners()
    {
        capacity = capacity == 0 ? 1 : capacity * 2;
        std::shared_ptr<T[]> newData(new T[capacity], std::default_delete<T[]>());
        for (int i = 0; i < length; ++i)
        {
            newData[i] = data[i];
        }
        data = newData;
    }

    
    DynamicArray() : capacity(0), length(0) {}

    void push_back(T value)
    {
        if (length == capacity)
        {
            regeners();
        }
        data[length] = value;
        

        length++;
    }

    void remove(int index)
    {
        if (index >= length)
        {
            return;
        }
        for (int i = index; i < length - 1; ++i)
        {
            data[i] = data[i + 1];
        }
        --length;
    }

    T &operator[](int index)
    {
        if (index < length)
        {
            return data[index];
        }
        return data[length - 1];
    }

    int size() const
    {
        return length;
    }
};

#endif // WET2_DYNAMICARRAY_H