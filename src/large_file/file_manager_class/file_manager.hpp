#ifndef _FILE_MANAGER_HPP_
#define _FILE_MANAGER_HPP_

#include <iostream>
#include <vector>
#include "local_file.hpp"

namespace LargeFile
{
    namespace FileManager
    {
        using namespace std;
        using namespace SmallFile::LocalFile;
        class FileManager
        {
            private:
            vector<File*> filesRecord;
            int numOfFilesOpen = 0;
            File::Mode fileMode;

            public:
            void createFiles(int);
            void openFiles(int);
            FileManager(File::Mode);
            FileManager(int, File::Mode);
            ~FileManager();
            void write(string&, int, int&);
            void read(char*, int, int&);
            void closeAllFiles();
        };
    }
}

#endif //_FILE_MANAGER_HPP_