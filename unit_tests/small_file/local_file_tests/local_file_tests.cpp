#include <iostream>
#include <string>
#include <fstream>
#include "gtest/gtest.h"
#include "local_file.hpp"
using namespace std;
using namespace small_file::local_file;

const char* file_prefix = "../test_files/";
string _100bytes = "HelloWuEC7wOv238xglW75H8ZoCTPRcoudsY1RqB96XC"
						"ZnmMKp3BXb2FYCWbI1sWgPRuv3OeYSuUGz7UzXJMxEy"
						"Z9DlVdOVqnJA6";
string _500bytes = "dDvxR48Ubch5Z2LgDyrOTsqiEugtLd5oKh6dj0evcv5eTvVqj"
					"i3JvA2wlt8UCtEUEtxKybEKF5SL2l7vmOgXm3q3jJDha36pM"
					"fwYSiG1sgJAMDj6qoNv9tn4cQqFKwi9otgBctWGrZXY73Z3Z"
					"4iAJbz7zy5DWi2mALsR3C70wggSxGLberWDZMAmslCTdYwST"
					"qLdLs22j95VcYR4OaXR8iU8kBcvFYABH7snKf3xHH2G1tDbS"
					"GseTv6JlSpzReYh2EuI0rNCPsIja4mf1QEDItFssmdkHb3Og"
					"97511RpO2UhXbsojBqUc6j2U51mZn6OhpVDINeiy00GrE0d7"
					"PI7S0Vm5t5a85xa9rOrVmMVdZMbwVRd2VWfNH7dwDb2njZNU"
					"LZKMuwXEUwxEv0m5cEzkJulAX21F42UzdqTUhIsmgh5UlwEl"
					"rqpFDrblXVeCIOHjVmyeamrlWnwuRfgEOhYXEMlIzeKUOriE"
					"5sRNhbDe2VsQPxHrQbQ";
// int file_maxsize = small_file::local_file::file::max_size;

TEST(LocalFileTests, ExisitingFileOpenTest)
{
	string file_name = "ExisitingFileOpenTest.txt";

	fstream test_file(string(file_prefix) + file_name, ios::out);
	test_file.close();

	file check(file_name, file::Mode::READ_ONLY);
	EXPECT_TRUE(check.file_is_open()) << "Failed to open existing file";

	check.close_file();
	remove((string(file_prefix) + file_name).c_str());
}

TEST(LocalFileTests, CreateAndOpenNewFileTest)
{
	string file_name = "CreateAndOpenNewFileUsingStringTest.txt";

	file check(file_name, file::Mode::WRITE_ONLY);

	fstream test_file(string(file_prefix) + file_name, ios::out | ios::in);
	EXPECT_TRUE(test_file.is_open()) << "Failed to create new file";

	check.close_file();
	test_file.close();
	remove((string(file_prefix) + file_name).c_str());
}

TEST(LocalFileTests, CheckFileSizeTest)
{
	string file_name = "CheckFileSizeTest.txt";

	fstream dump_file(string(file_prefix) + file_name, ios::out);
	dump_file << _100bytes;
	dump_file.close(); 

	file test_file(file_name, file::Mode::READ_WRITE);
	int length = static_cast<int>(test_file.check_file_size());
	EXPECT_EQ(file::max_size, length) << "Incorrect file size after writing 100 bytes";

	dump_file.open(string(file_prefix) + file_name, ios::out | ios::trunc);
	dump_file << _500bytes;
	dump_file.close();

	length = static_cast<int>(test_file.check_file_size());
	EXPECT_EQ(file::max_size, length) << "Incorrect file size after writing 500 bytes";

	test_file.close_file();
	remove((string(file_prefix) + file_name).c_str());
}

TEST(LocalFileTests, WriteFullStringToFileTest)
{
	string file_name = "WriteStringToFileTest.txt";
	stringstream file_buf;

	file test_file(file_name, file::Mode::WRITE_ONLY);
	test_file.seekPut(0, file::Dir::BEG);
	test_file.write(_100bytes.size(), _100bytes.c_str());

	fstream check(string(file_prefix) + file_name, ios::out | ios::in | ios::binary);
	if (!check.is_open())
		FAIL() << "File not created!";
	
	file_buf << check.rdbuf();
	char test[110] = "";
	file_buf.read(test, 100);
	EXPECT_EQ(_100bytes, string(test)) << "Wrong string written";
	EXPECT_EQ(file::max_size, (int)test_file.check_file_size()) << "File size beyond maximum";

	test_file.close_file();
	check.close();
	remove((string(file_prefix) + file_name).c_str());
}

TEST(LocalFileTests, FileWriteBeyondMaxSizeTest)
{
	string file_name = "FileWriteBeyondMaxSizeTest.txt";
	stringstream file_buf;

	file test_file(file_name, file::Mode::WRITE_ONLY);
	test_file.seekPut(450, file::Dir::BEG);
	int char_written = test_file.write(_100bytes.size(), _100bytes.c_str());

	fstream check(string(file_prefix) + file_name, ios::out | ios::in | ios::binary);
	if (!check.is_open())
		FAIL() << "File not created!";

	file_buf << check.rdbuf();
	file_buf.seekg(450, ios::beg);
	char test[100] = "";
	file_buf.read(test, sizeof(test));

	EXPECT_EQ(50, char_written) << "Characters written incorrectly returned";
	EXPECT_STREQ(_100bytes.substr(0, 50).c_str(), test) << "Incorrect string written to file";
	EXPECT_EQ(file::max_size, (int)test_file.check_file_size()) << "File size beyond maximum";

	check.close();
	test_file.close_file();
	remove((string(file_prefix) + file_name).c_str());
}

