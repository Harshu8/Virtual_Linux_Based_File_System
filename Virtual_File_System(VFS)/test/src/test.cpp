#include <iostream>
#include "MockDirectory.h"

class CommandLineTestFixture : public ::testing::Test
{
public:
    std::shared_ptr<MockDirectory> mockDirectoryObj = std::make_shared<MockDirectory>();
    std::shared_ptr<IDirectory> root = std::make_shared<Directory>();
    std::shared_ptr<File> file = std::make_shared<File>();
    std::shared_ptr<IDirectory> rootPointToNull = nullptr;
    std::shared_ptr<File> filePointToNull = nullptr;
    std::shared_ptr<CommandLine> cmdObj = std::make_shared<CommandLine>(mockDirectoryObj);
};

class DirectoryTestFixture : public ::testing::Test
{
public:
    std::shared_ptr<Directory> dirObj = std::make_shared<Directory>();
    std::vector<std::string> result;
};

class FileTestFixture : public ::testing::Test
{
public:
    std::shared_ptr<File> fileObj = std::make_shared<File>();
};

TEST(CommandLineTest, whenThereIsRootThenGetAllDirsSizeIsOne)
{
    std::shared_ptr<IDirectory> root = std::make_shared<Directory>();
    root->setName("Root");
    CommandLine cmdObj(root);

    int expectedValue = cmdObj.getAllDirsSize();
    int actualValue = 1;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST(CommandLineTest, whenThereIsRootThenGetAllDirsByIndexGiveCorrectOutput)
{
    std::shared_ptr<IDirectory> root = std::make_shared<Directory>();
    root->setName("Root");
    CommandLine cmdObj(root);

    int expectedValueOfSize = cmdObj.getAllDirsSize();
    int actualValueOfSize = 1;

    std::string expectedString = (cmdObj.getDirFromAllDirsByIndex(0))->getName();
    std::string actualString = "Root";

    EXPECT_EQ(expectedValueOfSize, actualValueOfSize);
    EXPECT_EQ(expectedString, actualString);
}

TEST(CommandLineTest, whenInputStringThenGetCorrectOutputFromGetPath)
{
    std::shared_ptr<IDirectory> root = std::make_shared<Directory>();
    CommandLine cmdObj(root);
    std::string rootName = "root";
    root->setName(rootName);

    std::vector<std::string> res = cmdObj.getPath();
    std::string expectedString = res[0];
    std::string ActualString = "root";

    EXPECT_EQ(expectedString, ActualString);
}

TEST(CommandLineTest, whenInputStringThenGetCorrectOutputFromGetDesiredDirName)
{
    std::shared_ptr<IDirectory> root = std::make_shared<Directory>();
    CommandLine cmdObj(root);

    std::string expectedString = cmdObj.getDesiredDirName("dir1/dir2/dir3");
    std::string ActualString = "dir1";

    EXPECT_EQ(expectedString, ActualString);
}

TEST(CommandLineTest, whenInputStringThenGetCorrectOutputFromGetDesiredDirName2)
{
    std::shared_ptr<IDirectory> root = std::make_shared<Directory>();
    CommandLine cmdObj(root);

    std::string expectedString = cmdObj.getDesiredDirName("/");
    std::string ActualString = "";

    EXPECT_EQ(expectedString, ActualString);
}

TEST_F(CommandLineTestFixture, whenInputStringToParseInputFunctionThenGetCorrectOutput)
{
    std::string rootName = "root";
    root->setName(rootName);

    std::string input = "f1 f1/f2 f1/f3";
    std::vector<std::string> returnStringVector = cmdObj->parseInput(input);

    EXPECT_EQ(returnStringVector[0], "f1");
    EXPECT_EQ(returnStringVector[1], "f1/f2");
    EXPECT_EQ(returnStringVector[2], "f1/f3");
}

TEST_F(CommandLineTestFixture, whenInputStringToParseInputFunctionThenGetIncorrectOutput)
{
    std::string rootName = "root";
    root->setName(rootName);

    std::string input = "f1f1/f2f1/f3";
    std::vector<std::string> returnStringVector = cmdObj->parseInput(input);

    EXPECT_NE(returnStringVector[0], "f1");
    EXPECT_EQ(returnStringVector[0], "f1f1/f2f1/f3");
}

TEST_F(CommandLineTestFixture, whenCreateNewDirectoryThenItReturnCorrectOutput)
{
    std::string tempString = "dir1////////////////";
    EXPECT_CALL(*mockDirectoryObj, getSubdirectory("dir1"))
        .Times(1)
        .WillOnce(Return(rootPointToNull));

    EXPECT_CALL(*mockDirectoryObj, addDirectory("dir1"))
        .Times(1)
        .WillOnce(Return(true));

    int expectedValue = cmdObj->mkdir(mockDirectoryObj, tempString);
    int actualValue = 1;

    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenCreateNewDirectoryButItIsAlreadyPresentThenItReturnCorrectOutput)
{
    std::string tempString = "hello";

    EXPECT_CALL(*mockDirectoryObj, getSubdirectory("hello"))
        .Times(1)
        .WillOnce(Return(root));

    int expectedValue = cmdObj->mkdir(mockDirectoryObj, tempString);
    int actualValue = 2;

    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenCreateNewFileThenItReturnCorrectOutput)
{
    std::string tempString = "hello";

    EXPECT_CALL(*mockDirectoryObj, getSubdirectory("hello"))
        .Times(1)
        .WillOnce(Return(rootPointToNull));

    EXPECT_CALL(*mockDirectoryObj, addFile("hello"))
        .Times(1)
        .WillOnce(Return(false));

    int expectedValue = cmdObj->touch(mockDirectoryObj, tempString);
    int actualValue = 1;

    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenCreateNewFileButFileAlreadyExistThenItReturnCorrectOutput)
{
    std::string tempString = "dir1";
    EXPECT_CALL(*mockDirectoryObj, getSubdirectory("dir1"))
        .Times(1)
        .WillOnce(Return(rootPointToNull));

    EXPECT_CALL(*mockDirectoryObj, addFile("dir1"))
        .Times(1)
        .WillOnce(Return(true));

    int expectedValue = cmdObj->touch(mockDirectoryObj, tempString);
    int actualValue = 1;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseCdFunctionThenItReturnCorrectOutput)
{
    EXPECT_CALL(*mockDirectoryObj, getSubdirectory("folder"))
        .Times(1)
        .WillOnce(Return(root));
        
    int expectedValue = cmdObj->cd("folder");
    int actualValue = 1;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseCdFunctionOnRootItselfThenItReturnCorrectOutput)
{
    int expectedValue = cmdObj->cd("..");
    int actualValue = 2;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseCdFunctionButDirectoryNotFoundThenItReturnCorrectOutput)
{
    EXPECT_CALL(*mockDirectoryObj, getSubdirectory("dirname"))
        .Times(1)
        .WillOnce(Return(rootPointToNull));

    int expectedValue = cmdObj->cd("dirname");
    int actualValue = 3;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenInputWrongStringThenGetIncorrectOutputFromGetPath)
{
    std::string rootName = "root";
    root->setName(rootName);
    CommandLine cmd(root);

    std::vector<std::string> res = cmd.getPath();
    std::string expectedString = res[0];
    std::string actualString = "f1";

    EXPECT_NE(expectedString, actualString);
}

TEST_F(CommandLineTestFixture, whenUseGetFileAndDirectoryListFunctionThenItReturnCorrectOutput)
{
    std::vector<std::string> returnStringVector;
    returnStringVector.push_back("Harsh");

    EXPECT_CALL(*mockDirectoryObj, getFileAndDirectoryList())
        .Times(1)
        .WillOnce(Return(returnStringVector));

    std::vector<std::string> expectReturnValue = cmdObj->ls();
    std::string expectedString = expectReturnValue[0];
    std::string actualString = "Harsh";

    EXPECT_EQ(expectedString, actualString);
}

TEST_F(CommandLineTestFixture, whenUseGetFileAndDirectoryListFunctionWithWrongInputThenItReturnIncorrectOutput)
{
    std::vector<std::string> returnStringVector;
    returnStringVector.push_back("Harsh");
    returnStringVector.push_back("upadhyay");

    EXPECT_CALL(*mockDirectoryObj, getFileAndDirectoryList())
        .Times(1)
        .WillOnce(Return(returnStringVector));

    std::vector<std::string> expectReturnValue = cmdObj->ls();
    EXPECT_NE(expectReturnValue[0], "upadhyay");
    EXPECT_NE(expectReturnValue[1], "harsh");
}

TEST_F(CommandLineTestFixture, whenUseFindFunctionWithInputThenItReturnCorrectOutput) // result of Find encounter in find function and then return true from there so the actual find function is not completed
{
    std::vector<std::string> result;
    std::string pathName = "";
    std::string name = "Harsh";

    EXPECT_CALL(*mockDirectoryObj, getSubdirectory("Harsh"))
        .Times(1)
        .WillOnce(Return(rootPointToNull));

    EXPECT_CALL(*mockDirectoryObj, resultOfFind(name, &result, pathName))
        .Times(1)
        .WillOnce(Return(true));

    int expectValue = cmdObj->find(mockDirectoryObj, name, &result, pathName);
    int actualValue = 1;
    EXPECT_NE(expectValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseFindFunctionWithWrongInputThenItReturnIncorrectOutput) // result of Find encounter in find function and then return true from there so the actual find function is not completed
{
    std::vector<std::string> result;
    std::string pathName = "";
    std::string name = "Harsh";
    root->setName("Directory");

    EXPECT_CALL(*mockDirectoryObj, getSubdirectory("Harsh"))
        .Times(1)
        .WillOnce(Return(root));

    EXPECT_CALL(*mockDirectoryObj, resultOfFind(name, &result, pathName))
        .Times(1)
        .WillOnce(Return(false));

    int expectValue = cmdObj->find(mockDirectoryObj, name, &result, pathName);
    int actualValue = 2;
    EXPECT_EQ(expectValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseUpdateFunctionThenItSuccessfullyUpdateName)
{
    std::string currentName = "hello";
    std::string newName = "hello2";

    EXPECT_CALL(*mockDirectoryObj, getSubdirectory("hello"))
        .Times(2)
        .WillRepeatedly(Return(root));

    EXPECT_CALL(*mockDirectoryObj, updateName(currentName, newName))
        .Times(1)
        .WillOnce(Return(true));

    int expectedValue = cmdObj->update(mockDirectoryObj, currentName, newName);
    int actualValue = 1;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseUpdateFunctionButMockMethodReturnFalseThenItSuccessfullyReturnCorrectOutput)
{
    std::string currentName = "hello";
    std::string newName = "hello2";

    EXPECT_CALL(*mockDirectoryObj, getSubdirectory("hello"))
        .Times(2)
        .WillRepeatedly(Return(root));

    EXPECT_CALL(*mockDirectoryObj, updateName(currentName, newName))
        .Times(1)
        .WillOnce(Return(false));

    int expectedValue = cmdObj->update(mockDirectoryObj, currentName, newName);
    int actualValue = 2;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseRmdirFunctionThenItReturnCorrectOutput)
{
    std::string name = "hello";
    EXPECT_CALL(*mockDirectoryObj, getSubdirectory(name))
        .Times(2)
        .WillRepeatedly(Return(root));
    EXPECT_CALL(*mockDirectoryObj, removeDir(name))
        .Times(1)
        .WillOnce(Return(true));

    int expectedValue = cmdObj->rmdir(mockDirectoryObj, name);
    int actualValue = 1;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseRmdirFunctionThenItReturnCorrectOutputAsDirectoryNotFound)
{
    std::string name = "hello";
    EXPECT_CALL(*mockDirectoryObj, getSubdirectory(name))
        .Times(2)
        .WillRepeatedly(Return(rootPointToNull));

    int expectedValue = cmdObj->rmdir(mockDirectoryObj, name);
    int actualValue = 2;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseRmdirFunctionWithWrongInputThenItReturnCorrectOutput)
{
    std::string name = "dir1";
    EXPECT_CALL(*mockDirectoryObj, getSubdirectory(name))
        .Times(2)
        .WillRepeatedly(Return(rootPointToNull));

    int expectedValue = cmdObj->rmdir(mockDirectoryObj, name);
    int actualValue = 3;

    EXPECT_NE(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseRmFunctionThenItReturnCorrectOutput)
{
    std::string name = "dir1";
    EXPECT_CALL(*mockDirectoryObj, getSubdirectory(name))
        .Times(2)
        .WillRepeatedly(Return(rootPointToNull));

    EXPECT_CALL(*mockDirectoryObj, removeFile(name))
        .Times(1)
        .WillOnce(Return(true));

    int expectedValue = cmdObj->rm(mockDirectoryObj, name);
    int actualValue = 1;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseRmFunctionButFileNotFoundThenItReturnCorrectOutput)
{
    std::string name = "dir1";

    EXPECT_CALL(*mockDirectoryObj, getSubdirectory(name))
        .Times(2)
        .WillRepeatedly(Return(rootPointToNull));

    EXPECT_CALL(*mockDirectoryObj, removeFile(name))
        .Times(1)
        .WillOnce(Return(false));

    int expectedValue = cmdObj->rm(mockDirectoryObj, name);
    int actualValue = 2;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseGetFileForViFunctionThenItReturnCorrectOutput)
{
    std::string name = "dir1";

    EXPECT_CALL(*mockDirectoryObj, getSubdirectory(name))
        .Times(2)
        .WillRepeatedly(Return(rootPointToNull));

    file->setName("dir1");
    EXPECT_CALL(*mockDirectoryObj, getFileByName(name))
        .Times(2)
        .WillRepeatedly(Return(file));

    std::shared_ptr<File> expectedFile = cmdObj->getFileForViAndUpdate(mockDirectoryObj, name);
    std::string actualFile = "dir1";
    EXPECT_EQ(expectedFile->getName(), actualFile);
}

TEST_F(CommandLineTestFixture, whenUseGetFileForViFunctionButFileNotFoundThenItReturnCorrectOutput)
{
    std::string name = "dir1";

    EXPECT_CALL(*mockDirectoryObj, getSubdirectory(name))
        .Times(2)
        .WillRepeatedly(Return(root));

    std::shared_ptr<File> expectedFile = cmdObj->getFileForViAndUpdate(mockDirectoryObj, name);
    EXPECT_EQ(expectedFile, nullptr);
}

TEST_F(CommandLineTestFixture, whenUseCatFunctionThenItReturnCorrectOutput)
{
    std::string name = "dir1";
    std::string result;

    EXPECT_CALL(*mockDirectoryObj, getSubdirectory(name))
        .Times(2)
        .WillRepeatedly(Return(rootPointToNull));

    EXPECT_CALL(*mockDirectoryObj, getFileByName(name))
        .Times(2)
        .WillRepeatedly(Return(file));

    int expectedValue = cmdObj->cat(mockDirectoryObj, name, result);
    int actualValue = 1;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseCatFunctionButFileNotFoundThenItReturnCorrectOutput)
{
    std::string name = "dir1";
    std::string result;

    EXPECT_CALL(*mockDirectoryObj, getSubdirectory(name))
        .Times(2)
        .WillRepeatedly(Return(rootPointToNull));

    EXPECT_CALL(*mockDirectoryObj, getFileByName(name))
        .Times(2)
        .WillRepeatedly(Return(filePointToNull));

    int expectedValue = cmdObj->cat(mockDirectoryObj, name, result);
    int actualValue = 2;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseDeleteFileContentFunctionThenContentDeletedSuccessfully)
{
    std::string name = "dir1";

    EXPECT_CALL(*mockDirectoryObj, getSubdirectory(name))
        .Times(2)
        .WillRepeatedly(Return(rootPointToNull));

    EXPECT_CALL(*mockDirectoryObj, getFileByName(name))
        .Times(2)
        .WillRepeatedly(Return(file));

    int expectedValue = cmdObj->deleteFileContent(mockDirectoryObj, name);
    int actualValue = 1;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(CommandLineTestFixture, whenUseDeleteFileContentFunctionButFileNotFoundThenItReturnCorrectOutput)
{
    std::string name = "dir1";

    EXPECT_CALL(*mockDirectoryObj, getSubdirectory(name))
        .Times(2)
        .WillRepeatedly(Return(rootPointToNull));

    EXPECT_CALL(*mockDirectoryObj, getFileByName(name))
        .Times(2)
        .WillRepeatedly(Return(filePointToNull));

    int expectedValue = cmdObj->deleteFileContent(mockDirectoryObj, name);
    int actualValue = 2;
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(DirectoryTestFixture, whenSetNameThenGetCorrectName)
{
    dirObj->setName("DirName");
    std::string expectedString = dirObj->getName();
    std::string actualString = "DirName";
    EXPECT_EQ(expectedString, actualString);
}

TEST_F(DirectoryTestFixture, whenSetDifferentNameThenGetIncorrectName)
{
    dirObj->setName("DirName");
    std::string expectedString = dirObj->getName();
    std::string actualString = "incorrectDirName";
    EXPECT_NE(expectedString, actualString);
}

TEST_F(DirectoryTestFixture, WhenAddFileAndDirectoryThenItAddedSuccessfullyCheckByGetFileAndDirectoryListByNameAndSize)
{
    dirObj->addDirectory("Dir");
    dirObj->addFile("file");
    dirObj->addDirectory("Dir2");
    std::vector<std::string> returnVector = dirObj->getFileAndDirectoryList();
    EXPECT_EQ(returnVector.size(), 3);
    EXPECT_EQ(returnVector[0], "Dir/");
    EXPECT_EQ(returnVector[1], "Dir2/");
    EXPECT_EQ(returnVector[2], "file");
}

TEST_F(DirectoryTestFixture, WhenAddWrongNameFileAndDirectoryThenItAddedWronglyCheckByGetFileAndDirectoryListByName)
{
    dirObj->addDirectory("Dir");
    dirObj->addFile("file");
    dirObj->addDirectory("Dir2");
    std::vector<std::string> returnVector = dirObj->getFileAndDirectoryList();
    EXPECT_NE(returnVector[0], "Dir");
    EXPECT_NE(returnVector[1], "Dir2");
    EXPECT_NE(returnVector[2], "file/");
}

TEST_F(DirectoryTestFixture, WhenAddFileAndDirectoryThenItAddedSuccessfullyCheckByGetFileAndDirectoryList)
{
    dirObj->addDirectory("Dir");
    dirObj->addFile("file");
    dirObj->addDirectory("Dir2");
    std::vector<std::string> returnVector = dirObj->getFileAndDirectoryList();
    EXPECT_EQ(returnVector[0], "Dir/");
    EXPECT_NE(returnVector[1], "file");
    EXPECT_NE(returnVector[2], "Dir2/");
}

TEST_F(DirectoryTestFixture, WhenAddDirectoryAndFindItThenResultOfFindGiveCorrectOutput)
{
    dirObj->addDirectory("Directory");
    std::string name = "Directory";
    std::string pathName = "";
    bool isFind = dirObj->resultOfFind(name, &result, pathName);

    EXPECT_TRUE(isFind);
    std::string expectedString = result[0];
    std::string actualString = "Directory";
    EXPECT_EQ(expectedString, actualString);
}

TEST_F(DirectoryTestFixture, WhenAddDirectoryInRootAndFindItThenResultOfFindGiveCorrectOutput)
{
    dirObj->addDirectory("Fol");

    std::string name = "Fol";
    std::string pathName = "root/";

    bool isFind = dirObj->resultOfFind(name, &result, pathName);

    std::string expectedString = result[0];
    std::string actualString = "root/Fol";
    EXPECT_TRUE(isFind);
    EXPECT_EQ(expectedString, actualString);
}

TEST_F(DirectoryTestFixture, WhenAddFileAndFindItThenResultOfFindGiveCorrectOutput)
{
    dirObj->addFile("File");
    std::string name = "File";
    std::string pathName = "";

    bool isFind = dirObj->resultOfFind(name, &result, pathName);

    std::string expectedString = result[0];
    std::string actualString = "File";
    EXPECT_TRUE(isFind);
    EXPECT_EQ(expectedString, actualString);
}

TEST_F(DirectoryTestFixture, WhenUpdateDirectoryNameThenItGotUpdatedSuccessfully)
{
    dirObj->addDirectory("myDir1");

    bool isUpdated = dirObj->updateName("myDir1", "newMyDir2");

    std::string name = "newMyDir2";
    std::string pathName = "";

    bool isFind = dirObj->resultOfFind(name, &result, pathName);

    std::string expectedString = result[0];
    std::string actualString = "newMyDir2";

    EXPECT_TRUE(isUpdated);
    EXPECT_TRUE(isFind);
    EXPECT_EQ(expectedString, actualString);
}

TEST_F(DirectoryTestFixture, WhenRemoveOneDirectoryThenItRemoveSuccessfully)
{
    std::string name = "myDir1";
    std::string pathName = "";

    dirObj->addDirectory("myDir1");

    bool isFind = dirObj->resultOfFind(name, &result, pathName);
    std::string expectedString = result[0];
    std::string actualString = "myDir1";
    EXPECT_TRUE(isFind);
    EXPECT_EQ(expectedString, actualString);

    bool isRemoved = dirObj->removeDir("myDir1");
    EXPECT_TRUE(isRemoved);

    std::vector<std::string> result2;
    bool isFind2 = dirObj->resultOfFind(name, &result2, pathName);
    int expectedValue = result2.size();
    int actualValue = 0;

    EXPECT_FALSE(isFind2);
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(DirectoryTestFixture, WhenRemoveDirectoryWithWrongNameThenItIsNotRemove)
{
    std::string name = "myDir1";
    std::string pathName = "";

    dirObj->addDirectory("myDir1");

    bool isFind = dirObj->resultOfFind(name, &result, pathName);

    std::string expectedString = result[0];
    std::string actualString = "myDir1";
    EXPECT_TRUE(isFind);
    EXPECT_EQ(expectedString, actualString);

    bool isRemoved = dirObj->removeDir("myDir1asjdkh");

    std::vector<std::string> result2;
    bool isFind2 = dirObj->resultOfFind(name, &result2, pathName);
    int expectedValue = result2.size();
    int actualValue = 0;
    EXPECT_TRUE(isFind2);
    EXPECT_NE(expectedValue, actualValue);
}

TEST_F(DirectoryTestFixture, WhenRemoveFileThenItIsRemoveSuccessfully)
{
    std::string name = "myDir1";
    std::string pathName = "";

    dirObj->addFile("myDir1");

    bool isFind = dirObj->resultOfFind(name, &result, pathName);
    std::string expectedString = result[0];
    std::string actualString = "myDir1";
    EXPECT_TRUE(isFind);
    EXPECT_EQ(expectedString, actualString);

    bool isRemoved = dirObj->removeFile("myDir1");
    EXPECT_TRUE(isRemoved);

    std::vector<std::string> result2;
    bool isFind2 = dirObj->resultOfFind(name, &result2, pathName);
    int expectedValue = result2.size();
    int actualValue = 0;
    EXPECT_FALSE(isFind2);
    EXPECT_EQ(expectedValue, actualValue);
}

TEST_F(DirectoryTestFixture, WhenRemoveFileWithWrongNameThenItIsNotRemove)
{
    std::string name = "myDir1";
    std::string pathName = "";

    dirObj->addFile("myDir1");

    bool isFind = dirObj->resultOfFind(name, &result, pathName);
    EXPECT_TRUE(isFind);

    std::string expectedString = result[0];
    std::string actualString = "myDir1";
    EXPECT_EQ(expectedString, actualString);

    bool isRemoved = dirObj->removeFile("myDir1WrongName");

    std::vector<std::string> result2;
    bool isFind2 = dirObj->resultOfFind(name, &result2, pathName);
    int expectedValue = result2.size();
    int actualValue = 0;

    EXPECT_TRUE(isFind2);
    EXPECT_NE(expectedValue, actualValue);
}

TEST_F(FileTestFixture, whenSetNameThenGetCorrectName)
{
    fileObj->setName("File1");
    std::string expectedString = fileObj->getName();
    std::string actualString = "File1";
    EXPECT_EQ(expectedString, actualString);
}

TEST_F(FileTestFixture, whenSetIncorrectNameThenGetIncorrectName)
{
    fileObj->setName("File1");
    std::string expectedString = fileObj->getName();
    std::string actualString = "incorrectName";
    EXPECT_NE(expectedString, actualString);
}

TEST_F(FileTestFixture, whenSetCorrectFileContentThenGetCorrectContent)
{
    fileObj->setName("File1");
    fileObj->setNewFileContent("new content");
    std::string actualString = "new content";
    std::string expectedString = fileObj->getFileContent();
    EXPECT_EQ(expectedString, actualString);
}

TEST_F(FileTestFixture, whenSetIncorrectContentThenGetIncorrectContent)
{
    fileObj->setName("File1");
    fileObj->setNewFileContent("new content");
    std::string actualString = " new content";
    std::string expectedString = fileObj->getFileContent();
    EXPECT_NE(expectedString, actualString);
}

TEST_F(FileTestFixture, whenUpdateFileContentThenContentUpdatedSuccessfully)
{
    fileObj->setName("File1");
    fileObj->setNewFileContent("new content");
    fileObj->updateContent("extra content");
    std::string actualString = "new content extra content";
    std::string expectedString = fileObj->getFileContent();
    EXPECT_EQ(expectedString, actualString);
}

TEST_F(FileTestFixture, whenUpdateIncorrectFileContentThenGetIncorrectContent)
{
    fileObj->setName("File1");
    fileObj->setNewFileContent(" new content");
    fileObj->updateContent("extra content");
    std::string actualString = "new content extra content";
    std::string expectedString = fileObj->getFileContent();
    EXPECT_NE(expectedString, actualString);
}

TEST_F(FileTestFixture, whenDeleteFileContentThenGotDeletedSuccessfully)
{
    fileObj->setName("File1");
    fileObj->setNewFileContent(" new content");
    fileObj->deleteContent();
    std::string actualString = "";
    std::string expectedString = fileObj->getFileContent();
    EXPECT_EQ(expectedString, actualString);
}

TEST_F(FileTestFixture, whenDeleteFileContentThenGetEmptyFile)
{
    fileObj->setName("File1");
    fileObj->setNewFileContent(" new content");
    fileObj->deleteContent();
    std::string actualString = "new content";
    std::string expectedString = fileObj->getFileContent();
    EXPECT_NE(expectedString, actualString);
}

TEST_F(FileTestFixture, whenDeleteFileContentAfterUpdateFileContentThenGotDeletedSuccessfully)
{
    fileObj->setName("File1");
    fileObj->setNewFileContent("content");
    fileObj->updateContent("+ new content");
    EXPECT_EQ(fileObj->getFileContent(), "content + new content");
    fileObj->deleteContent();
    std::string actualString = "";
    std::string expectedString = fileObj->getFileContent();
    EXPECT_EQ(expectedString, actualString);
}

TEST_F(FileTestFixture, whenDeleteFileContentAfterUpdateFileContentThenGetEmptyFile)
{
    fileObj->setName("File1");
    fileObj->setNewFileContent("new content");
    fileObj->updateContent("+ new content");
    fileObj->deleteContent();
    std::string actualString = "new content + new content";
    std::string expectedString = fileObj->getFileContent();
    EXPECT_EQ(expectedString.size(), 0);
    EXPECT_NE(expectedString, actualString);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}