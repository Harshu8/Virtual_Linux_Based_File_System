#pragma once
#include "Directory.h"

class CommandLine
{
private:
    int directoryIndex;
    std::shared_ptr<IDirectory> root = std::make_shared<Directory>();
    std::shared_ptr<IDirectory> currentDirectory = std::make_shared<Directory>();
    std::vector<std::shared_ptr<IDirectory>> allDirs;

public:
    CommandLine(const std::shared_ptr<IDirectory> &root);
    int getAllDirsSize();
    std::shared_ptr<IDirectory> getDirFromAllDirsByIndex(int index);
    std::vector<std::string> parseInput(const std::string &inputString);
    std::vector<std::string> getPath();
    std::string getDesiredDirName(const std::string &name);
    int mkdir(const std::shared_ptr<IDirectory> &root, std::string name);
    int touch(const std::shared_ptr<IDirectory> &root, std::string name);
    int cd(const std::string &name);
    std::vector<std::string> ls();
    int find(const std::shared_ptr<IDirectory> &root, std::string name, std::vector<std::string> *result, std::string pathName);
    int update(const std::shared_ptr<IDirectory> &root, std::string name, std::string newName);
    int rmdir(const std::shared_ptr<IDirectory> &root, std::string name);
    int rm(const std::shared_ptr<IDirectory> &root, std::string name);
    std::shared_ptr<File> getFileForViAndUpdate(const std::shared_ptr<IDirectory> &mainDirectory, std::string name);
    int cat(const std::shared_ptr<IDirectory> &root, std::string fileName, std::string &result);
    int deleteFileContent(const std::shared_ptr<IDirectory> &root, std::string fileName);
};