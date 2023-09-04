#pragma once
#include "IFileSystem.h"

class File : public IFileSystem
{
private:
    std::string fileContent;
    std::string name;

public:
    std::string getName();
    bool setName(const std::string &name);
    std::string getFileContent();
    bool setNewFileContent(const std::string &fileContent);
    bool updateContent(const std::string &fileContent);
    bool deleteContent();
};