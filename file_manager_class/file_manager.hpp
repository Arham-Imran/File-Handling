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
    int num_of_files_open;
    void create_files(int);

    public:
    file_manager();
    ~file_manager();
    void file_control(string&, int&);
};

#endif //_FILE_MANAGER_HPP_