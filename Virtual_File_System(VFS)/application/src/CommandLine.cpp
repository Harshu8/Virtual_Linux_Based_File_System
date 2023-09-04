#include "../inc/CommandLine.h"
#include <sstream>
#include <cstring>

CommandLine::CommandLine(const std::shared_ptr<IDirectory> &root)
{
    allDirs.push_back(root);
    directoryIndex = 0;
    this->root = root;
    currentDirectory = root;
}

int CommandLine::getAllDirsSize()
{
    return allDirs.size();
}

std::shared_ptr<IDirectory> CommandLine::getDirFromAllDirsByIndex(int index)
{
    return allDirs[index];
}

std::vector<std::string> CommandLine::parseInput(const std::string &argument)
{
    std::vector<std::string> parseResult;
    std::stringstream tempStringStream(argument);
    std::string intermediateString;
    while (getline(tempStringStream, intermediateString, ' '))
    {
        if (!(strcmp(intermediateString.c_str(), " ") == -32))
        {
            parseResult.push_back(intermediateString);
        }
    }
    return parseResult;
}

std::vector<std::string> CommandLine::getPath()
{
    std::vector<std::string> result;
    for (int index = 0; index < allDirs.size(); index++)
    {
        result.push_back(allDirs[index]->getName());
    }
    return result;
}

std::string CommandLine::getDesiredDirName(const std::string &name)
{
    std::string desiredDir = "";
    int index = 0;
    while (index < name.size())
    {
        if (name[index] == '/')
        {
            break;
        }
        desiredDir.insert(desiredDir.end(), name[index]);
        index++;
    }
    return desiredDir;
}

int CommandLine::mkdir(const std::shared_ptr<IDirectory> &mainDirectory, std::string name)
{
    // 1 - successful, 2 - dir already here, 3 - directory is not find
    int returnValue = 1;
    int index = 0;
    int nameSize = name.size();
    int slashCount = 0;
    while (index < nameSize)
    {
        if (name[index] == '/')
        {
            slashCount++;
        }
        index++;
    }
    std::string desiredDir = getDesiredDirName(name);
    int indexForNewName = desiredDir.size();

    auto subdirectory = currentDirectory->getSubdirectory(desiredDir);
    if (subdirectory)
    {
        if (slashCount != 0)
        {
            name.erase(name.begin(), name.begin() + indexForNewName + 1);
        }
        else
        {
            returnValue = 2;
            name = "";
        }
        currentDirectory = subdirectory;
        if (name != "")
        {
            returnValue = mkdir(subdirectory, name);
        }
    }
    else
    {
        int count = 0, count2 = 0;
        bool isNameEncounter = false;
        bool isAnotherName = false;

        for (int index = name.size() - 1; index >= 0; index--)
        {
            if (name[index] == '/')
            {
                if (isNameEncounter)
                {
                    isAnotherName = true;
                    returnValue = 3;
                    break;
                }
                count++;
            }
            else
            {
                isNameEncounter = true;
            }
        }
        name.erase(name.end() - count, name.end());
        if (!isAnotherName)
        {
            if (!currentDirectory->addDirectory(name))
            {
                returnValue = 2;
            }
        }
    }

    int size = allDirs.size() - 1;
    currentDirectory = allDirs[size];
    return returnValue;
}

int CommandLine::touch(const std::shared_ptr<IDirectory> &mainDirectory, std::string name)
{
    int returnValue = 1; // 1 - successful, 2 - directory is not find
    std::string desiredDir = getDesiredDirName(name);
    int indexForNewName = desiredDir.size();

    auto subdirectory = currentDirectory->getSubdirectory(desiredDir);
    if (subdirectory)
    {
        if (indexForNewName < name.size())
        {
            name.erase(name.begin(), name.begin() + indexForNewName + 1);
            currentDirectory = subdirectory;
            returnValue = touch(subdirectory, name);
        }
        else
        {
            returnValue = 1; // actually the file is not create but give successful code because the real terminal does the same.
            name = "";
        }
    }
    else
    {
        int count = 0, count2 = 0, slashCount = 0;
        bool isNameEncounter = false;
        bool isAnotherName = false;

        for (int index = name.size() - 1; index >= 0; index--)
        {
            if (name[index] == '/')
            {
                if (isNameEncounter)
                {
                    isAnotherName = true;
                    returnValue = 2;
                    break;
                }
                count++;
            }
            else
            {
                isNameEncounter = true;
            }
        }
        name.erase(name.end() - count, name.end());

        if (!isAnotherName)
        {
            currentDirectory->addFile(name);
        }
    }

    int size = allDirs.size() - 1;
    currentDirectory = allDirs[size];
    return returnValue;
}

