#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
using namespace std;

class file
{
    private:
    char file_name[50];
    fstream file_obj;

    public:
    streamsize file_size;
    static const int max_size = 500;
    file(int);
    ~file();
    streamsize check_file_size();
    int write_file(string);
    int write_file(int, string);
};

file::file(int num)
{
    sprintf(file_name, "../test%d.txt", num);
    file_obj.open(file_name, ios::in | ios::out);
    if(!file_obj.is_open())
    {
        file_obj.open(file_name, ios::out);
        file_obj.close();
        file_obj.open(file_name, ios::out | ios::in);
    }
    file_size = check_file_size();
}

file::~file()
{
    file_obj.close();
}

streamsize file::check_file_size()
{
    file_obj.ignore(std::numeric_limits<std::streamsize>::max());
    streamsize length = file_obj.gcount();
    file_obj.clear();
    file_obj.seekp(0, ios::beg);
    file_obj.seekg(0, ios::beg);
    return length;
}

int file::write_file(string new_data)
{
    if(file_size >= max_size)
    {
        return 0;
    }
    else if(new_data.size() + file_size <= max_size)
    {
        file_obj.close();
        file_obj.open(file_name, ios::out | ios::in | ios::app);
        file_obj << new_data;
        file_obj.flush();
        file_size += new_data.size();
        file_obj.close();
        file_obj.open(file_name, ios::out | ios::in);
        
        return new_data.size();
    }
    else if(file_size < max_size && file_size + new_data.size() > max_size)
    {
        int buffer_size = max_size - file_size;
        new_data.resize(buffer_size);
        file_obj.close();
        file_obj.open(file_name, ios::in | ios::out | ios::app);
        file_obj << new_data;
        file_obj.flush();
        file_obj.close();
        file_obj.open(file_name, ios::out | ios::in);
        file_size += buffer_size;

        return buffer_size;
    }
    else
    {
        return -1;
    }
}

int file::write_file(int pos, string new_data)
{
    if(file_size >= max_size)
    {
        return 0;
    }
    else if(pos == 0 && file_size == 0)
    {
        return write_file(new_data);
    }
    else if(new_data.size() + pos <= max_size)
    {
        pos = pos>file_size ? file_size : pos;

        if(pos == 0)
        {
            return write_file(new_data);
        }

        stringstream file_buf;
        file_obj.seekg(0, ios::beg);
        file_buf << file_obj.rdbuf();
        file_buf.seekp(pos, ios::beg);
        file_buf << new_data;
        file_obj.seekg(pos, ios::beg);
        file_buf << file_obj.rdbuf();
        file_obj.seekp(0, ios::beg);
        file_obj << file_buf.str();
        file_obj.flush();
        file_obj.seekp(0, ios::beg);
        file_obj.seekg(0, ios::beg);
        file_size += new_data.size();
        
        return new_data.size();
    }
    else if(pos < max_size && pos + new_data.size() > max_size)
    {
        int buffer_size = max_size - pos;
        pos = pos>file_size ? file_size : pos;
        new_data.resize(buffer_size);

        if(pos == 0)
        {
            return write_file(new_data);
        }

        stringstream file_buf;
        file_obj.seekg(0, ios::beg);
        file_buf << file_obj.rdbuf();
        file_buf.seekp(pos, ios::beg);
        file_buf << new_data;
        file_obj.seekg(pos, ios::beg);
        file_buf << file_obj.rdbuf();
        file_obj.seekp(0, ios::beg);
        file_obj << file_buf.str();
        file_obj.flush();
        file_obj.seekp(0, ios::beg);
        file_obj.seekg(0, ios::beg);
        file_size += buffer_size;

        return buffer_size;
    }
    else
    {
        return -1;
    }
}

void create_files(vector<file*>& files, int num_of_files)
{
    static int num = 1;
    file *new_file = NULL;
    for(int i=0; i<num_of_files; i++)
    {
        new_file = new file(num);
        files.push_back(new_file);
        num++;
    }
}

void parse_input(int& pos, string& line)
{
    cin >> pos;
    getline(cin, line);
    line.erase(line.begin());
}

void file_control(vector<file*>& files, string& line, int& pos)
{

    int files_to_create = (pos / file::max_size) + 1;
    if(files_to_create > files.size() && pos%500 + line.size() > 500)
    {
        create_files(files, files_to_create - files.size() + 1);        
    }
    else if (files_to_create > files.size())
    {
        create_files(files, files_to_create - files.size());
    }
    int characters_written = 0; 
    for(int i=0; i<files.size(); i++)   // checks and writes into existing files
    {
        characters_written = files[i]->write_file(pos, line);
        if(characters_written <= 0)
        {
            pos -= file::max_size;
            pos = pos<0 ? 0 : pos;
            continue;
        }
        else if(characters_written > 0 && characters_written < line.size())
        {
            line.erase(line.begin(), line.begin() + characters_written);
            pos = 0;
            characters_written = 0;
        }
        else if(characters_written == line.size())
        {
            return;
        }
    }
}

// void check_prev_files(vector<file*>& files, string& line, )
// {
//     int chars_written = 0;
//     for(int i=files.size()-1; i>=0; i--)
//     {
//         if(files[i]->file_size < file::max_size)
//         {
//             chars_written = files[i]->write_file(line);
//             if(chars_written == line.size())
//             {
//                 return;
//             }
//             else if(chars_written < line.size() && chars_written > 0)
//             {
//                 line.erase(line.begin(), line.begin() + chars_written);
//                 chars_written = 0;
//                 continue;
//             }
//             else
//             {
//                 continue;
//             }
//         }
//         else
//         {
//             return;
//         }
//     }
// }

int main()
{
    int pos = -1;
    int chars_written = -2;
    string line;
    vector<file*> files;

    create_files(files, 4);
    
    while(true)
    {
        parse_input(pos, line);
        file_control(files, line, pos);
    }
}