#include <cstdio>
#include <LinkedList.h>

int main(int argc, char *argv[])
{
    printf("Hello World\n");
    PreallocatedLinkedList<int> l = PreallocatedLinkedList<int>(10);
    for (int i = 0; i < 10; i++) {
        printf("%i\n",     l.add(i));
    }
    
    printf("%i\n", l.length());
    printf("%i\n", l.size());
    printf("%i\n", l.first()->data);
}