int CommandLine::cd(const std::string &name1)
{
    int cdErrorCode = 1; // 1 : no error, 2 : already at root directory, 3: directory not found
    std::string name = name1;
    bool isFirstDir = true;
    std::shared_ptr<IDirectory> firstDirectory = std::make_shared<Directory>();
    std::stringstream stringStreamForName(name);
    std::string intermediateString;
    int slashCount = 0;
    bool isNameEncounter = false, isAnotherName = false;
    int count = 0;
    for (int index = name.size() - 1; index >= 0; index--)
    {
        if (name[index] == '/')
        {
            if (isNameEncounter)
            {
                isAnotherName = true;
                break;
            }
            count++;
        }
        else
        {
            isNameEncounter = true;
        }
    }
    name.erase(name.end() - count, name.end());
    int index = 0;
    while (index < name.size())
    {
        if (name[index] == '/')
        {
            slashCount++;
        }
        index++;
    }
    if (slashCount > 0)
    {
        name = name + "/";
    }
    while (getline(stringStreamForName, intermediateString, '/'))
    {
        if (intermediateString.size() == 0)
        {
            break;
        }
        if (strcmp(intermediateString.c_str(), "..") == 0)
        {
            if (isFirstDir)
            {
                firstDirectory = currentDirectory;
                isFirstDir = false;
            }
            if (currentDirectory == root)
            {
                cdErrorCode = 2;
                break;
            }
            else
            {
                allDirs.erase(allDirs.end());
                directoryIndex = directoryIndex - 1;
                currentDirectory = allDirs[directoryIndex];
            }
        }
        else if (intermediateString[0] != '.')
        {
            auto subdirectory = currentDirectory->getSubdirectory(intermediateString);
            if (isFirstDir)
            {
                firstDirectory = currentDirectory;
                isFirstDir = false;
            }
            if (subdirectory)
            {
                directoryIndex = directoryIndex + 1;
                allDirs.push_back(subdirectory);
                currentDirectory = subdirectory;
            }
            else
            {
                if (slashCount != 0 && isFirstDir)
                {
                    allDirs.erase(allDirs.end() - slashCount, allDirs.end());
                    currentDirectory = firstDirectory;
                }
                cdErrorCode = 3;
                break;
            }
        }
        else
        {
            cdErrorCode = 3;
            break;
        }
    }
    return cdErrorCode;
}

std::vector<std::string> CommandLine::ls()
{
    return currentDirectory->getFileAndDirectoryList();
}

int CommandLine::find(const std::shared_ptr<IDirectory> &mainDirectory, std::string name, std::vector<std::string> *result, std::string pathName)
{
    int returnValue = 1; // 1 - successful, 2 - directory is not find

    bool isNameEncounter = false, isAnotherName = false;
    int count = 0;
    for (int index = name.size() - 1; index >= 0; index--)
    {
        if (name[index] == '/')
        {
            if (isNameEncounter)
            {
                isAnotherName = true;
                break;
            }
            count++;
        }
        else
        {
            isNameEncounter = true;
        }
    }
    name.erase(name.end() - count, name.end());
    int index = 0;
    int slashCount = 0;
    while (index < name.size())
    {
        if (name[index] == '/')
        {
            slashCount++;
        }
        index++;
    }

    std::string desiredDir = getDesiredDirName(name);
    int indexForNewName = desiredDir.size();

    auto subdirectory = mainDirectory->getSubdirectory(desiredDir);

    if (subdirectory && slashCount != 0)
    {
        name.erase(name.begin(), name.begin() + indexForNewName + 1);
        pathName = pathName + desiredDir + "/";
        returnValue = find(subdirectory, name, result, pathName);
    }
    else
    {
        returnValue = 1;
        mainDirectory->resultOfFind(desiredDir, result, pathName);
        if (result->size() == 0)
        {
            returnValue = 2;
        }
    }
    return returnValue;
}

