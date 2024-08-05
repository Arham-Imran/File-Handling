#ifndef _LOCAL_FILE_HPP_
#define _LOCAL_FILE_HPP_

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class file
{
    public:
    enum class Mode
    {
        APPEND, BINARY, READ_ONLY, WRITE_ONLY, READ_WRITE, TRUNCATE, APPEND_AT_END
    };

    private:
    string file_name = "../File_Dump/";
    fstream file_obj;
    streamsize file_size = 0;
    Mode file_mode;

    public:
    static const int max_size = 500;
    file(int, Mode);
    file(string, Mode);
    ~file();
    void open_file();
    void fill_file_random();
    bool file_is_open();
    void close_file();
    streamsize check_file_size();
    int seek(int, string);
};

#endif //_LOCAL_FILE_HPP_