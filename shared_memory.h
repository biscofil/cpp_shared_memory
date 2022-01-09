#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <boost/interprocess/shared_memory_object.hpp>
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
    std::shared_ptr<shared_memory_object> m_file;
    std::shared_ptr<mapped_region> region;

public:
    SharedMemory(bool h, const std::string &fn, const std::size_t &fs) : host(h), FileName(fn), FileSize(fs)
    {
        if (host)
        {

            shared_memory_object::remove(FileName.c_str());

            // Map the whole file with read-write permissions in this process
            mode = read_write;

            m_file = std::make_shared<shared_memory_object>(create_only, FileName.c_str(), mode);

            // Set size
            m_file->truncate(FileSize);
        }
        else
        {
            std::cout << "Open the file mapping and map it as read-only..." << std::endl;

            mode = read_only;

            m_file = std::make_shared<shared_memory_object>(open_only, FileName.c_str(), mode);
        }

        region = std::make_shared<mapped_region>(*m_file, mode); // !!!!!!!!!!!!!!!!!!!!!!!!!!
        // TODO: create multiple regions

        fetchAddressAndSize();
    }

    ~SharedMemory()
    {
        if (host)
        {
            // Remove on exit
            shared_memory_object::remove(FileName.c_str());
        }
    }

    void fetchAddressAndSize()
    {
        // Get the address of the mapped region
        std::cout << "Get the address and size of the mapped region..." << std::endl;
        addr = region->get_address();
        std::cout << "\tAddress:\t" << addr << std::endl;
        size = region->get_size();
        std::cout << "\tSize:\t\t" << size << std::endl;
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

    std::string readFileRaw()
    {
        return std::string(static_cast<char *>(addr));
    }
};