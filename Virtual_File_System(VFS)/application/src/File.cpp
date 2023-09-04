#include "File.h"

std::string File::getName()
{
    return name;
}

bool File::setName(const std::string &newName)
{
    name = newName;
    return true;
}

std::string File::getFileContent()
{
    return fileContent;
}

bool File::updateContent(const std::string &newContent)
{
    bool isUpdate = true;
    std::string temp = newContent;
    if (fileContent.size() != 0)
    {
        fileContent = fileContent + " " + temp;
    }
    else
    {
        fileContent = fileContent + temp;
    }
    return isUpdate;
}

bool File::setNewFileContent(const std::string &newContent)
{
    fileContent = newContent;
    return true;
}

bool File::deleteContent()
{
    fileContent.erase();
    return true;
}