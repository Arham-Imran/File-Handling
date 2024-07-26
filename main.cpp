#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
using namespace std;

class file_system
{
    private:
    char file_name[50];
    fstream file;

    public:
    streamsize file_size;
    static const int max_size = 500;
    file_system(int);
    ~file_system();
    streamsize check_file_size();
    int write_file(string);
    int write_file(int, string);
};

file_system::file_system(int num)
{
    sprintf(file_name, "../test%d.txt", num);
    file.open(file_name, ios::in | ios::out);
    if(!file.is_open())
    {
        file.open(file_name, ios::out);
        file.close();
        file.open(file_name, ios::out | ios::in);
    }
    file_size = check_file_size();
}

file_system::~file_system()
{
    file.close();
}

streamsize file_system::check_file_size()
{
    file.ignore(std::numeric_limits<std::streamsize>::max());
    streamsize length = file.gcount();
    file.clear();
    file.seekp(0, ios::beg);
    file.seekg(0, ios::beg);
    return length;
}

int file_system::write_file(string new_data)
{
    if(new_data.size() + file_size <= max_size)
    {
        file.close();
        file.open(file_name, ios::out | ios::in | ios::app);
        file << new_data;
        file.flush();
        file_size += new_data.size();
        file.close();
        file.open(file_name, ios::out | ios::in);
        
        return new_data.size();
    }
    else if(file_size >= max_size)
    {
        return 0;
    }
    else if(file_size < max_size && file_size + new_data.size() > max_size)
    {
        int buffer_size = max_size - file_size;
        new_data.resize(buffer_size);
        file.close();
        file.open(file_name, ios::in | ios::out | ios::app);
        file << new_data;
        file.flush();
        file.close();
        file.open(file_name, ios::out | ios::in);
        file_size += buffer_size;

        return buffer_size;
    }
    else
        return -1;
}

int file_system::write_file(int pos, string new_data)
{
    if(new_data.size() + file_size <= max_size && pos < file_size)
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
        file.flush();
        file.seekp(0, ios::beg);
        file.seekg(0, ios::beg);
        file_size += new_data.size();
        
        return new_data.size();
    }
    else if(file_size >= max_size && pos < file_size)
    {
        return 0;
    }
    else if(file_size < max_size && file_size + new_data.size() > max_size && pos < file_size)
    {
        int buffer_size = max_size - file_size;
        new_data.resize(buffer_size);
        stringstream file_buf;
        file.seekg(0, ios::beg);
        file_buf << file.rdbuf();
        file_buf.seekp(pos, ios::beg);
        file_buf << new_data;
        file.seekg(pos, ios::beg);
        file_buf << file.rdbuf();
        file.seekp(0, ios::beg);
        file << file_buf.str();
        file.flush();
        file.seekp(0, ios::beg);
        file.seekg(0, ios::beg);
        file_size += buffer_size;

        return buffer_size;
    }
    else
        return -1;
}

void create_files(vector<file_system*>& files, int num_of_files)
{
    static int num = 1;
    file_system *new_file = NULL;
    for(int i=0; i<num_of_files; i++)
    {
        new_file = new file_system(num);
        files.push_back(new_file);
        num++;
    }
}

void check_prev_files(vector<file_system*>& files, string& line, )
{
    int chars_written = 0;
    for(int i=files.size()-1; i>=0; i--)
    {
        if(files[i]->file_size < file_system::max_size)
        {
            chars_written = files[i]->write_file(line);
            if(chars_written == line.size())
            {
                return;
            }
            else if(chars_written < line.size() && chars_written > 0)
            {
                line.erase(line.begin(), line.begin() + chars_written);
                chars_written = 0;
                continue;
            }
            else
            {
                continue;
            }
        }
        else
        {
            return;
        }
    }
}

int main()
{
    int pos = -1;
    bool broke_loop = false;
    int chars_written = -2;
    string line;
    vector<file_system*> files;

    create_files(files, 4);
    
    while(true)
    {
        cin >> pos;
        getline(cin, line);
        line.erase(line.begin());

        for(int i=0; i<files.size(); i++)
        {
            if(files[i]->file_size > pos)
            {
                chars_written = files[i]->write_file(pos, line);
                if(chars_written < line.size() && chars_written > 0)
                {
                    line.erase(line.begin(), line.begin() + chars_written);
                    pos = 0;
                    chars_written = 0;
                }
                else if(chars_written == line.size())
                {
                    broke_loop = true;
                    chars_written = 0;
                    break;
                }
                else if(chars_written == 0)
                {
                    pos = 0;
                }
            }
            else if(files[i]->file_size == 0)
            {
                files[i]->write_file(line);
                broke_loop = true;
                break;
            }
            else
            {
                pos -= files[i]->file_size;
            }
        }
        
        if(!broke_loop && pos >= 0)
        {
            create_files(files, 1);
            files.back()->write_file(line);
            broke_loop = false;
        }
    }
}

/*
test cases:

1. All files are empty and write pos > 0
2. All files are full 
3. Only final file is full and write pos > than all file sizes combined
4. One file has 0 < space < line.size() and rest files are full, pos > size of all files
5. Each file has 0 < space < line.size() and pos very small

*/