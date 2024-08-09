#include <iostream>
#include <string>
#include <fstream>
#include "gtest/gtest.h"
#include "file_manager.hpp"
using namespace std;
using namespace small_file::local_file;
using namespace large_file::file_manager;

const char* file_prefix = "../test_files/";
const string _600bytes = "dDvxR48Ubch5Z2LgDyrOTsqiEugtLd5oKh6dj0evcv5eTvVqj"
						"i3JvA2wlt8UCtEUEtxKybEKF5SL2l7vmOgXm3q3jJDha36pM"
						"fwYSiG1sgJAMDj6qoNv9tn4cQqFKwi9otgBctWGrZXY73Z3Z"
						"4iAJbz7zy5DWi2mALsR3C70wggSxGLberWDZMAmslCTdYwST"
						"qLdLs22j95VcYR4OaXR8iU8kBcvFYABH7snKf3xHH2G1tDbS"
						"GseTv6JlSpzReYh2EuI0rNCPsIja4mf1QEDItFssmdkHb3Og"
						"97511RpO2UhXbsojBqUc6j2U51mZn6OhpVDINeiy00GrE0d7"
						"PI7S0Vm5t5a85xa9rOrVmMVdZMbwVRd2VWfNH7dwDb2njZNU"
						"LZKMuwXEUwxEv0m5cEzkJulAX21F42UzdqTUhIsmgh5UlwEl"
						"rqpFDrblXVeCIOHjVmyeamrlWnwuRfgEOhYXEMlIzeKUOriE"
						"HelloWuEC7wOv238xglW75H8ZoCTPRcoudsY1RqB96XC"
						"ZnmMKp3BXb2FYCWbI1sWgPRuv3OeYSuUGz7UzXJMxEy"
						"5sRNhbDe2VsQPxHrQbQZ9DlVdOVqnJA6";

TEST(FileManagerTests, FileCreationTest)
{
	const int create_files = 4;
	int huge_file_size = 4000;  //size in bytes
	file_manager file_test(huge_file_size, file::Mode::WRITE_ONLY);
	file_test.create_files(create_files);

	fstream test_file;
	string file_name;
	for (int i = 0; i < (create_files + (huge_file_size / file::max_size) + 1); i++)
	{
		file_name = string(file_prefix);
		file_name.append("test" + to_string(i) + ".txt");
		test_file.open(file_name);

		EXPECT_TRUE(test_file.is_open()) << "Files not created by File manager class";
		test_file.close();
	}
	file_test.close_all_files();

	for (int i = 0; i < (create_files + 1 + (huge_file_size)/file::max_size); i++)
	{
		file_name = string(file_prefix);
		file_name.append("test" + to_string(i) + ".txt");

		remove(file_name.c_str());
	}
}

TEST(FileManagerTests, WritingToASingleFileTest)
{
	const string write_str = "Hello Test";
	const int pos = 50;
	string test_str(write_str);
	int test_pos = pos;

	file_manager file_test(file::Mode::WRITE_ONLY);
	file_test.write(test_str, test_str.size(), test_pos);
	file_test.close_all_files();

	string file_name = string(file_prefix) + "test0.txt";
	fstream file(file_name, ios::in | ios::out);
	EXPECT_TRUE(file.is_open()) << "File not created by file_manager";
	stringstream file_buf;
	char test[20] = "";

	file_buf << file.rdbuf();
	file_buf.seekg(pos, ios::beg);
	file_buf.read(test, strlen(write_str.c_str()));
	EXPECT_STREQ(write_str.c_str(), test) << "String not written correctly at correct postion";

	file.close();
	for (int i = 0; i < (1 + (pos + write_str.size() / file::max_size)); i++)
	{
		file_name = string(file_prefix);
		file_name.append("test" + to_string(i) + ".txt");

		remove(file_name.c_str());
	}
}

TEST(FileManagerTests, WritingTo2DifferentFilesTest)
{
	const string write_str = "Hello Test";
	const int pos = 494;
	string test_str(write_str);
	int test_pos = pos;

	file_manager file_test(file::Mode::WRITE_ONLY);
	file_test.write(test_str, test_str.size(), test_pos);
	file_test.close_all_files();

	int file_index = (pos + write_str.size()) / file::max_size;
	fstream file;
	string file_name;
	stringstream file_buf;

	for (int i = 0; i < file_index + 1; i++)
	{
		file_name = string(file_prefix);
		file_name.append("test" + to_string(i) + ".txt");
		file.open(file_name, ios::out | ios::in | ios::binary);
		file_buf << file.rdbuf();
		file.close();	
	}

	char test[20] = "";
	file_buf.seekg(pos, ios::beg);
	file_buf.read(test, strlen(write_str.c_str()));
	EXPECT_EQ(write_str, string(test)) << "Strings not written correctly";

	for (int i = 0; i < (1 + (pos + write_str.size() / file::max_size)); i++)
	{
		file_name = string(file_prefix);
		file_name.append("test" + to_string(i) + ".txt");

		remove(file_name.c_str());
	}
}

