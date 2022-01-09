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

    std::cout << sharedMem.readFileRaw() << std::endl;

    return 0;
}