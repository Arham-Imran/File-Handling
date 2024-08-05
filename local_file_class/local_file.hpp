#ifndef _LOCAL_FILE_HPP_
#define _LOCAL_FILE_HPP_

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class file
{
    private:
    string file_name = "File_Dump/";
    fstream file_obj;
    streamsize file_size = 0;

    public:
    static const int max_size = 500;
    file(int);
    file(string);
    ~file();
    bool file_is_open();
    void close_file();
    streamsize check_file_size();
    // int write_file(string);
    // int write_file(int, string);
    int seek(int, string);
};

#endif //_LOCAL_FILE_HPP_