#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>

using namespace boost::interprocess;

int main(int argc, char *argv[])
{

    // Open already created shared memory object.
    shared_memory_object shm(open_only, "MySharedMemory", read_only);

    // Map the whole shared memory in this process
    mapped_region region(shm, read_only);

    // Check that memory was initialized to 1
    char *mem = static_cast<char *>(region.get_address());
    for (std::size_t i = 0; i < region.get_size(); ++i)
    {
        if (*mem++ != 1)
        {
            std::cout << "Error checking memory" << std::endl;
            return 1; // Error checking memory
        }
    }
    return 0;
}