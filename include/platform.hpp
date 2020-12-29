#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>


#include "containers/vector.hpp"
#include "containers/string.hpp"
#include "containers/map.hpp"


#include "memory.hpp"


struct FileHandle
{
    int fd{0};

    operator bool()
    {
        return fd != 0;
    }
};

enum class FileMode
{
    Read,
    Write
};

struct Platform
{
  private:
    inline static Vector<FileHandle> m_ClosedFds{};

  public:
    static void init()
    {
        m_ClosedFds.init(16);
    }

    static void destory()
    {
        burstWork();
        
    }

    static void burstWork()
    {
        auto fds = m_ClosedFds.data();
        for (size_t i = 0; i < m_ClosedFds.count(); ++i)
        {
            close(fds[i].fd);
        }


    }


    // File functions
    
    static FileHandle openFile(const char* t_FileName, FileMode t_Mode = FileMode::Read)
    {
        int fd;
        if (t_Mode == FileMode::Read)
        {
            fd = open(t_FileName, O_RDONLY, S_IRUSR | S_IWUSR);
        } else if (t_Mode == FileMode::Write)
        {
            fd = open(t_FileName, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        }
        
        return {fd};
        }

    static String readWholeFile(FileHandle t_Fd)
    {
        struct stat statBuf;
        fstat(t_Fd.fd, &statBuf);
        const auto fileSize =  statBuf.st_size;

        String fileContents;
        fileContents.init(fileSize);

        // @Note(Stan) : This can probably be empirically optimized
        char buf[Kilobytes(1)];
        ssize_t readBytes;
        while ((readBytes = read(t_Fd.fd, buf, Kilobytes(1))) > 0)
        {
            fileContents.append(buf, (size_t)readBytes);
        }

        return fileContents;

    }

    static void closeFile(FileHandle t_Fd)
    {
        m_ClosedFds.push_back(t_Fd);
        
    }

    // Printing functions

    static void print(String t_Contents)
    {
        write(STDOUT_FILENO, t_Contents.data(), t_Contents.count());
    }
    
    static void print(const char* t_Contents)
    {
        write(STDOUT_FILENO, t_Contents, strlen(t_Contents));
    }
    

};
