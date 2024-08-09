#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
#include "file_manager.hpp"
using namespace std;
using namespace SmallFile::LocalFile;
using namespace LargeFile::FileManager;

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
    FileManager new_filesystem(File::Mode::WRITE_ONLY);
    
    while(true)
    {
        parse_input(pos, line);
        new_filesystem.write(line, line.size(), pos);
    }
}