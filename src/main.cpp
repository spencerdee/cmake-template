#include <cstdio>
#include <chrono>
#include "LinkedList.h"
#include "Memory.h"

int main(int argc, char *argv[])
{
    printf("Hello World\n");

    int err = 0;

    int s = 10000;

    auto start = std::chrono::high_resolution_clock::now();
    LinkedListFile<int> l1 = LinkedListFile<int>(s);
    
    for (int i = 0; i < s; i++) {
        err = l1.append(i);
        if (err) {
            printf("Error appending to list.\n");
            break;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    printf("final time file = %f\n", elapsed.count());

    start = std::chrono::high_resolution_clock::now();
    LinkedListPreallocated<int> l2 = LinkedListPreallocated<int>(s);
    
    for (int i = 0; i < s; i++) {
        err = l2.append(i);
        if (err) {
            printf("Error appending to list.\n");
            break;
        }
    }

    end = std::chrono::high_resolution_clock::now();

    elapsed = end - start;

    printf("final time malloc = %f\n", elapsed.count());
}