int CommandLine::update(const std::shared_ptr<IDirectory> &mainDirectory, std::string name, std::string newName)
{
    int returnValue = 1; // 1 - successful, 2 - directory is not find or file
    int nameSize = name.size();
    int slashCount = 0;

    bool isNameEncounter = false, isAnotherName = false;
    int count = 0;
    for (int index = name.size() - 1; index >= 0; index--)
    {
        if (name[index] == '/')
        {
            if (isNameEncounter)
            {
                isAnotherName = true;
                break;
            }
            count++;
        }
        else
        {
            isNameEncounter = true;
        }
    }

    bool isNameEncounter2 = false, isAnotherName2 = false;
    int count2 = 0;
    for (int index = newName.size() - 1; index >= 0; index--)
    {
        if (newName[index] == '/')
        {
            if (isNameEncounter2)
            {
                isAnotherName2 = true;
                break;
            }
            count2++;
        }
        else
        {
            isNameEncounter2 = true;
        }
    }

    name.erase(name.end() - count, name.end());
    newName.erase(newName.end() - count2, newName.end());
    int index = 0;

    while (index < nameSize)
    {
        if (name[index] == '/')
        {
            slashCount++;
        }
        index++;
    }

    std::string desiredDir = getDesiredDirName(name);
    int indexForNewNameForNextIteration = desiredDir.size();

    auto subdirectory = mainDirectory->getSubdirectory(desiredDir);
    if (mainDirectory != root)
    {
        subdirectory = mainDirectory->getSubdirectory(desiredDir);
    }
    else
    {
        subdirectory = currentDirectory->getSubdirectory(desiredDir);
    }

    if (subdirectory)
    {
        if ((indexForNewNameForNextIteration < name.size()) && (indexForNewNameForNextIteration < newName.size()))
        {
            name.erase(name.begin(), name.begin() + indexForNewNameForNextIteration + 1);
            newName.erase(newName.begin(), newName.begin() + indexForNewNameForNextIteration + 1);
        }
        else
        {
            if (mainDirectory != root)
            {
                if (!mainDirectory->updateName(name, newName))
                {
                    returnValue = 2;
                }
            }
            else
            {
                if (!currentDirectory->updateName(name, newName))
                {
                    returnValue = 2;
                }
            }
            name = "";
        }
        if (name != "")
        {
            returnValue = update(subdirectory, name, newName);
        }
    }
    else
    {
        if (!mainDirectory->updateName(name, newName))
        {
            returnValue = 2;
        }
    }
    return returnValue;
}

int CommandLine::rmdir(const std::shared_ptr<IDirectory> &mainDirectory, std::string name)
{
    int returnValue = 1; // 1 - successful, 2 - directory is not find, 3 - directory not empty
    int nameSize = name.size();
    int slashCount = 0;

    bool isNameEncounter = false, isAnotherName = false;
    int count = 0;
    for (int index = name.size() - 1; index >= 0; index--)
    {
        if (name[index] == '/')
        {
            if (isNameEncounter)
            {
                isAnotherName = true;
                break;
            }
            count++;
        }
        else
        {
            isNameEncounter = true;
        }
    }
    name.erase(name.end() - count, name.end());
    int index = 0;

    while (index < nameSize)
    {
        if (name[index] == '/')
        {
            slashCount++;
        }
        index++;
    }

    std::string desiredDir = getDesiredDirName(name);
    int indexForNewName = desiredDir.size();

    auto subdirectory = mainDirectory->getSubdirectory(desiredDir);
    if (mainDirectory != root)
    {
        subdirectory = mainDirectory->getSubdirectory(desiredDir);
    }
    else
    {
        subdirectory = currentDirectory->getSubdirectory(desiredDir);
    }

    if (subdirectory)
    {
        if (indexForNewName < name.size())
        {
            name.erase(name.begin(), name.begin() + indexForNewName + 1);
        }
        else
        {
            if (mainDirectory != root)
            {
                if (!mainDirectory->removeDir(name))
                {
                    returnValue = 3;
                }
            }
            else
            {
                if (!currentDirectory->removeDir(name))
                {
                    returnValue = 3;
                }
            }
            name = "";
        }
        if (name != "")
        {
            returnValue = rmdir(subdirectory, name);
        }
    }
    else
    {
        returnValue = 2;
    }
    return returnValue;
}

