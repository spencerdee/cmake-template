#include <stack>
#include <string>
#include <stdexcept>
using namespace std;

/**
 * Linked List Node with set Template
 */
template <class T> struct LinkedListNode {
    T data;
    LinkedListNode<T>* next;
    LinkedListNode<T>* prev;
};


/**
 * Pre
 */
template <class T> struct PreallocatedLinkedList {
    private:
        stack<LinkedListNode<T>*> freeNodes;
        LinkedListNode<T>* head;
        LinkedListNode<T>* tail;
        LinkedListNode<T>* nodes;
        int allocated; // total allocated space (full nodes + empty nodes)
        int len; // total unfilled nodes
    public:
        PreallocatedLinkedList<T>(int size) {
            if (size < 1) {
                throw invalid_argument("Linked List size must be > 0, cannot be " + to_string(size) + ".");
            }
            nodes = (LinkedListNode<T>*) malloc(sizeof(LinkedListNode<T>) * size);
            for (int i = 0; i < size; i++) {
                nodes[i] = LinkedListNode<T>();
                freeNodes.push(&nodes[i]);
            }
            allocated = size;
            len = 0;
        }

        ~PreallocatedLinkedList<T>() {
            free(nodes);
        }

        LinkedListNode<T>* first() {
            return head;
        }

        int size() {
            return allocated;
        }

        int length() {
            return len;
        }

        int add(T data) {
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
                    tail = node;
                }
                len++;
                return 1;
            } else {
                return 0;
            }
        }
};

