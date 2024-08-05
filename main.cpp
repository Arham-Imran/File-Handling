#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
#include "file_manager.hpp"
using namespace std;

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
    file_manager new_filesystem;
    
    while(true)
    {
        parse_input(pos, line);
        new_filesystem.file_control(line, pos);
    }
}