#include <limits>
#include <cstring>
#include "local_file.hpp"

namespace small_file
{
    namespace local_file
    {
        using namespace std;    

        const int file::max_size = 500; 
        
        file::file(){}

        // file::file(int num, Mode open_mode)
        // {
        //     file_name.append("test" + to_string(num) + ".txt");
            
        //     file_mode = open_mode;
        //     file_size = check_file_size();
        //     open_file();

        //     fill_file_random();
        //     file_obj.seekp(0, ios::beg);
        // }

        file::file(string name, Mode open_mode)
        {
            open_file(name, open_mode);
            file_size = check_file_size();

            fill_file_random();
            file_obj.seekp(0, ios::beg);
        }

        file::~file()
        {
            close_file();
        }

        void file::open_file(string name, Mode open_mode)
        {
            if (!file_obj.is_open())
            {
                file_name = name;
                file_mode = open_mode;
                switch (file_mode)
                {
                case Mode::APPEND:
                    file_obj.open(file_prefix + file_name, ios::app);
                    break;

                case Mode::BINARY:
                    file_obj.open(file_prefix + file_name, ios::binary | ios::in | ios::out);
                    if (!file_obj.is_open())
                        file_obj.open(file_prefix + file_name, ios::binary | ios::out);
                    break;

                case Mode::READ_ONLY:
                    file_obj.open(file_prefix + file_name, ios::in);
                    break;

                case Mode::WRITE_ONLY:
                    file_obj.open(file_prefix + file_name, ios::out);
                    break;

                case Mode::READ_WRITE:
                    file_obj.open(file_prefix + file_name, ios::in | ios::out);
                    // if (!file_obj.is_open())
                    // {
                    //     file_obj.open(file_name, ios::out);
                    //     file_obj.close();
                    //     file_obj.open(file_name, ios::in | ios::out);
                    // }
                    break;

                case Mode::TRUNCATE:
                    file_obj.open(file_prefix + file_name, ios::out | ios::trunc);
                    break;

                case Mode::APPEND_AT_END:
                    file_obj.open(file_prefix + file_name, ios::ate | ios::in | ios::out);
                    // if (!file_obj.is_open())
                    //     file_obj.open(file_name, ios::ate | ios::out);
                    break;

                default:
                    break;
                }
            }
        }

        void file::fill_file_random()
        {
            if(file_mode == Mode::READ_WRITE || file_mode == Mode::WRITE_ONLY)
            {
                char random = 'A';
                while (file_size < file::max_size)
                {
                    file_obj.seekp(0, ios::end);
                    file_obj << random;
                    file_obj.flush();

                    random++;
                    file_size++;

                    if (random == 'z')
                        random = 'A';

                    if (file_size % 100 == 0 && file_size < file::max_size)
                    {
                        file_obj << endl;
                        file_size++;
                    }
                }
            }
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
            if(file_obj.is_open())
                file_obj.close();   
            
            file_obj.open(file_prefix + file_name, ios::out | ios::in | ios::binary);
            if (!file_obj.is_open())
            {
                open_file(file_name, file_mode);
                return 0;
            }

            file_obj.ignore(std::numeric_limits<std::streamsize>::max());
            streamsize length = file_obj.gcount();
            file_obj.clear();
            file_obj.seekg(0, ios::beg);

            file_obj.close();
            open_file(file_name, file_mode);
            return length;
        }

        int file::write(int pos, const char* new_data)
        {
            char write_buffer[501] = "";
            if (pos >= max_size)
            {
                return 0;
            }
            else if (pos + strlen(new_data) > max_size)
            {
                strncpy(write_buffer, new_data, max_size - pos);
                // new_data.resize(max_size - pos);
                
                file_obj.seekp(pos, ios::beg);
                file_obj << write_buffer;
                file_obj.flush();

                file_size += strlen(write_buffer); 
                return strlen(write_buffer);
            }
            else if (pos + strlen(new_data) <= max_size)
            {
                file_obj.seekp(pos, ios::beg);
                file_obj << new_data;
                file_obj.flush();

                file_size += strlen(write_buffer);
                return strlen(new_data);
            }
            else
            {
                return -1;
            }
        }

        int file::read(int count, char* readBuffer)
        {
            if(file_obj.tellp() >= file::max_size)
            {
                return 0;
            }
            else if((count + file_obj.tellp()) > file::max_size)
            {
                file_obj.read(readBuffer, file::max_size - file_obj.tellg());
                return (file::max_size - file_obj.tellg());
            }
            else if((count + file_obj.tellp()) <= file::max_size)
            {
                file_obj.read(readBuffer, count);
                return count;
            }
            else
            {
                return -1;
            }
        }

        int file::tellGet()
        {
            return file_obj.tellp();
        }

        void file::seekGet(int pos, Dir dir)
        {
            switch(dir)
            {
                case Dir::BEG:
                if(pos < 0)
                {
                    return;
                }
                file_obj.seekg(pos, ios::beg);
                break;

                case Dir::CUR:
                file_obj.seekg(pos, ios::cur);
                break;

                case Dir::END:
                if(pos > 0)
                {
                    return;
                }
                file_obj.seekg(pos, ios::end);
                break;

                default:
                return;
                break;
            }
        }

        int file::tellPut()
        {
            return file_obj.tellp();
        }

        void file::seekPut(int pos, Dir dir)
        {
            switch(dir)
            {
                case Dir::BEG:
                if(pos < 0)
                {
                    return;
                }
                file_obj.seekp(pos, ios::beg);
                break;

                case Dir::CUR:
                file_obj.seekp(pos, ios::cur);
                break;

                case Dir::END:
                if(pos > 0)
                {
                    return;
                }
                file_obj.seekp(pos, ios::end);
                break;

                default:
                return;
                break;
            }
        }
    }
}