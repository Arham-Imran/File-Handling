#include <cstring>
#include "file_manager.hpp"

namespace LargeFile
{
    namespace FileManager
    {
        
        FileManager::FileManager(File::Mode openMode)
        {
            fileMode = openMode;
        }

        FileManager::FileManager(int specified_size, File::Mode openMode)
        {
            fileMode = openMode;
            int fileNeeded = (specified_size / File::maxSize) + 1;
            createFiles(fileNeeded);
            numOfFilesOpen = fileNeeded;
        }

        FileManager::~FileManager()
        {
            closeAllFiles();
        }

        void FileManager::openFiles(int numOfFiles)
        {
            createFiles(numOfFiles);
        }

        void FileManager::createFiles(int num_of_files)
        {
            File* new_file = NULL;
            for(int i = numOfFilesOpen; i < (numOfFilesOpen + num_of_files); i++)
            {
                new_file = new File("test" + to_string(i) + ".txt", fileMode);
                filesRecord.push_back(new_file);
                // numOfFilesOpen++;
            }
            numOfFilesOpen += num_of_files;
        }

        void FileManager::write(string& line, int count, int& pos)
        {
            if(pos < 0)
            {
                return;
            }

            int files_to_create = ((pos + count)/ File::maxSize) + 1;
            createFiles(files_to_create - numOfFilesOpen);

            int characters_written = 0;
            int fileIndex = pos / File::maxSize;
            if(fileIndex >= numOfFilesOpen)
            {
                return;
            }
            
            for (int i = fileIndex; i < numOfFilesOpen; i++)   //checks and writes into existing files
            {
                filesRecord[i]->seekPut(pos % File::maxSize, File::Dir::BEG);
                characters_written = filesRecord[i]->File::write(count, line.c_str());
                if (characters_written > 0 && characters_written < count)
                {
                    line.erase(line.begin(), line.begin() + characters_written);
                    pos = 0;
                    characters_written = 0;
                    count -= characters_written;
                }
                else if (characters_written == line.size())
                {
                    return;
                }
            }
        }

        void FileManager::read(char* readBuffer, int count, int& pos)
        {
            if(count <= 0 || pos < 0)
            {
                return;
            }
            
            int charactersRead = 0;
            int fileIndex = pos / File::maxSize;
            char storeBuffer[File::maxSize + 1] = "";
            if(fileIndex >= numOfFilesOpen)
            {
                return;
            }

            for(int i = fileIndex; i < numOfFilesOpen; i++)
            {
                filesRecord[i]->seekGet(pos % File::maxSize, File::Dir::BEG);
                charactersRead = filesRecord[i]->read(count, storeBuffer);
                if(charactersRead >= 0 && charactersRead < count)
                {
                    strncat(readBuffer, storeBuffer, charactersRead);
                    pos = 0;
                    count -= charactersRead;
                }
                else if(charactersRead == count)
                {
                    strncat(readBuffer, storeBuffer, charactersRead);
                    return;
                }
            }
        }

        void FileManager::closeAllFiles()
        {
            for (int i = 0; i < numOfFilesOpen; i++)
                delete filesRecord[i];
            numOfFilesOpen = 0;
        }
    }
}