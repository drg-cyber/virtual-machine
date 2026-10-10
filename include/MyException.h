/*
 * File:        MyException.h
 * Written by:  NAME:AHMED MOHAMED DAFAALLA AHMED   ID:253UC255PR
 * Description: Custom exception class used in place of std::runtime_error /
 *              std::exception for signalling VM errors.
 */

#pragma once

#include <string>

// basic custom exception class, just wraps a message
// used instead of std::exception since we're not allowed STL stuff
class MyException
{
private:
    std::string message;

public:
    MyException(const std::string &msg);

    std::string what() const; // returns the error message
};