int CommandLine::rm(const std::shared_ptr<IDirectory> &mainDirectory, std::string name)
{
    int returnValue = 1; // 1 - successful, 2 - file not find, 3 - rm: cannot remove 'r1': Is a directory

    std::string desiredDir = getDesiredDirName(name);
    int indexForNewName = desiredDir.size();

    auto subdirectory = mainDirectory->getSubdirectory(desiredDir);
    if (mainDirectory != root)
    {
        subdirectory = mainDirectory->getSubdirectory(desiredDir);
    }
    else
    {
        subdirectory = currentDirectory->getSubdirectory(desiredDir);
    }

    if (subdirectory)
    {
        if (indexForNewName < name.size())
        {
            name.erase(name.begin(), name.begin() + indexForNewName + 1);
            returnValue = rm(subdirectory, name);
        }
        else
        {
            returnValue = 3;
            name = "";
        }
    }
    else
    {
        int size = allDirs.size() - 1;
        if (mainDirectory != root)
        {
            if (!mainDirectory->removeFile(name))
            {
                returnValue = 2;
            }
        }
        else
        {
            if (!currentDirectory->removeFile(name))
            {
                returnValue = 2;
            }
        }
    }
    return returnValue;
}

std::shared_ptr<File> CommandLine::getFileForViAndUpdate(const std::shared_ptr<IDirectory> &mainDirectory, std::string name)
{
    std::shared_ptr<File> desiredFile = nullptr;
    std::string desiredDir = getDesiredDirName(name);
    int indexForNewName = desiredDir.size();

    auto subdirectory = mainDirectory->getSubdirectory(desiredDir);
    if (mainDirectory != root)
    {
        subdirectory = mainDirectory->getSubdirectory(desiredDir);
    }
    else
    {
        subdirectory = currentDirectory->getSubdirectory(desiredDir);
    }

    if (subdirectory)
    {
        if (indexForNewName < name.size())
        {
            name.erase(name.begin(), name.begin() + indexForNewName + 1);
            desiredFile = getFileForViAndUpdate(subdirectory, name);
        }
        else
        {
            name = "";
        }
    }
    else
    {
        desiredFile = mainDirectory->getFileByName(name);
        if (mainDirectory != root)
        {
            desiredFile = mainDirectory->getFileByName(name);
        }
        else
        {
            desiredFile = currentDirectory->getFileByName(name);
        }
    }
    return desiredFile;
}

int CommandLine::cat(const std::shared_ptr<IDirectory> &mainDirectory, std::string name, std::string &result)
{
    int returnValue = 1; // 1 - successful, 2 - file not find

    std::string desiredDir = getDesiredDirName(name);
    int indexForNewName = desiredDir.size();

    auto subdirectory = mainDirectory->getSubdirectory(desiredDir);
    if (mainDirectory != root)
    {
        subdirectory = mainDirectory->getSubdirectory(desiredDir);
    }
    else
    {
        subdirectory = currentDirectory->getSubdirectory(desiredDir);
    }

    if (subdirectory)
    {
        if (indexForNewName < name.size())
        {
            name.erase(name.begin(), name.begin() + indexForNewName + 1);
            returnValue = cat(subdirectory, name, result);
        }
        else
        {
            returnValue = 2;
            name = "";
        }
    }
    else
    {
        std::shared_ptr<File> desiredFile = mainDirectory->getFileByName(name);
        if (mainDirectory != root)
        {
            desiredFile = mainDirectory->getFileByName(name);
        }
        else
        {
            desiredFile = currentDirectory->getFileByName(name);
        }
        if (desiredFile)
        {
            result = desiredFile->getFileContent();
            returnValue = 1;
        }
        else
        {
            returnValue = 2;
        }
    }
    return returnValue;
}

int CommandLine::deleteFileContent(const std::shared_ptr<IDirectory> &mainDirectory, std::string name)
{
    int returnValue = 1; // 1 - successful, 2 - file not find

    std::string desiredDir = getDesiredDirName(name);
    int indexForNewName = desiredDir.size();

    auto subdirectory = mainDirectory->getSubdirectory(desiredDir);
    if (mainDirectory != root)
    {
        subdirectory = mainDirectory->getSubdirectory(desiredDir);
    }
    else
    {
        subdirectory = currentDirectory->getSubdirectory(desiredDir);
    }

    if (subdirectory)
    {
        if (indexForNewName < name.size())
        {
            name.erase(name.begin(), name.begin() + indexForNewName + 1);
            returnValue = deleteFileContent(subdirectory, name);
        }
        else
        {
            returnValue = 2;
            name = "";
        }
    }
    else
    {
        std::shared_ptr<File> desiredFile = mainDirectory->getFileByName(name);
        if (mainDirectory != root)
        {
            desiredFile = mainDirectory->getFileByName(name);
        }
        else
        {
            desiredFile = currentDirectory->getFileByName(name);
        }

        if (desiredFile)
        {
            desiredFile->deleteContent();
            returnValue = 1;
        }
        else
        {
            returnValue = 2;
        }
    }
    return returnValue;
}