#pragma once
#include <iostream>

class IFileSystem
{
public:
    virtual std::string getName() = 0;
    virtual bool setName(const std::string &) = 0;
};