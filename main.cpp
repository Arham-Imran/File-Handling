#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
#include "file_manager.hpp"
using namespace std;
using namespace small_file::local_file;
using namespace large_file::file_manager;

void parse_input(int& pos, string& line)
{
    cin >> pos;
    getline(cin, line);
    line.erase(line.begin());
}

int main()
{
    int pos = -1;
    string line = "";
    file_manager new_filesystem(file::Mode::WRITE_ONLY);
    
    while(true)
    {
        parse_input(pos, line);
        new_filesystem.seek(line, pos);
    }
}