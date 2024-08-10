#include <cstring>
#include "fileManager.hpp"

namespace LargeFile
{
    namespace FileManager
    {
        
        FileManager::FileManager(File::Mode openMode)
        {
            fileMode = openMode;
        }

        FileManager::FileManager(int specifiedSize, File::Mode openMode)
        {
            fileMode = openMode;
            int fileNeeded = (specifiedSize / File::maxSize) + 1;
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

        void FileManager::createFiles(int numOfFiles)
        {
            File* newFile = NULL;
            for(int i = numOfFilesOpen; i < (numOfFilesOpen + numOfFiles); i++)
            {
                newFile = new File("test" + to_string(i) + ".txt", fileMode);
                filesRecord.push_back(newFile);
                // numOfFilesOpen++;
            }
            numOfFilesOpen += numOfFiles;
        }

        void FileManager::write(string& line, int count, int& pos)
        {
            if(pos < 0)
            {
                return;
            }

            int filesToCreate = ((pos + count)/ File::maxSize) + 1;
            createFiles(filesToCreate - numOfFilesOpen);

            int charactersWritten = 0;
            int fileIndex = pos / File::maxSize;
            if(fileIndex >= numOfFilesOpen)
            {
                return;
            }
            
            for (int i = fileIndex; i < numOfFilesOpen; i++)   //checks and writes into existing files
            {
                filesRecord[i]->seekPut(pos % File::maxSize, File::Dir::BEG);
                charactersWritten = filesRecord[i]->File::write(count, line.c_str());
                if (charactersWritten > 0 && charactersWritten < count)
                {
                    line.erase(line.begin(), line.begin() + charactersWritten);
                    pos = 0;
                    charactersWritten = 0;
                    count -= charactersWritten;
                }
                else if (charactersWritten == line.size())
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