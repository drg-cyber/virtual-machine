/*
 * File:        MyStack.h
 * Written by:  NAME:AHMED MOHAMED DAFAALLA AHMED   ID:253UC255PR
 * Description: Custom fixed-capacity stack template used for the VM's
 *              stack (PUSH/POP) operations.
 */

#pragma once
#include <iostream>
#include "MyException.h"

// custom stack implementation
// you have to give it a fixed size at first
template <typename S>
class MyStack
{
    S *s_arr;   // the underlying array used to hold stack elements
    int s_size;  // max size of the stack
    int s_top;   // index of the top element, -1 means empty

    
public:
    // no default constructor, size must be given when creating a MyStack
    MyStack(int s_size)
    {
        this->s_top = -1; //-1 means empty
        this->s_size = s_size;
        s_arr = new S[s_size];
    }

    ~MyStack() // free up space
    {
        delete[] s_arr;
    }

    bool isemptyS() const { return s_top == -1; }

    bool isfullS() const { return s_top == s_size - 1; }

    int size() const { return s_top + 1; }

    // push increments top first then inserts, standard array stack logic
    void push(S val)
    {
        if (isfullS())
            throw MyException("STACK IS FULL!");
        else
        {
            s_arr[++s_top] = val;
        }
    }

    // pop returns the value then decrements top
    S pop()
    {
        if (isemptyS())
            throw MyException("STACK IS EMPTY!");
        else
            return s_arr[s_top--];
    }

    // just look at the top value without removing it
    S showtop()
    {
        if (isemptyS())
            throw MyException("STACK IS EMPTY!");
        else
            return s_arr[s_top];
    }

    // debug print
    void printStack()
    {
        if (isemptyS())
            std::cout << "STACK IS EMPTY!\n";
        else
        {
            for (int i = 0; i <= s_top; i++)
            {
                std::cout << s_arr[i];
                if (i == s_top - 1)
                    std::cout << std::endl;
                else
                    std::cout << ", ";
            }
        }
    }
};