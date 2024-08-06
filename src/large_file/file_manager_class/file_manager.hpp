#ifndef _FILE_MANAGER_HPP_
#define _FILE_MANAGER_HPP_

#include <iostream>
#include <vector>
#include "local_file.hpp"
using namespace std;

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
    void seek(string&, int&);
    void close_all_files();
};

#endif //_FILE_MANAGER_HPP_