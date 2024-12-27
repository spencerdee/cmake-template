#include <cstdio>
#include <chrono>
#include "LinkedList.h"

int main(int argc, char *argv[])
{
    printf("Hello World\n");

    int s = 10000;

    auto start = std::chrono::high_resolution_clock::now();
    LinkedListFile<int> l1 = LinkedListFile<int>(s);
    
    for (int i = 0; i < s; i++) {
        l1.append(i);
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    printf("final time file = %f\n", elapsed.count());

    start = std::chrono::high_resolution_clock::now();
    LinkedListFile<int> l2 = LinkedListFile<int>(4000);
    
    for (int i = 0; i < s; i++) {
        l2.append(i);
    }

    end = std::chrono::high_resolution_clock::now();

    elapsed = end - start;

    printf("final time malloc = %f\n", elapsed.count());
}
