/*
 * File:        MyQueue.h
 * Written by:  NAME:AHMED MOHAMED DAFAALLA AHMED   ID:253UC255PR
 * Description: Custom array-based queue template used in place of std::queue.
 */

#pragma once
#include <iostream>
#include "MyException.h"

// simple array based queue, linear not circular so once q_rear hits capacity that's it
// (no wraparound, so space at the front doesn't get reused after dequeues)
template <typename Q>
class MyQueue
{
    Q *q_arr;        // underlying array
    int q_capacity;   // max number of elements
    int q_front;      // index of front element
    int q_rear;       // index of rear element

public:
    // both front and rear start at -1 to signal empty
    MyQueue(int q_capacity)
    {
        this->q_capacity = q_capacity;
        this->q_front = -1;
        this->q_rear = -1;
        q_arr = new Q[q_capacity];
    }

    ~MyQueue()
    {
        delete[] q_arr;
    }

    bool isemptyQ() const { return q_front == -1 && q_rear == -1; }

    // full just means we've hit the end of the array, even if front has moved up
    bool isfullQ() const { return q_rear == q_capacity - 1; }

    int size() const
    {
        if (isemptyQ())
            return 0;

        return q_rear - q_front + 1;
    }

    // adds to the back, sets front to 0 the first time something's added
    void enqueue(Q val)
    {
        if (isfullQ())
            std::cout << "QUEUE IS FULL!\n";
        else
        {
            if (q_front == -1)
                q_front = 0;
            q_rear++;
            q_arr[q_rear] = val;
        }
    }

    // removes from front, resets both pointers to -1 if that was the last element
    void dequeue()
    {
        if (isemptyQ())
            std::cout << "QUEUE IS EMPTY!\n";
        else
        {
            if (q_front == q_rear)
                q_front = q_rear = -1;
            else
                q_front++;
        }
    }

    Q showfront() const
    {
        if (isemptyQ())
            throw MyException("QUEUE IS EMPTY!");
        else
            return q_arr[q_front];
    }

    Q showrear() const
    {
        if (isemptyQ())
            throw MyException("QUEUE IS EMPTY!");
        else
            return q_arr[q_rear];
    }

    // debug print, loops from front to rear only (not the whole array)
    void printQueue()
    {
        if (isemptyQ())
            std::cout << "QUEUE IS EMPTY!\n";
        else
        {
            for (int i = q_front; i <= q_rear; i++)
            {
                std::cout << q_arr[i];
                if (i != q_rear)
                    std::cout << ", ";
                else
                    std::cout << std::endl;
            }
        }
    }
};