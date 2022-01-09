#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstddef>
#include <cstdlib>
#include "common.h"
#include "shared_memory.h"

int main(int argc, char *argv[])
{
    SharedMemory sharedMem(true, FileName, FileSize);
    sharedMem.set(std::string("HI THERE"));
    // sharedMem.checkMemoryContent();

    std::cout << "Press ENTER to close" << std::endl;
    std::cin.get();

    return 0;
}