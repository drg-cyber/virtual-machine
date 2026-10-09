/*
 * File:        MyVector.h
 * Written by:  NAME:AHMED MOHAMED DAFAALLA AHMED   ID:253UC255PR
 * Description: Custom dynamic array template used in place of std::vector.
 */

#pragma once
#include <iostream>
#include "MyException.h"

// our own version of vector since we can't use it
// works with any type V just like the normal vector would
template <typename V>
class MyVector
{
    V *v_arr;       // pointer to the actual array on the heap
    int v_size;     // how many elements are actually being used
    int v_capacity; // how much space we've allocated so far

    // makes a bigger array and copies everything over, then frees the old one
    // resize() is private because we only use it inside the class
    void resize(int new_capacity)
    {
        V *new_dyarry = new V[new_capacity];
        for (int i = 0; i < v_size; i++)
            new_dyarry[i] = v_arr[i];

        delete[] v_arr;
        v_arr = new_dyarry;
        v_capacity = new_capacity;
    }

public:
    // start with capacity 1 so we're not allocating 0-size arrays
    // the capacity and size should never be equal
    MyVector()
    {
        this->v_size = 0;
        this->v_capacity = 1;
        v_arr = new V[v_capacity];
    }

    // destructor to free up the memory after use
    ~MyVector()
    {
        delete[] v_arr;
    }

    // Copy constructor — deep copy
    MyVector(const MyVector &other)
    {
        v_size = other.v_size;
        v_capacity = other.v_capacity;
        v_arr = new V[v_capacity];
        for (int i = 0; i < v_size; i++)
            v_arr[i] = other.v_arr[i];
    }

    // Copy assignment — deep copy, with self-assignment and old-memory cleanup handled
    MyVector &operator=(const MyVector &other)
    {
        if (this != &other)
        {
            delete[] v_arr;
            v_size = other.v_size;
            v_capacity = other.v_capacity;
            v_arr = new V[v_capacity];
            for (int i = 0; i < v_size; i++)
                v_arr[i] = other.v_arr[i];
        }
        return *this;
    }

    // only needed if V is a pointer type, deletes what each element points to
    // and because the normal destructor can delete those elements
    void deleteAll()
    {
        for (int i = 0; i < v_size; i++)
            delete v_arr[i];
        v_size = 0;
    }

    int size() const { return v_size; }

    int capacity() const { return v_capacity; }

    bool empty() { return v_size == 0; }

    // just prints out everything comma separated, mainly for debugging
    void printVec()
    {
        for (int i = 0; i < v_size; i++)
        {
            std::cout << v_arr[i];
            if (i == v_size - 1)
                std::cout << std::endl;
            else
                std::cout << ", ";
        }
    }

    // adds to the end, doubles capacity if we run out of space (amortized growth)
    void push_back(V val)
    {
        if (v_size == v_capacity)
        {
            int new_capacity = v_capacity * 2; // passes the new capacity to the resize method
            resize(new_capacity);
        }

        v_arr[v_size++] = val;
    }

    // just shrinks the size, doesn't actually erase or free anything
    void pop_back()
    {
        if (v_size > 0)
            --v_size;
    }

    // access with bounds checking, throws if index is out of range
    V &operator[](int index)
    {
        if (index < 0 || index >= v_size)
            throw MyException("INDEX OUT OF RANGE!");
        return v_arr[index];
    }

    // const version so we can use [] on const MyVector objects too
    const V &operator[](int index) const
    {
        if (index < 0 || index >= v_size)
            throw MyException("INDEX OUT OF RANGE!");
        return v_arr[index];
    }
};