TEST(LocalFileTests, FileOutOfBoundsWriteTest)
{
	string file_name = "FileOutOfBoundsWriteTest.txt";

	file test_file(file_name, file::Mode::WRITE_ONLY);
	test_file.seekPut(0, file::Dir::END);
	int char_written = test_file.write(750, _500bytes.c_str());
	int length = static_cast<int>(test_file.check_file_size());

	EXPECT_EQ(0, char_written) << "Characters written to full file";
	EXPECT_EQ(file::max_size, length) << "Size exceeded max file size or decreased than max";

	test_file.close_file();
	remove((string(file_prefix) + file_name).c_str());
}

TEST(LocalFileTests, FileReadTest)
{
	string fileName = "FileReadTest.txt";
	fstream dump(string(file_prefix) + fileName, ios::out);
	dump << _500bytes;
	dump.close();

	file testFile(fileName, file::Mode::READ_ONLY);
	EXPECT_TRUE(testFile.file_is_open()) << "File did not open correctly";

	char readBuffer[510] = "";
	testFile.seekGet(0, file::Dir::BEG);
	testFile.read(500, readBuffer);

	EXPECT_STREQ(readBuffer, _500bytes.c_str()) << "String not correctly read from file";

	testFile.close_file();
	remove((string(file_prefix) + fileName).c_str());
}

TEST(LocalFileTests, FileReadBeyondMaxTest)
{
	string fileName = "FileReadBeyondMaxTest.txt";
	fstream dump(string(file_prefix) + fileName, ios::out);
	dump << _500bytes;
	dump.close();

	file testFile(fileName, file::Mode::READ_ONLY);
	EXPECT_TRUE(testFile.file_is_open()) << "File did not open correctly";

	char readBuffer[201] = "";
	testFile.seekGet(300, file::Dir::BEG);
	testFile.read(500, readBuffer);

	EXPECT_STREQ(readBuffer, (_500bytes.substr(300, 200)).c_str()) << "String not correctly read from file";

	testFile.close_file();
	remove((string(file_prefix) + fileName).c_str());
}

TEST(LocalFileTests, TellAndSeekGetTest)
{
	string fileName = "SeekGetTest.txt";
	char testBuffer[501] = "";
	char checkBuffer[501] = "";
	
	file test(fileName, file::Mode::WRITE_ONLY);
	test.close_file();

	test.open_file(fileName, file::Mode::READ_ONLY);
	fstream check(string(file_prefix) + fileName, ios::in);
	EXPECT_TRUE(check.is_open()) << "File not opened/created!";

	test.seekGet(45, file::Dir::BEG);
	EXPECT_EQ(test.tellGet(), 45) << "Seek pointer not set correctly";
	check.seekg(45, ios::beg);
	
	test.read(50, testBuffer);
	check.read(checkBuffer, 50);
	EXPECT_STREQ(testBuffer, checkBuffer) << "Not the same string!";

	test.seekGet(100, file::Dir::CUR);
	EXPECT_EQ(test.tellGet(), 100 + 45 + 50) << "Seek pointer not set correctly";
	check.seekg(100, ios::cur);

	test.read(60, testBuffer);
	check.read(checkBuffer, 60);
	EXPECT_STREQ(testBuffer, checkBuffer) << "Not the same string!";

	test.seekGet(-75, file::Dir::END);
	EXPECT_EQ(test.tellGet(), 500 - 75) << "Seek pointer not set correctly";
	check.seekg(-75, ios::end);

	test.read(75, testBuffer);
	check.read(checkBuffer, 75);
	EXPECT_STREQ(testBuffer, checkBuffer) << "Not the same string!";

	test.close_file();
	check.close();
	remove((string(file_prefix) + fileName).c_str());
}

TEST(LocalFileTests, TellAndSeekPutTest)
{
	string fileName = "SeekPutTest.txt";
	char checkBuffer[501] = "";
	
	file test(fileName, file::Mode::WRITE_ONLY);
	test.close_file();

	test.open_file(fileName, file::Mode::READ_WRITE);
	fstream check(string(file_prefix) + fileName, ios::in);
	EXPECT_TRUE(check.is_open()) << "File not opened/created!";

	test.seekPut(45, file::Dir::BEG);
	EXPECT_EQ(test.tellPut(), 45) << "Put pointer not set correctly";
	check.seekg(45, ios::beg);
	
	test.write(50, _100bytes.c_str());
	check.read(checkBuffer, 50);
	EXPECT_STREQ(checkBuffer, (_100bytes.substr(0, 50).c_str())) << "Not the same string!";

	test.seekPut(100, file::Dir::CUR);
	EXPECT_EQ(test.tellPut(), 100 + 45 + 50) << "Put pointer not set correctly";
	check.seekg(100, ios::cur);

	test.write(60, _100bytes.c_str());
	check.read(checkBuffer, 60);
	EXPECT_STREQ(checkBuffer, (_100bytes.substr(0, 60).c_str())) << "Not the same string!";

	test.seekPut(-75, file::Dir::END);
	EXPECT_EQ(test.tellPut(), 500 - 75) << "Put pointer not set correctly";
	check.seekg(-75, ios::end);

	test.write(75, _500bytes.c_str());
	check.read(checkBuffer, 75);
	EXPECT_STREQ(checkBuffer, (_500bytes.substr(0, 75).c_str())) << "Not the same string!";
}