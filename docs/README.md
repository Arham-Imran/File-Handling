# File Handling

## Class Diagram

```mermaid
classDiagram
    class FileManager {
        - vector~File*~ filesRecord
        - int numOfFilesOpen
        - File::Mode fileMode
        + FileManager(File::Mode openMode)
        + FileManager(int specifiedSize, File::Mode openMode)
        + ~FileManager()
        + createFiles(int numOfFiles)
        + openFiles(int numOfFiles)
        + write(string& line, int count, int& pos)
        + read(char* readBuffer, int count, int& pos)
        + closeAllFiles()
    }

    class File {
        -string filePrefix
        -string fileName
        -fstream fileObj
        -streamsize fileSize
        -Mode fileMode
        +static const int maxSize
        +File()
        +File(string name, Mode openMode)
        +~File()
        +openFile(string name, Mode openMode)
        +fillFileRandom()
        +fileIsOpen()
        +closeFile()
        +checkFileSize()
        +write(int count, const char* newData)
        +read(int count, char* readBuffer)
        +tellGet()
        +seekGet(int pos, Dir dir)
        +tellPut()
        +seekPut(int pos, Dir dir)
    }

    class Mode {
        <<enumeration>>
        APPEND
        BINARY
        READ_ONLY
        WRITE_ONLY
        READ_WRITE
        TRUNCATE
        APPEND_AT_END
    }

    class Dir {
        <<enumeration>>
        BEG
        CUR
        END
    }

    class AccessModifiers {
        <<legend>>
        + Public
        - Private
    }

    File --> Mode
    File --> Dir
    FileManager "1" *-- "0..*" File : filesRecord
```

## Explanation

This project's main purpose is to have a large file which is composed of multiple small files. Each small file should not exceed the maximum size. The large file will navigate to the correct file corresponding to the byte number. Then it can either read bytes from there or write character bytes there, overwriting the previously written data. The large file will know when it has reached the end of the current small file and switches to the next file. The large file is represented by the `FileManager` class and the small file by `File` class.