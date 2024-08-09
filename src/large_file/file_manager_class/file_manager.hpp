#ifndef _FILE_MANAGER_HPP_
#define _FILE_MANAGER_HPP_

#include <iostream>
#include <vector>
#include "local_file.hpp"

namespace large_file
{
    namespace file_manager
    {
        using namespace std;
        using namespace small_file::local_file;
        class file_manager
        {
            private:
            vector<file*> files_record;
            int num_of_files_open = 0;
            file::Mode file_mode;

            public:
            void create_files(int);
            file_manager(file::Mode);
            file_manager(int, file::Mode);
            ~file_manager();
            void write(string&, int, int&);
            void read(char*, int, int&);
            void close_all_files();
        };
    }
}

#endif //_FILE_MANAGER_HPP_