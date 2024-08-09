#ifndef _LOCAL_FILE_HPP_
#define _LOCAL_FILE_HPP_

#include <iostream>
#include <fstream>
#include <string>

namespace SmallFile
{
    namespace LocalFile
    {
        using namespace std;
        class File
        {
            public:
            enum class Mode
            {
                APPEND, BINARY, READ_ONLY, WRITE_ONLY, READ_WRITE, TRUNCATE, APPEND_AT_END
            };
            enum class Dir
            {
                BEG, CUR, END
            };

            private:
            string filePrefix = "../test_files/";
            string fileName;
            fstream fileObj;
            streamsize fileSize = 0;
            Mode fileMode;

            public:
            static const int maxSize;
            File();
            File(string, Mode);
            ~File();
            void openFile(string, Mode);
            void fillFileRandom();
            bool fileIsOpen();
            void closeFile();
            streamsize checkFileSize();
            int write(int, const char*);
            int read(int, char*);
            int tellGet();
            void seekGet(int, Dir);
            int tellPut();
            void seekPut(int, Dir);
        };
    }
}

#else 
extern const int maxSize;
#endif //_LOCAL_FILE_HPP_