TEST(FileManagerTests, WritingTo3DifferentFilesTest)
{
	const string write_str = _600bytes;
	const int pos = 450;
	string test_str(_600bytes);
	int test_pos = pos;

	file_manager file_test(file::Mode::WRITE_ONLY);
	file_test.write(test_str, test_str.size(), test_pos);
	file_test.close_all_files();

	int file_index = (pos + write_str.size()) / file::max_size;
	fstream file;
	string file_name;
	stringstream file_buf;

	for (int i = 0; i < file_index + 1; i++)
	{
		file_name = string(file_prefix);
		file_name.append("test" + to_string(i) + ".txt");
		file.open(file_name, ios::out | ios::in | ios::binary);
		file_buf << file.rdbuf();
		file.close();
	}

	char test[700] = "";
	file_buf.seekg(pos, ios::beg);
	file_buf.read(test, strlen(write_str.c_str()));
	EXPECT_EQ(write_str, string(test)) << "Strings not written correctly";

	for (int i = 0; i < (1 + (pos + write_str.size() / file::max_size)); i++)
	{
		file_name = string(file_prefix);
		file_name.append("test" + to_string(i) + ".txt");

		remove(file_name.c_str());
	}
}

TEST(FileManagerTests, ReadingFromSingleFileTest)
{
	string fileName = "test0.txt";
	int pos = 0;
	file_manager fileCreator(499, file::Mode::WRITE_ONLY), fileTest(file::Mode::READ_ONLY);
	fstream check(string(file_prefix) + fileName, ios::in);
	char testBuffer[501] = "";
	char checkBuffer[501] = "";
	
	fileTest.open_files(1);
	fileTest.read(testBuffer, sizeof(testBuffer), pos);
	check.seekg(pos, ios::beg);
	check.read(checkBuffer, sizeof(checkBuffer) - 1);
	EXPECT_STREQ(testBuffer, checkBuffer);

	fileCreator.close_all_files();
	fileTest.close_all_files();
	check.close();

	remove((string(file_prefix) + fileName).c_str());
}

TEST(FileManagerTests, ReadingFrom2FilesTest)
{
	const int testPos = 250;
	int pos = testPos;
	file_manager fileCreator(500, file::Mode::WRITE_ONLY), fileTest(file::Mode::READ_ONLY);
	fstream check;
	stringstream fileBuf;
	char testBuffer[501] = "";
	char checkBuffer[501] = "";
	
	fileTest.open_files(2);
	fileTest.read(testBuffer, sizeof(testBuffer) - 1, pos);

	for(int i = 0; i < 2; i++)
	{
		check.open(string(file_prefix) + "test" + to_string(i) + ".txt");
		fileBuf << check.rdbuf();
		check.close();
	}

	string temp = fileBuf.str();
	fileBuf.seekg(testPos, ios::beg);
	fileBuf.read(checkBuffer, sizeof(checkBuffer) - 1);
	EXPECT_STREQ(testBuffer, checkBuffer);

	fileCreator.close_all_files();
	fileTest.close_all_files();
	check.close();

	for(int i = 0; i < 2; i++)
	{
		remove((string(file_prefix) + "test" + to_string(i) + ".txt").c_str());
	}
}

TEST(FileManagerTests, ReadingFrom3FilesTest)
{
	const int testPos = 490;
	int pos = testPos;
	file_manager fileCreator(1000, file::Mode::WRITE_ONLY), fileTest(file::Mode::READ_ONLY);
	fstream check;
	stringstream fileBuf;
	char testBuffer[701] = "";
	char checkBuffer[701] = "";
	
	fileTest.open_files(3);
	fileTest.read(testBuffer, sizeof(testBuffer) - 1, pos);

	for(int i = 0; i < 3; i++)
	{
		check.open(string(file_prefix) + "test" + to_string(i) + ".txt");
		fileBuf << check.rdbuf();
		check.close();
	}

	fileBuf.seekg(testPos, ios::beg);
	fileBuf.read(checkBuffer, sizeof(checkBuffer) - 1);
	EXPECT_STREQ(testBuffer, checkBuffer);

	fileCreator.close_all_files();
	fileTest.close_all_files();
	check.close();

	for(int i = 0; i < 3; i++)
	{
		remove((string(file_prefix) + "test" + to_string(i) + ".txt").c_str());
	}
}