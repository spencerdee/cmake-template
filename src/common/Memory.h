#ifndef memoryh
#define memoryh

#include <windows.h>
#include <stdexcept>
#include <string>
using namespace std;


class FileMemory {
    public:
        FileMemory(size_t bytes) {
            if (bytes < 1) {
                throw invalid_argument("Linked List size must be > 0, cannot be " + to_string(bytes) + ".");
            }

            SYSTEM_INFO si;
            GetSystemInfo(&si);
            size = (bytes / si.dwPageSize + 1) * si.dwPageSize;

            hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, NULL);

            mem = MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,
                        0,
                        size);

            if (mem == nullptr) {
                printf("Error creating backing memory.\n");
                return;
            }

            memory = mem;
        }

        ~FileMemory() {
            UnmapViewOfFile(mem);
            CloseHandle(hMapFile);
        }

        void* memory;
        size_t size;

    private:
        void* hMapFile;
        void* mem;
};

#endif