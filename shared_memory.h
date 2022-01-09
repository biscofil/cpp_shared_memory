#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;

class SharedMemory
{
private:
    const bool host;
    boost::interprocess::mode_t mode;
    const std::string FileName;
    const std::size_t FileSize;
    void *addr;
    std::size_t size;
    std::shared_ptr<file_mapping> m_file;
    std::shared_ptr<mapped_region> region;

public:
    SharedMemory(bool h, const std::string &fn, const std::size_t &fs) : host(h), FileName(fn), FileSize(fs)
    {
        if (host)
        {
            createFile();

            // Map the whole file with read-write permissions in this process
            mode = read_write;
        }
        else
        {
            std::cout << "Open the file mapping and map it as read-only..." << std::endl;
            mode = read_only;
        }

        // Create a file mapping
        std::cout << "Create a file mapping..." << std::endl;

        m_file = std::make_shared<file_mapping>(FileName.c_str(), mode);

        region = std::make_shared<mapped_region>(*m_file, mode);
        // TODO: create multiple regions

        fetchAddressAndSize();
    }

    ~SharedMemory()
    {
        if (host)
        {
            // Remove on exit
            deleteFile();
        }
    }

    void fetchAddressAndSize()
    {
        // Get the address of the mapped region
        std::cout << "Get the address of the mapped region..." << std::endl;
        addr = region->get_address();
        std::cout << "\tAddress:\t" << addr << std::endl;
        size = region->get_size();
    }

    void createFile()
    {
        if (not host)
        {
            throw "MUST BE HOST";
        }
        // Create a file
        std::cout << "Creating file " << FileName << std::endl;
        file_mapping::remove(FileName.c_str());
        std::filebuf fbuf;
        fbuf.open(FileName, std::ios_base::in | std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
        // Set the size
        fbuf.pubseekoff(FileSize - 1, std::ios_base::beg);
        fbuf.sputc(0);
        std::cout << "File created" << std::endl;
    }

    void deleteFile()
    {
        if (not host)
        {
            throw "MUST BE HOST";
        }
        std::cout << "Deleting file " << FileName << std::endl;
        file_mapping::remove(FileName.c_str());
    }

    void init()
    {
        if (not host)
        {
            throw "MUST BE HOST";
        }
        // Write all the memory to 1
        std::memset(addr, 1, size);
    }

    void set(const std::string &src)
    {
        if (not host)
        {
            throw "MUST BE HOST";
        }
        if (src.length() > FileSize)
        {
            throw "STR too long";
        }
        // Write all the memory to 0
        std::memset(addr, 0, size);
        // Copy
        std::strcpy(static_cast<char *>(addr), src.c_str());
    }

    void checkMemoryContent()
    {
        // Check that memory was initialized to 1
        std::cout << "Check that memory was initialized to 1..." << std::endl;
        const char *mem = static_cast<char *>(addr);
        for (std::size_t i = 0; i < size; ++i)
        {
            if (*mem++ != 1)
            {
                std::cout << " Error checking memory" << std::endl;
                exit(1); // Error checking memory
            }
        }
    }

    std::vector<char> readFileRaw()
    {
        // Now test it reading the file
        std::cout << "Now test it reading the file..." << std::endl;
        std::filebuf fbuf;
        fbuf.open(FileName, std::ios_base::in | std::ios_base::binary);

        // Read it to memory
        std::cout << "Read it to memory..." << std::endl;
        std::vector<char> vect(FileSize, 0);
        fbuf.sgetn(&vect[0], std::streamsize(vect.size()));

        return vect;
    }
};