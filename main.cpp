#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class DBConnectionInterface
{
public:
    DBConnectionInterface()
    {}

    virtual ~DBConnectionInterface()
    {}

    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual int execQuery(const char*) = 0;
};

class DBConnection : public DBConnectionInterface
{
public:
    DBConnection() {};
    ~DBConnection() {};

    bool open()
    {
        return true;
    }

    bool close()
    {
        return true;
    }

    int execQuery(const char* str)
    {
        if (strcmp(str, "command") == 0)
        {
            // smth command
            return 0;
        }
        else if (strcmp(str, "end") == 0)
        {
            // return end code
            return -1;
        }
        else return 666; // error code
    }

private:

};

class MockDBConnection : public DBConnectionInterface
{
public:
    MOCK_METHOD(bool, open, (), (override));
    MOCK_METHOD(bool, close, (), (override));
    MOCK_METHOD(int, execQuery, (const char*), (override));
};

class ClassThatUsesDB
{
public:
    ClassThatUsesDB() {};
    ~ClassThatUsesDB() = default;

    bool openConnection(DBConnectionInterface& connect)
    {
        return connect.open();
    }

    int useConnection(DBConnectionInterface& connect, const char* str)
    {
        return connect.execQuery(str);
    }

    bool closeConnection(DBConnectionInterface& connect)
    {
        return connect.close();
    }

private:
    //DBConnectionInterface *_connect;
};

// ���� �� �����
//class SomeTestSuite : public ::testing::Test
//{
//protected:
//    void SetUp()
//    {
//        //someProtocol = new SomeRealProtocol();
//        someDBConnection = new DBConnectionInterface();
//    }
//
//    void TearDown()
//    {
//        //delete someProtocol;
//        delete someDBConnection;
//    }
//
//protected:
//    //SomeProtocolInterface* someProtocol;
//    DBConnectionInterface* someDBConnection;
//};

//���� �� ��������\�������� � �������������� ���-��������
TEST(SomeTestSuite, test_open_close)
{
    ClassThatUsesDB* someClassThatUsesDB = new ClassThatUsesDB;

    //������� ���-������
    MockDBConnection* someDBConnection = new MockDBConnection;

    EXPECT_CALL(*someDBConnection, open).WillOnce(::testing::Return(true));
    EXPECT_CALL(*someDBConnection, close).WillOnce(::testing::Return(true));

    //��������� �������� �� ���������
    someClassThatUsesDB->openConnection(*someDBConnection);
    someClassThatUsesDB->closeConnection(*someDBConnection);

    delete someClassThatUsesDB;
    delete someDBConnection;
}

//���� �� ����� � �������������� ���-��������
TEST(SomeTestSuite, query)
{
    ClassThatUsesDB* someClassThatUsesDB = new ClassThatUsesDB();

    DBConnection* someDBConnection = new DBConnection();

    int error_code_1 = INT_MAX;
    int error_code_2 = INT_MAX;
    int error_code_3 = INT_MAX;

    //��������� �������� �� ���������
    error_code_1 = someClassThatUsesDB->useConnection(*someDBConnection, "command");
    error_code_2 = someClassThatUsesDB->useConnection(*someDBConnection, "end");
    error_code_3 = someClassThatUsesDB->useConnection(*someDBConnection, "abcd");

    ASSERT_EQ(error_code_1, 0);
    ASSERT_EQ(error_code_2, -1);
    ASSERT_EQ(error_code_3, 666);

    delete someClassThatUsesDB;
    delete someDBConnection;
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}