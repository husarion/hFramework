#include <gtest/gtest.h>

#include "../include/hUtils.h"

#include <string>

using namespace std;

string log;

int sys_log_data(const char* buf, int len)
{
	log += string(buf, len);
	return len;
}

#define TEST_SYS_LOG_DATA_CRNL(str,expected) { log = ""; sys_log_data_crnl(str,strlen(str)); EXPECT_STREQ(log.c_str(),expected); EXPECT_EQ(log.size(),strlen(expected)); }

TEST(utils, sys_log_data_crnl)
{
	TEST_SYS_LOG_DATA_CRNL("", "");
	TEST_SYS_LOG_DATA_CRNL("a", "a");
	TEST_SYS_LOG_DATA_CRNL("\r", "\r");
	TEST_SYS_LOG_DATA_CRNL("\ra", "\ra");
	TEST_SYS_LOG_DATA_CRNL("\na", "\n\ra");
	TEST_SYS_LOG_DATA_CRNL("\n", "\n\r");
	TEST_SYS_LOG_DATA_CRNL("ABC\n", "ABC\n\r");
	TEST_SYS_LOG_DATA_CRNL("\r\n", "\r\n\r");
	TEST_SYS_LOG_DATA_CRNL("\n\n", "\n\r\n\r");
	TEST_SYS_LOG_DATA_CRNL("\n\r\n", "\n\r\r\n\r");
	TEST_SYS_LOG_DATA_CRNL("a\nb\nc", "a\n\rb\n\rc");
	TEST_SYS_LOG_DATA_CRNL("\nac\r\n", "\n\rac\r\n\r");
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
