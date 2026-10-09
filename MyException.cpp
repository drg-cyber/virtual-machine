/*
 * File:        MyException.cpp
 * Written by:  NAME:AHMED MOHAMED DAFAALLA AHMED   ID:253UC255PR
 * Description: Custom exception class used in place of std::runtime_error /
 *              std::exception for signalling VM errors.
 */

#include "MyException.h"

// just stores whatever message gets passed in
MyException::MyException(const std::string &msg)
    : message(msg)
{
}

std::string MyException::what() const
{
    return message;
}