#include "Directory.h"

std::string Directory::getName()
{
    return name;
}

bool Directory::setName(const std::string &inputName)
{
    name = inputName;
    return true;
}

bool Directory::addFile(const std::string &newFileName)
{
    bool isAdd = true;
    for (auto &filesIterator : files)
    {
        if (filesIterator->getName() == newFileName)
        {
            isAdd = false;
            break;
        }
    }
    if (isAdd == true)
    {
        auto newFile = std::make_shared<File>();
        newFile->setName(newFileName);
        files.push_back(newFile);
    }
    return isAdd;
}

bool Directory::addDirectory(const std::string &newDirectoryName)
{
    bool isAdd = true;
    for (auto &directoryIterator : subdirectories)
    {
        if (directoryIterator->getName() == newDirectoryName)
        {
            isAdd = false;
            break;
        }
    }
    if (isAdd == true)
    {
        auto newDirectory = std::make_shared<Directory>();
        newDirectory->setName(newDirectoryName);
        subdirectories.push_back(newDirectory);
    }
    return isAdd;
}

std::shared_ptr<IDirectory> Directory::getSubdirectory(const std::string &name)
{
    std::shared_ptr<IDirectory> result = nullptr;
    for (auto &directoryIterator : subdirectories)
    {
        if (directoryIterator->getName() == name)
        {
            result = directoryIterator;
        }
    }
    return result;
}

std::shared_ptr<File> Directory::getFileByName(const std::string &name)
{
    std::shared_ptr<File> result = nullptr;
    for (auto &filesIterator : files)
    {
        if (filesIterator->getName() == name)
        {
            result = filesIterator;
        }
    }
    return result;
}

std::vector<std::string> Directory::getFileAndDirectoryList()
{
    std::vector<std::string> result;
    for (auto &directoryIterator : subdirectories)
    {
        result.push_back(directoryIterator->getName() + '/');
    }
    for (auto &filesIterator : files)
    {
        result.push_back(filesIterator->getName());
    }
    return result;
}

bool Directory::resultOfFind(const std::string &name, std::vector<std::string> *result, std::string pathName)
{
    bool isFind = false;
    for (auto &directoryIterator : subdirectories)
    {
        if (directoryIterator->getName() == name)
        {
            if (result->size() == 0)
            {
                result->push_back(pathName + name);
            }
            pathName = pathName + name + '/';
            for (int index = 0; index < directoryIterator->files.size(); index++)
            {
                result->push_back(pathName + directoryIterator->files[index]->getName());
            }
            for (int index = 0; index < directoryIterator->subdirectories.size(); index++)
            {
                result->push_back(pathName + directoryIterator->subdirectories[index]->getName());
                directoryIterator->resultOfFind(directoryIterator->subdirectories[index]->getName(), result, pathName);
            }
            isFind = true;
        }
    }
    for (auto &filesIterator : files)
    {
        if (filesIterator->getName() == name)
        {
            result->push_back(filesIterator->getName());
            isFind = true;
            break;
        }
    }
    return isFind;
}

bool Directory::updateName(const std::string &name, const std::string &newName)
{
    bool isUpdate = false;
    for (auto &directoryIterator : subdirectories)
    {
        if (directoryIterator->getName() == name)
        {
            directoryIterator->name = newName;
            isUpdate = true;
            break;
        }
    }
    for (auto &filesIterator : files)
    {
        if (filesIterator->getName() == name)
        {
            filesIterator->setName(newName);
            isUpdate = true;
            break;
        }
    }
    return isUpdate;
}

bool Directory::removeDir(const std::string &name)
{
    bool isDirectoryRemove = false;
    int position = 0;
    for (auto &directoryIterator : subdirectories)
    {
        if (directoryIterator->getName() == name)
        {
            if (directoryIterator->files.size() > 0 || directoryIterator->subdirectories.size() > 0)
            {
                break;
            }
            else
            {
                subdirectories.erase(subdirectories.begin() + position);
                isDirectoryRemove = true;
            }
            break;
        }
        position++;
    }
    return isDirectoryRemove;
}

bool Directory::removeFile(const std::string &name)
{
    bool isFileRemove = false;
    int position = 0;
    for (auto &filesIterator : files)
    {
        if (filesIterator->getName() == name)
        {
            isFileRemove = true;
            files.erase(files.begin() + position);
            break;
        }
        position++;
    }
    return isFileRemove;
}