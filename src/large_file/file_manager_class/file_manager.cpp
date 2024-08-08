#include "file_manager.hpp"

namespace large_file
{
    namespace file_manager
    {
        
        file_manager::file_manager(file::Mode open_mode)
        {
            file_mode = open_mode;
            // create_files(4);
        }

        file_manager::file_manager(int specified_size, file::Mode open_mode)
        {
            file_mode = open_mode;
            int files_needed = (specified_size / file::max_size) + 1;
            create_files(files_needed);
            num_of_files_open = files_needed;
        }

        file_manager::~file_manager()
        {
            close_all_files();
        }

        void file_manager::create_files(int num_of_files)
        {
            file* new_file = NULL;
            for(int i = num_of_files_open; i < (num_of_files_open + num_of_files); i++)
            {
                new_file = new file("test" + to_string(i) + ".txt", file_mode);
                files_record.push_back(new_file);
                // num_of_files_open++;
            }
            num_of_files_open += num_of_files;
        }

        void file_manager::seek(string& line, int& pos)
        {
            int files_to_create = ((pos + line.size())/ file::max_size) + 1;
            create_files(files_to_create - num_of_files_open);
            int characters_written = 0;
            
            for (int i = 0; i < num_of_files_open; i++)   //checks and writes into existing files
            {
                characters_written = files_record[i]->file::write(pos, line.c_str());
                if (characters_written <= 0)
                {
                    pos -= file::max_size;
                    continue;
                }
                else if (characters_written > 0 && characters_written < line.size())
                {
                    line.erase(line.begin(), line.begin() + characters_written);
                    pos = 0;
                    characters_written = 0;
                }
                else if (characters_written == line.size())
                {
                    return;
                }
            }
        }

        void file_manager::close_all_files()
        {
            for (int i = 0; i < num_of_files_open; i++)
                delete files_record[i];
            num_of_files_open = 0;
        }
    }
}