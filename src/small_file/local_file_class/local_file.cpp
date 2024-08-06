#include <limits>
#include "local_file.hpp"

namespace small_file
{
    namespace local_file
    {
        file::file(){}

        file::file(int num, Mode open_mode)
        {
            file_name.append("test" + to_string(num) + ".txt");
            
            file_mode = open_mode;
            file_size = check_file_size();
            open_file();

            fill_file_random();
            file_obj.seekp(0, ios::beg);
        }

        file::file(string name, Mode open_mode)
        {
            file_name.append(name);
            file_mode = open_mode;

            file_size = check_file_size();
            open_file();

            fill_file_random();
            file_obj.seekp(0, ios::beg);
        }

        file::~file()
        {
            close_file();
        }

        void file::open_file()
        {
            if (!file_obj.is_open())
            {
                switch (file_mode)
                {
                case Mode::APPEND:
                    file_obj.open(file_name, ios::app);
                    break;

                case Mode::BINARY:
                    file_obj.open(file_name, ios::binary | ios::binary);
                    if (!file_obj.is_open())
                        file_obj.open(file_name, ios::binary | ios::out);
                    break;

                case Mode::READ_ONLY:
                    file_obj.open(file_name, ios::in);
                    break;

                case Mode::WRITE_ONLY:
                    file_obj.open(file_name, ios::out);
                    break;

                case Mode::READ_WRITE:
                    file_obj.open(file_name, ios::in | ios::out);
                    if (!file_obj.is_open())
                    {
                        file_obj.open(file_name, ios::out);
                        file_obj.close();
                        file_obj.open(file_name, ios::in | ios::out);
                    }
                    break;

                case Mode::TRUNCATE:
                    file_obj.open(file_name, ios::out | ios::trunc);
                    break;

                case Mode::APPEND_AT_END:
                    file_obj.open(file_name, ios::ate | ios::in | ios::out);
                    if (!file_obj.is_open())
                        file_obj.open(file_name, ios::ate | ios::out);
                    break;

                default:
                    break;
                }
            }
        }

        void file::fill_file_random()
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
                    file_size += 2;
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
            
            file_obj.open(file_name, ios::out | ios::in | ios::binary);
            if (!file_obj.is_open())
                return 0;

            file_obj.ignore(std::numeric_limits<std::streamsize>::max());
            streamsize length = file_obj.gcount();
            file_obj.clear();
            file_obj.seekg(0, ios::beg);

            file_obj.close();
            open_file();
            return length;
        }

        int file::seek(int pos, string new_data)
        {
            if (pos >= max_size)
            {
                return 0;
            }
            else if (pos + new_data.size() > max_size)
            {
                new_data.resize(max_size - pos);
                
                file_obj.seekp(pos, ios::beg);
                file_obj << new_data;
                file_obj.flush();

                return new_data.size();
            }
            else if (pos + new_data.size() <= max_size)
            {
                file_obj.seekp(pos, ios::beg);
                file_obj << new_data;
                file_obj.flush();

                return new_data.size();
            }
            else
            {
                return -1;
            }
        }
    }
}