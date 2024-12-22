#ifndef basethreadh
#define basethreadh

#include <thread>

class BaseClass {
    public:
        virtual void run() {};
};

class BaseClassThread {
    public:
        BaseClassThread(BaseClass* baseClass) : baseClass(baseClass) {};
        ~BaseClassThread() {};
        int end() {
            thread.join();
            return 0;
        };

        void start() {
            thread = std::thread(&BaseClass::run, baseClass);
        };
        
    private:
        std::thread thread;
        
        BaseClass* baseClass;
};
#endif // basethreadh