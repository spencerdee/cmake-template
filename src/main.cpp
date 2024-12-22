#include <cstdio>
#include <iostream>
#include "LinkedList.h"
#include "BaseClassThread.h"

class TestClass : public BaseClass {
    public:
        void run() override {
            std::cout << "Thread Test" << std::endl;  
        }
};

int main(int argc, char *argv[])
{
    printf("Hello World\n");
    PreallocatedLinkedList<int> l = PreallocatedLinkedList<int>(20);
    for (int i = 0; i < 10; i++) {
        l.append(i);
    }

    l.insertAfter(l.first(), 42);
    
    printf("Length: %i\n", l.length());
    printf("Size: %i\n", l.size());

    LinkedListNode<int>* node = l.first();
    while (node != nullptr) {
        printf("%i\n", node->data);
        node = node->next;
    }

    TestClass* test = new TestClass();
    BaseClassThread* testClass = new BaseClassThread(test);
    testClass->start();
    std::cout << "main thread" << std::endl;
    testClass->end();
}
