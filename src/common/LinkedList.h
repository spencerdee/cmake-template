#include <stack>
#include <string>
#include <stdexcept>
#include <iostream>
#include <windows.h>
using namespace std;

/**
 * Linked List Node with set Template
 */
template <class T> struct LinkedListNode {
    T data;
    LinkedListNode<T>* next;
    LinkedListNode<T>* prev;
};

// interface for different linked list implementations
template <class T> class LinkedList {
    public:
        LinkedListNode<T>* first() {
            return head;
        }

        LinkedListNode<T>* last() {
            return tail;
        }

        int size() {
            return allocated;
        }

        int length() {
            return len;
        }

        int append(T data) {
            if (len < allocated) {
                LinkedListNode<T>* node = freeNodes.top();
                freeNodes.pop();
                node->data = data;
                node->next = nullptr;
                if (head != nullptr) {
                    node->prev = tail;
                    tail->next = node;
                } else {
                    head = node;
                }
                tail = node;
                len++;
                return 1;
            } else {
                return 0;
            }
        }

        int insertAfter(LinkedListNode<T>* loc, T data) {
            if (loc >= memBoundLower && loc <= memBoundUpper &&
                len < allocated && loc != nullptr) {
                if (loc == tail) {
                    return append(data);
                }
                LinkedListNode<T>* newNode = freeNodes.top();
                freeNodes.pop();
                newNode->data = data;
                if (loc->next != nullptr) {
                    loc->next->prev = newNode;
                }
                newNode->next = loc->next;
                newNode->prev = loc;
                loc->next = newNode;
                len++;
                return 1;
            }
            std::cerr << "Unable to insert into list.\n";
            return 0;
        }

    protected:
        LinkedList() {};
        ~LinkedList() {};

        stack<LinkedListNode<T>*> freeNodes;
        LinkedListNode<T>* head;
        LinkedListNode<T>* tail;
        void* memBoundLower;
        void* memBoundUpper;
        int allocated; // total allocated space (full nodes + empty nodes)
        int len; // total unfilled nodes
};

template <class T> struct LinkedListFile : public LinkedList<T> {
    public:
        LinkedListFile<T>(int size) {
            if (size < 1) {
                throw invalid_argument("Linked List size must be > 0, cannot be " + to_string(size) + ".");
            }

            SYSTEM_INFO si;
            GetSystemInfo(&si);
            uint32_t memSize = (size * sizeof(LinkedListNode<T>) / si.dwPageSize + 1) * si.dwPageSize;

            size = memSize / sizeof(LinkedListNode<T>);

            hMapFile = (LinkedListNode<T>*) CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, memSize, NULL);

            mem = MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,
                        0,
                        memSize);

            if (mem == nullptr) {
                printf("Error creating backing memory.\n");
                return;
            }

            for (int i = 0; i < size; i++) {
                this->freeNodes.push(&((LinkedListNode<T>*)mem)[i]);
            }

            this->allocated = size;
            this->len = 0;
            this->head = nullptr;
            this->tail = nullptr;
            this->memBoundLower = mem;
            this->memBoundUpper = (LinkedListNode<T>*) mem + memSize;
        }

        ~LinkedListFile<T>() {
            UnmapViewOfFile(mem);
            CloseHandle(hMapFile);
        }

    protected:
        HANDLE hMapFile;
        void* mem;
};

template <class T> class LinkedListPreallocated : public LinkedList<T> {
    public:
        LinkedListPreallocated(int size) {
            if (size < 1) {
                throw invalid_argument("Linked List size must be > 0, cannot be " + to_string(size) + ".");
            }
            nodes = (LinkedListNode<T>*) malloc(sizeof(LinkedListNode<T>) * size);
            for (int i = 0; i < size; i++) {
                nodes[i] = LinkedListNode<T>();
                this->freeNodes.push(&nodes[i]);
            }
            this->allocated = size;
            this->len = 0;
            this->head = nullptr;
            this->tail = nullptr;
            this->memBoundLower = nodes;
            this->memBoundUpper = nodes + sizeof(LinkedListNode<T>) * size;
        }

        ~LinkedListPreallocated<T>() {
            free(nodes);
        }
    
    protected:
        LinkedListNode<T>* nodes;
};
