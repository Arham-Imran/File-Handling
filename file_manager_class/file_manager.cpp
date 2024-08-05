#include "file_manager.hpp"

file_manager::file_manager() : num_of_files_open(0) {}

file_manager::~file_manager()
{
    close_all_files();
}

void file_manager::create_files(int num_of_files)
{
    file* new_file = NULL;
    for(int i = 0; i < num_of_files; i++)
    {
        new_file = new file(num_of_files_open);
        files_record.push_back(new_file);
        num_of_files_open++;
    }
}

// void file_manager::file_control(string& line, int& pos)
// {
//     int files_to_create = (pos / file::max_size) + 1;
//     if(files_to_create > num_of_files_open && (pos % 500) + line.size() > 500)       // creates an extra file if string is split between 2 new files
//     {
//         create_files(files_to_create - num_of_files_open + 1);        
//     }
//     else if (files_to_create > num_of_files_open)
//     {
//         create_files(files_to_create - num_of_files_open);
//     }

//     int characters_written = 0; 
//     for(int i = 0; i < num_of_files_open; i++)   // checks and writes into existing files
//     {
//         characters_written = files_record[i]->write(pos, line);
//         if(characters_written <= 0)
//         {
//             pos -= file::max_size;
//             pos = (pos < 0) ? 0 : pos;
//             continue;
//         }
//         else if(characters_written > 0 && characters_written < line.size())
//         {
//             line.erase(line.begin(), line.begin() + characters_written);
//             pos = 0;
//             characters_written = 0;
//         }
//         else if(characters_written == line.size())
//         {
//             return;
//         }
//     }
// }

void file_manager::seek(string& line, int& pos)
{
    int files_to_create = ((pos + line.size())/ file::max_size) + 1;
    create_files(files_to_create - num_of_files_open);
    int characters_written = 0;
    
    for (int i = 0; i < num_of_files_open; i++)   //checks and writes into existing files
    {
        characters_written = files_record[i]->file::seek(pos, line);
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