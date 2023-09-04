#include <iostream>
#include <cstring>
#include <sstream>
#include "CommandLine.h"
#include "Directory.h"

void mkdirFun(const std::shared_ptr<IDirectory> &root, CommandLine &cmd, std::string &argument1)
{
    getline(std::cin, argument1);
    std::vector<std::string> stringToken = cmd.parseInput(argument1);
    for (int index = 0; index < stringToken.size(); index++)
    {
        if (stringToken[0][0] == '/')
        {
            std::cout << "Permission denied\n";
            continue;
        }
        int returnValue = cmd.mkdir(root, stringToken[index]);
        if (returnValue == 2)
        {
            std::cout << "mkdir: cannot create directory '" << stringToken[index] << "': directory already exists\n";
        }
        else if (returnValue == 3)
        {
            std::cout << "mkdir: cannot create directory '" << stringToken[index] << "': No such file or directory\n";
        }
    }
}

void touchFun(const std::shared_ptr<IDirectory> &root, CommandLine &cmd, std::string &argument1)
{
    getline(std::cin, argument1);
    std::vector<std::string> stringToken = cmd.parseInput(argument1);
    for (int index = 0; index < stringToken.size(); index++)
    {
        if (stringToken[index][0] == '/')
        {
            std::cout << "Permission denied\n";
            continue;
        }
        int returnValue = cmd.touch(root, stringToken[index]);
        if (returnValue == 2)
        {
            std::cout << "touch: cannot create file '" << stringToken[index] << "': No such directory\n";
        }
    }
}

void cdFun(CommandLine &cmd, std::string &argument1)
{
    std::cin >> argument1;
    int code = cmd.cd(argument1);
    if (code == 2)
    {
        std::cout << "cd: Already at root directory\n";
    }
    else if (code == 3)
    {
        std::cout << "cd: '" << argument1 << "': No such file or directory\n";
    }
}

void lsFun(CommandLine &cmd)
{
    std::vector<std::string> result = cmd.ls();
    for (int index = 0; index < result.size(); index++)
    {
        std::cout << result[index] << std::endl;
    }
}

void findFun(CommandLine &cmd, std::string &argument1)
{
    std::cin >> argument1;
    std::vector<std::string> result;

    std::string pathName = "";
    int lastIndex = cmd.getAllDirsSize();
    int returnValue = cmd.find(cmd.getDirFromAllDirsByIndex(lastIndex - 1), argument1, &result, pathName);
    if (returnValue == 2)
    {
        std::cout << "find: '" << argument1 << "': No such file or directory\n";
    }
    else
    {
        for (int index = 0; index < result.size(); index++)
        {
            std::cout << result[index] << std::endl;
        }
    }
}

void mvFun(const std::shared_ptr<IDirectory> &root, CommandLine &cmd, std::string &argument1, std::string &argument2)
{
    std::cin >> argument1;
    std::cin >> argument2;
    int slashCount1 = 0, slashCount2 = 0, lastSlashIndex1 = 0, lastSlashIndex2 = 0;
    bool isPresentInSameDir = true;
    for (int index = 0; index < argument1.size(); index++)
    {
        if (argument1[index] == '/')
        {
            slashCount1++;
            lastSlashIndex1 = index;
        }
    }
    for (int index = 0; index < argument2.size(); index++)
    {
        if (argument2[index] == '/')
        {
            slashCount2++;
            lastSlashIndex2 = index;
        }
    }
    int temp = lastSlashIndex1;
    for (int index = 0; index < lastSlashIndex1; index++)
    {
        if (argument1[index] != argument2[index])
        {
            isPresentInSameDir = false;
            break;
        }
    }

    if (slashCount1 != slashCount2)
    {
        std::cout << "mv: cannot update name, Path mismatch\n";
    }
    else if (!isPresentInSameDir)
    {
        std::cout << "mv: cannot update name as the given names can't belong to same dir\n";
    }
    else if (cmd.update(root, argument1, argument2) == 2)
    {
        std::cout << "mv: cannot update name '" << argument1 << "': No such file or directory\n";
    }
}

void rmdirFun(const std::shared_ptr<IDirectory> &root, CommandLine &cmd, std::string &argument1)
{
    getline(std::cin, argument1);
    std::vector<std::string> stringToken = cmd.parseInput(argument1);
    for (int index = 0; index < stringToken.size(); index++)
    {
        if (stringToken[index][0] == '/')
        {
            std::cout << "rmdir: failed to remove '" << stringToken[index] << "': No such file or directory\n";
            continue;
        }
        int returnValue = cmd.rmdir(root, stringToken[index]);
        if (returnValue == 2)
        {
            std::cout << "rmdir: failed to remove '" << stringToken[index] << "': No such file or directory\n";
        }
        else if (returnValue == 3)
        {
            std::cout << "rmdir: failed to remove '" << stringToken[index] << "': Directory not empty\n";
        }
    }
}

