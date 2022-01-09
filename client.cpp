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

    SharedMemory sharedMem(false, FileName, FileSize);
    // sharedMem.checkMemoryContent();

    for (const auto &a : sharedMem.readFileRaw())
    {
        std::cout << a;
    }
    std::cout << std::endl;

    return 0;
}