#ifndef _LOCAL_FILE_HPP_
#define _LOCAL_FILE_HPP_

#include <iostream>
#include <fstream>
#include <string>

namespace small_file
{
    namespace local_file
    {
        using namespace std;
        class file
        {
            public:
            enum class Mode
            {
                APPEND, BINARY, READ_ONLY, WRITE_ONLY, READ_WRITE, TRUNCATE, APPEND_AT_END
            };

            private:
            string file_prefix = "../test_files/";
            string file_name;
            fstream file_obj;
            streamsize file_size = 0;
            Mode file_mode;

            public:
            static const int max_size;
            file();
            //file(int, Mode);
            file(string, Mode);
            ~file();
            void open_file(string, Mode);
            void fill_file_random();
            bool file_is_open();
            void close_file();
            streamsize check_file_size();
            int write(int, const char*);
        };
    }
}

#else 
extern const int max_size;
#endif //_LOCAL_FILE_HPP_