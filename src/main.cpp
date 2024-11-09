#include <cstdio>
#include <LinkedList.h>

int main(int argc, char *argv[])
{
    printf("Hello World\n");
    PreallocatedLinkedList<int> l = PreallocatedLinkedList<int>(10);
    for (int i = 0; i < 10; i++) {
        l.add(i);
    }
    
    printf("Length: %i\n", l.length());
    printf("Size: %i\n", l.size());

    LinkedListNode<int>* node = l.first();
    while (node != nullptr) {
        printf("%i\n", node->data);
        node = node->next;
    }
}
