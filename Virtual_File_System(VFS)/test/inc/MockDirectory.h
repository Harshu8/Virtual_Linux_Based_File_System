#pragma once
#include "CommandLine.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::_;
using ::testing::Return;

class MockDirectory : public IDirectory
{
public:
    std::vector<std::string> stringVectorTemp;
    std::shared_ptr<IDirectory> r1;
    std::shared_ptr<File> r2;
    std::string stringTemp;
    MockDirectory()
    {
        ON_CALL(*this, getName()).WillByDefault(Return(stringTemp));
        ON_CALL(*this, setName(_)).WillByDefault(Return(true));
        ON_CALL(*this, addFile(_)).WillByDefault(Return(true));
        ON_CALL(*this, addDirectory(_)).WillByDefault(Return(true));
        ON_CALL(*this, getSubdirectory(_)).WillByDefault(Return(r1));
        ON_CALL(*this, getFileByName(_)).WillByDefault(Return(r2));
        ON_CALL(*this, getFileAndDirectoryList()).WillByDefault(Return(stringVectorTemp));
        ON_CALL(*this, resultOfFind(_,_,_)).WillByDefault(Return(true));
        ON_CALL(*this, updateName(_,_)).WillByDefault(Return(true));
        ON_CALL(*this, removeDir(_)).WillByDefault(Return(true));
        ON_CALL(*this, removeFile(_)).WillByDefault(Return(true));
    }

        MOCK_METHOD0(getName, std::string());
        MOCK_METHOD1(setName, bool(const std::string &)); // not used in commandLine Class so not test it as mock it
        MOCK_METHOD1(addFile, bool(const std::string &));
        MOCK_METHOD1(addDirectory, bool(const std::string &));
        MOCK_METHOD1(getSubdirectory, std::shared_ptr<IDirectory>(const std::string &));
        MOCK_METHOD1(getFileByName, std::shared_ptr<File>(const std::string &));
        MOCK_METHOD0(getFileAndDirectoryList, std::vector<std::string>());
        MOCK_METHOD3(resultOfFind, bool(const std::string &, std::vector<std::string> *, std::string ));
        MOCK_METHOD2(updateName, bool(const std::string &, const std::string &));
        MOCK_METHOD1(removeDir, bool(const std::string &));
        MOCK_METHOD1(removeFile, bool(const std::string &));
};