#include "CppUnitTest.h"
#include "local_files.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IoTests
{
	namespace FileIoTests
	{
		const char* file_prefix = "../TestFiles/IoTest/";
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
		TEST_CLASS(LocalFilesTests)
		{
		public:

			TEST_METHOD(ExisitingFileOpenTest)
			{
				string file_name = "ExisitingFileOpenTest.txt";

				fstream test_file(string(file_prefix) + file_name, ios::out);
				test_file.close();

				file check(file_name, file::Mode::READ_ONLY);
				Assert::IsTrue(check.file_is_open());

				check.close_file();
				if (remove((string(file_prefix) + file_name).c_str()) != 0)
					Logger::WriteMessage("Delete File Operation Failed!!");
			}

			TEST_METHOD(CreateAndOpenNewFileUsingStringTest)
			{
				string file_name = "CreateAndOpenNewFileUsingStringTest.txt";

				file check(file_name, file::Mode::WRITE_ONLY);

				fstream test_file(string(file_prefix) + file_name, ios::out | ios::in);
				Assert::IsTrue(test_file.is_open());

				check.close_file();
				test_file.close();
				if (remove((string(file_prefix) + file_name).c_str()) != 0)
					Logger::WriteMessage("Delete File Operation Failed!!");
			}

			TEST_METHOD(CreateAndOpenNewFileUsingIntTest)
			{
				string file_name = "test0.txt";
				int file_num = 0;

				file check(file_num, file::Mode::WRITE_ONLY);

				fstream test_file(string(file_prefix) + file_name, ios::out | ios::in);
				Assert::IsTrue(test_file.is_open());

				check.close_file();
				test_file.close();
				if (remove((string(file_prefix) + file_name).c_str()) != 0)
					Logger::WriteMessage("Delete File Operation Failed!!");
			}

			TEST_METHOD(CheckFileSizeTest)
			{
				string file_name = "CheckFileSizeTest.txt";

				fstream dump_file(string(file_prefix) + file_name, ios::out);
				dump_file << _100bytes;
				dump_file.close(); 

				file test_file(file_name, file::Mode::READ_WRITE);
				int length = static_cast<int>(test_file.check_file_size());
				Assert::AreEqual(file::max_size, length, L"Incorrect file size after writing 100 bytes");

				dump_file.open(string(file_prefix) + file_name, ios::out | ios::trunc);
				dump_file << _500bytes;
				dump_file.close();

				length = static_cast<int>(test_file.check_file_size());
				Assert::AreEqual(file::max_size, length, L"Incorrect file size after writing 500 bytes");

				test_file.close_file();
				if (remove((string(file_prefix) + file_name).c_str()) != 0)
					Logger::WriteMessage("Delete File Operation Failed!!");
			}

			TEST_METHOD(WriteFullStringToFileTest)
			{
				string file_name = "WriteStringToFileTest.txt";
				stringstream file_buf;

				file test_file(file_name, file::Mode::WRITE_ONLY);
				test_file.seek(0, _100bytes);

				fstream check(string(file_prefix) + file_name, ios::out | ios::in | ios::binary);
				if (!check.is_open())
					Assert::Fail(L"File not created!");
				
				file_buf << check.rdbuf();
				char test[110] = "";
				file_buf.read(test, 100);
				Assert::AreEqual(_100bytes, string(test));
				Assert::AreEqual(file::max_size, (int)test_file.check_file_size(), L"File size beyond maximum");

				test_file.close_file();
				check.close();
				if (remove((string(file_prefix) + file_name).c_str()) != 0)
					Logger::WriteMessage("Delete File Operation Failed!!");
			}

			TEST_METHOD(FileWriteBeyondMaxSizeTest)
			{
				string file_name = "FileWriteBeyondMaxSizeTest.txt";
				stringstream file_buf;

				file test_file(file_name, file::Mode::WRITE_ONLY);
				int char_written = test_file.seek(450, _100bytes);

				fstream check(string(file_prefix) + file_name, ios::out | ios::in | ios::binary);
				if (!check.is_open())
					Assert::Fail(L"File not created!");
				file_buf << check.rdbuf();
				file_buf.seekg(450, ios::beg);
				char test[100] = "";
				file_buf.read(test, sizeof(test));

				Assert::AreEqual(50, char_written, L"Characters written incorrectly returned");
				Assert::AreEqual(_100bytes.substr(0, 50).c_str(), test, L"Incorrect string written to file");
				Assert::AreEqual(file::max_size, (int)test_file.check_file_size(), L"File size beyond maximum");

				check.close();
				test_file.close_file();
				if (remove((string(file_prefix) + file_name).c_str()) != 0)
					Logger::WriteMessage("Delete File Operation Failed!!");
			}

			TEST_METHOD(FileOutOfBoundsWriteTest)
			{
				string file_name = "WritingToFullFileTest.txt";

				file test_file(file_name, file::Mode::READ_WRITE);
				Logger::WriteMessage(to_string(test_file.check_file_size()).c_str());
				int char_written = test_file.seek(750, _500bytes);
				int length = static_cast<int>(test_file.check_file_size());

				Assert::AreEqual(0, char_written, L"Characters written to full file");
				Assert::AreEqual(file::max_size, length, L"Size exceeded max file size or decreased than max");

				test_file.close_file();
				if (remove((string(file_prefix) + file_name).c_str()) != 0)
					Logger::WriteMessage("Delete File Operation Failed!!");
			}
		};
	}
}