void rmFun(const std::shared_ptr<IDirectory> &root, CommandLine &cmd, std::string &argument1)
{
    getline(std::cin, argument1);
    std::vector<std::string> stringToken = cmd.parseInput(argument1);
    for (int index = 0; index < stringToken.size(); index++)
    {
        if (stringToken[index][0] == '/')
        {
            std::cout << "rm: failed to remove '" << stringToken[index] << "': No such file or directory\n";
            continue;
        }
        int returnValue = cmd.rm(root, stringToken[index]);
        if (returnValue == 2)
        {
            std::cout << "rm: failed to remove '" << stringToken[index] << "': No such file or directory\n";
        }
        else if (returnValue == 3)
        {
            std::cout << "rm: cannot " << stringToken[index] << " 'r1': Is a directory\n";
        }
    }
}

void viFun(const std::shared_ptr<IDirectory> &root, CommandLine &cmd, std::string &argument1, std::string &argument2)
{
    std::cin >> argument1;
    std::shared_ptr<File> desiredFile = cmd.getFileForViAndUpdate(root, argument1);
    if (desiredFile)
    {
        std::cout << "Enter content for file: ";
        std::string content;
        std::cin >> content;
        getline(std::cin, argument2);
        desiredFile->setNewFileContent(content + argument2);
    }
    else
    {
        std::cout << "vi: '" << argument1 << "' dir or file not exist\n";
    }
}

void catFun(const std::shared_ptr<IDirectory> &root, CommandLine &cmd, std::string &argument1)
{
    std::cin >> argument1;
    std::string result = "";
    int res = cmd.cat(root, argument1, result);
    if (res == 2)
    {
        std::cout << "cat: '" << argument1 << "' dir or file not exist\n";
    }
    else if (result.size() == 0)
    {
        std::cout << "cat: '" << argument1 << "' File is empty\n";
    }
    else
    {
        std::cout << result << "\n";
    }
}

void updateFileFun(const std::shared_ptr<IDirectory> &root, CommandLine &cmd, std::string &argument1, std::string &argument2)
{
    std::cin >> argument1;
    std::shared_ptr<File> desiredFile = cmd.getFileForViAndUpdate(root, argument1);
    if (desiredFile)
    {
        std::cout << "Enter content for file: ";
        std::string content;
        std::cin >> content;
        getline(std::cin, argument2);
        desiredFile->updateContent(content + argument2);
    }
    else
    {
        std::cout << "updatefile: '" << argument1 << "' dir or file not exist\n";
    }
}

void dfcFun(const std::shared_ptr<IDirectory> &root, CommandLine &cmd, std::string &argument1)
{
    std::cin >> argument1;
    int res = cmd.deleteFileContent(root, argument1);
    if (res == 2)
    {
        std::cout << "dfc: '" << argument1 << "' dir or file not exist\n";
    }
}

int main()
{
    std::shared_ptr<IDirectory> root = std::make_shared<Directory>();
    root->setName("root");
    CommandLine cmd(root);

    std::string command, argument1, argument2;

    while (true)
    {
        std::vector<std::string> pathList = cmd.getPath();
        for (int index = 0; index < pathList.size(); index++)
        {
            std::cout << pathList[index] << "/";
        }
        std::cout << "$ ";
        std::cin >> command;
        if (command == "mkdir")
        {
            mkdirFun(root, cmd, argument1);
        }
        else if (command == "touch")
        {
            touchFun(root, cmd, argument1);
        }
        else if (command == "cd")
        {
            cdFun(cmd, argument1);
        }
        else if (command == "ls")
        {
            lsFun(cmd);
        }
        else if (command == "find")
        {
            findFun(cmd, argument1);
        }
        else if (command == "mv")
        {
            mvFun(root, cmd, argument1, argument2);
        }
        else if (command == "rmdir")
        {
            rmdirFun(root, cmd, argument1);
        }
        else if (command == "rm")
        {
            rmFun(root, cmd, argument1);
        }
        else if (command == "vi")
        {
            viFun(root, cmd, argument1, argument2);
        }
        else if (command == "cat")
        {
            catFun(root, cmd, argument1);
        }
        else if (command == "updatefile")
        {
            updateFileFun(root, cmd, argument1, argument2);
        }
        else if (command == "dfc") // delete file content
        {
            dfcFun(root, cmd, argument1);
        }
        else if (command == "exit")
        {
            break;
        }
        else
        {
            std::cout << "invalid command\n";
        }
    }
    return 0;
}