#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
using namespace std;

class file_system
{
    private:
    char file_name[50];
    fstream file;
    streamsize file_size;

    public:
    static const int max_size = 5 * 1024 * 1024;
    file_system(int);
    streamsize sizeof_file();
    void write_file(int, string);

};

file_system::file_system(int num)
{
    sprintf(file_name, "test%d.txt", num);
    file.open(file_name, ios::in | ios::out);
    if(!file.is_open())
        file.open(file_name, ios:: out);
    else
        file_size = sizeof_file();
}

streamsize file_system::sizeof_file()
{
    file.ignore(std::numeric_limits<std::streamsize>::max());
    std::streamsize length = file.gcount();
    file.clear();
    file.close();
    return length;
}

void file_system::write_file(int pos, string new_data)
{
    if(new_data.size() + file_size <= max_size)
    {
        stringstream file_buf;
        file.seekg(0, ios::beg);
        file_buf << file.rdbuf();
        file_buf.seekp(pos, ios::beg);
        file_buf << new_data;
        file.seekg(pos, ios::beg);
        file_buf << file.rdbuf();
        file.seekp(0, ios::beg);
        file << file_buf.str();
    }
    else if(file_size)
    {

    }
    file_size = sizeof_file();
}

int main()
{
    fstream file("../test1.txt", ios::out);
    file << "Heelllelele" << endl;
    cout << file.bad() << endl;
    file.close();
}