#pragma once
#include "IDirectory.h"

class Directory : public IDirectory
{
private:
    std::string name;
    std::vector<std::shared_ptr<File>> files;
    std::vector<std::shared_ptr<Directory>> subdirectories;

public:
    std::string getName();
    bool setName(const std::string &name);
    bool addFile(const std::string &newFileName);
    bool addDirectory(const std::string &newDirectoryName);
    std::shared_ptr<IDirectory> getSubdirectory(const std::string &name);
    std::shared_ptr<File> getFileByName(const std::string &name);
    std::vector<std::string> getFileAndDirectoryList();
    bool resultOfFind(const std::string &name, std::vector<std::string> *result, std::string pathName);
    bool updateName(const std::string &name, const std::string &newName);
    bool removeDir(const std::string &name);
    bool removeFile(const std::string &name);
};