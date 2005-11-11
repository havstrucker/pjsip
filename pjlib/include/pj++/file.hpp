/* $Id$ 
 */
#ifndef __PJPP_FILE_HPP__
#define __PJPP_FILE_HPP__

#include <pj/file_io.h>
#include <pj/file_access.h>
#include <pj++/types.hpp>
#include <pj++/pool.hpp>

//
// File API.
//
class Pj_File_API
{
public:
    //
    // Check file existance.
    //
    static bool file_exists(const char *filename)
    {
        return pj_file_exists(filename) != 0;
    }

    //
    // Get file size.
    //
    static pj_off_t file_size(const char *filename)
    {
        return pj_file_size(filename);
    }

    //
    // Delete file.
    //
    static pj_status_t file_delete(const char *filename)
    {
        return pj_file_delete(filename);
    }

    //
    // Move/rename file.
    //
    static pj_status_t file_move(const char *oldname, const char *newname)
    {
        return pj_file_move(oldname, newname);
    }

    //
    // Get stat.
    //
    static pj_status_t file_stat(const char *filename, pj_file_stat *buf)
    {
        return pj_file_getstat(filename, buf);
    }
};


//
// File.
//
class Pj_File : public Pj_Object
{
public:
    //
    // Offset type to be used in setpos.
    //
    enum Offset_Type
    {
        SEEK_SET = PJ_SEEK_SET,
        SEEK_CUR = PJ_SEEK_CUR,
        SEEK_END = PJ_SEEK_END,
    };

    //
    // Default constructor.
    //
    Pj_File()
        : hnd_(0)
    {
    }

    //
    // Construct and open a file.
    //
    Pj_File(Pj_Pool *pool, const char *filename,
            unsigned access = PJ_O_RDONLY)
    : hnd_(NULL)
    {
        open(pool, filename, access);
    }

    //
    // Destructor closes the file.
    //
    ~Pj_File()
    {
        close();
    }

    //
    // Open a file.
    //
    pj_status_t open(Pj_Pool *pool, const char *filename, 
                     unsigned access = PJ_O_RDONLY )
    {
        close();
        return pj_file_open(pool->pool_(), filename, access, &hnd_);
    }

    //
    // Close a file.
    //
    void close()
    {
        if (hnd_ != 0) {
            pj_file_close(hnd_);
            hnd_ = 0;
        }
    }

    //
    // Write data.
    //
    pj_ssize_t write(const void *buff, pj_size_t size)
    {
        pj_ssize_t bytes = size;
        if (pj_file_write(hnd_, buff, &bytes) != PJ_SUCCESS)
            return -1;
        return bytes;
    }

    //
    // Read data.
    //
    pj_ssize_t read(void *buf, pj_size_t size)
    {
        pj_ssize_t bytes = size;
        if (pj_file_read(hnd_, buf, &bytes) != PJ_SUCCESS)
            return -1;
        return bytes;
    }

    //
    // Set file position.
    //
    pj_status_t setpos(pj_off_t offset, Offset_Type whence)
    {
        return pj_file_setpos(hnd_, offset, 
                              (enum pj_file_seek_type)whence);
    }

    //
    // Get file position.
    //
    pj_off_t getpos()
    {
        pj_off_t pos;
        if (pj_file_getpos(hnd_, &pos) != PJ_SUCCESS)
            return -1;
        return pos;
    }

private:
    pj_oshandle_t hnd_;
};



#endif  /* __PJPP_FILE_HPP__ */

