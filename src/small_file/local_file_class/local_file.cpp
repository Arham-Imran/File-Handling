#include <limits>
#include <cstring>
#include "local_file.hpp"

namespace SmallFile
{
    namespace LocalFile
    {
        using namespace std;    

        const int File::maxSize = 500; 
        
        File::File(){}

        File::File(string name, Mode open_mode)
        {
            openFile(name, open_mode);
            fileSize = checkFileSize();

            fillFileRandom();
            fileObj.seekp(0, ios::beg);
        }

        File::~File()
        {
            closeFile();
        }

        void File::openFile(string name, Mode open_mode)
        {
            if (!fileObj.is_open())
            {
                fileName = name;
                fileMode = open_mode;
                switch (fileMode)
                {
                case Mode::APPEND:
                    fileObj.open(filePrefix + fileName, ios::app);
                    break;

                case Mode::BINARY:
                    fileObj.open(filePrefix + fileName, ios::binary | ios::in | ios::out);
                    if (!fileObj.is_open())
                        fileObj.open(filePrefix + fileName, ios::binary | ios::out);
                    break;

                case Mode::READ_ONLY:
                    fileObj.open(filePrefix + fileName, ios::in);
                    break;

                case Mode::WRITE_ONLY:
                    fileObj.open(filePrefix + fileName, ios::out);
                    break;

                case Mode::READ_WRITE:
                    fileObj.open(filePrefix + fileName, ios::in | ios::out);
                    break;

                case Mode::TRUNCATE:
                    fileObj.open(filePrefix + fileName, ios::out | ios::trunc);
                    break;

                case Mode::APPEND_AT_END:
                    fileObj.open(filePrefix + fileName, ios::ate | ios::in | ios::out);
                    break;

                default:
                    break;
                }
            }
        }

        void File::fillFileRandom()
        {
            if(fileMode == Mode::READ_WRITE || fileMode == Mode::WRITE_ONLY)
            {
                char random = 'A';
                while (fileSize < File::maxSize)
                {
                    fileObj.seekp(0, ios::end);
                    fileObj << random;
                    fileObj.flush();

                    random++;
                    fileSize++;

                    if (random == 'z')
                        random = 'A';

                    if (fileSize % 100 == 0 && fileSize < File::maxSize)
                    {
                        fileObj << endl;
                        fileSize++;
                    }
                }
            }
        }

        bool File::fileIsOpen()
        {
            return fileObj.is_open();
        }

        void File::closeFile()
        {
            if (fileObj.is_open())
                fileObj.close();
            else
                return;
        }

        streamsize File::checkFileSize()
        {
            if(fileObj.is_open())
                fileObj.close();   
            
            fileObj.open(filePrefix + fileName, ios::out | ios::in | ios::binary);
            if (!fileObj.is_open())
            {
                openFile(fileName, fileMode);
                return 0;
            }

            fileObj.ignore(std::numeric_limits<std::streamsize>::max());
            streamsize length = fileObj.gcount();
            fileObj.clear();
            fileObj.seekg(0, ios::beg);

            fileObj.close();
            openFile(fileName, fileMode);
            return length;
        }

        int File::write(int count, const char* new_data)
        {
            char write_buffer[501] = "";
            if (tellPut() >= maxSize)
            {
                return 0;
            }
            else if (tellPut() + count > maxSize)
            {
                strncpy(write_buffer, new_data, maxSize - tellPut());
                
                fileObj << write_buffer;
                fileObj.flush();

                fileSize += strlen(write_buffer); 
                return strlen(write_buffer);
            }
            else if (tellPut() + count <= maxSize)
            {
                strncpy(write_buffer, new_data, count);

                fileObj.write(write_buffer, count);
                fileObj.flush();

                fileSize += strlen(write_buffer);
                return strlen(write_buffer);
            }
            else
            {
                return -1;
            }
        }

        int File::read(int count, char* readBuffer)
        {
            int currentPos = tellPut();
            if(currentPos >= File::maxSize)
            {
                return 0;
            }
            else if((count + currentPos) > File::maxSize)
            {
                fileObj.read(readBuffer, File::maxSize - currentPos);
                return (File::maxSize - currentPos);
            }
            else if((count + currentPos) <= File::maxSize)
            {
                fileObj.read(readBuffer, count);
                return count;
            }
            else
            {
                return -1;
            }
        }

        int File::tellGet()
        {
            return fileObj.tellp();
        }

        void File::seekGet(int pos, Dir dir)
        {
            switch(dir)
            {
                case Dir::BEG:
                if(pos < 0)
                {
                    return;
                }
                fileObj.seekg(pos, ios::beg);
                break;

                case Dir::CUR:
                fileObj.seekg(pos, ios::cur);
                break;

                case Dir::END:
                if(pos > 0)
                {
                    return;
                }
                fileObj.seekg(pos, ios::end);
                break;

                default:
                return;
                break;
            }
        }

        int File::tellPut()
        {
            return fileObj.tellp();
        }

        void File::seekPut(int pos, Dir dir)
        {
            switch(dir)
            {
                case Dir::BEG:
                if(pos < 0)
                {
                    return;
                }
                fileObj.seekp(pos, ios::beg);
                break;

                case Dir::CUR:
                fileObj.seekp(pos, ios::cur);
                break;

                case Dir::END:
                if(pos > 0)
                {
                    return;
                }
                fileObj.seekp(pos, ios::end);
                break;

                default:
                return;
                break;
            }
        }
    }
}