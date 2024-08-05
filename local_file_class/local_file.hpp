#ifndef _LOCAL_FILE_HPP_
#define _LOCAL_FILE_HPP_

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class file
{
    private:
    char file_name[50];
    fstream file_obj;
    streamsize file_size;

    public:
    static const int max_size = 500;
    file(int);
    ~file();
    streamsize check_file_size();
    int write_file(string);
    int write_file(int, string);
};

#endif //_LOCAL_FILE_HPP_