#pragma once
#include "IFileSystem.h"
#include "File.h"
#include <vector>
#include <memory>

class IDirectory : public IFileSystem
{
public:
    virtual std::string getName() = 0;
    virtual bool setName(const std::string &) = 0;
    virtual bool addFile(const std::string &) = 0;
    virtual bool addDirectory(const std::string &) = 0;
    virtual std::shared_ptr<IDirectory> getSubdirectory(const std::string &) = 0;
    virtual std::shared_ptr<File> getFileByName(const std::string &) = 0;
    virtual std::vector<std::string> getFileAndDirectoryList() = 0;
    virtual bool resultOfFind(const std::string &, std::vector<std::string> *, std::string) = 0;
    virtual bool updateName(const std::string &, const std::string &) = 0;
    virtual bool removeDir(const std::string &) = 0;
    virtual bool removeFile(const std::string &) = 0;
};