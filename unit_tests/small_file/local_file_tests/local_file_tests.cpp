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
	test_file.write(0, _100bytes.c_str());

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
	int char_written = test_file.write(450, _100bytes.c_str());

	fstream check(string(file_prefix) + file_name, ios::out | ios::in | ios::binary);
	if (!check.is_open())
		FAIL() << "File not created!";

	file_buf << check.rdbuf();
	file_buf.seekg(450, ios::beg);
	char test[100] = "";
	file_buf.read(test, sizeof(test));

	EXPECT_EQ(50, char_written) << "Characters written incorrectly returned";
	EXPECT_EQ(_100bytes.substr(0, 50).c_str(), test) << "Incorrect string written to file";
	EXPECT_EQ(file::max_size, (int)test_file.check_file_size()) << "File size beyond maximum";

	check.close();
	test_file.close_file();
	remove((string(file_prefix) + file_name).c_str());
}

TEST(LocalFileTests, FileOutOfBoundsWriteTest)
{
	string file_name = "FileOutOfBoundsWriteTest.txt";

	file test_file(file_name, file::Mode::WRITE_ONLY);
	int char_written = test_file.write(750, _500bytes.c_str());
	int length = static_cast<int>(test_file.check_file_size());

	EXPECT_EQ(0, char_written) << "Characters written to full file";
	EXPECT_EQ(file::max_size, length) << "Size exceeded max file size or decreased than max";

	test_file.close_file();
	remove((string(file_prefix) + file_name).c_str());
}
