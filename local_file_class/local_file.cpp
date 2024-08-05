#include <sstream>
#include <limits>
#include "local_file.hpp"

file::file(int num)
{
    file_name.append("test" + to_string(num) + ".txt");
    file_obj.open(file_name, ios::in | ios::out);
    if(!file_obj.is_open())
    {
        file_obj.open(file_name, ios::out);
        file_obj.close();
        file_obj.open(file_name, ios::out | ios::in);
    }
    file_size = check_file_size();
}

file::file(string name)
{
    file_name.append(name);
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
    close_file();
}

bool file::file_is_open()
{
    return file_obj.is_open();
}

void file::close_file()
{
    if (file_obj.is_open())
        file_obj.close();
    else
        return;
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