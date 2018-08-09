/*
 * An example with RAII to wait for a thread to complete
 */

#include <iostream>
#include <thread>
#include <unistd.h>

class thread_guard
{
    std::thread &t;

public:
    explicit thread_guard(std::thread &t_):
        t(t_)
    {}
    ~thread_guard()
    {
        //check that thread is joinable is important, because
        //it can be joined before
        if(t.joinable())
        {
            //if there is no join - thread object goes out of scope
            //and program suddenly terminates
            t.join();
        }
    }
    //mark copy constructor and copu operator as delete
    //to ensure that they're not automatically
    //provided by compilator
    //It could outlive the scope of the thread it was joining
    thread_guard(thread_guard const&)=delete;
    thread_guard& operator =(thread_guard const&)=delete;
};

extern void testFunc();

void f() {
    std::thread t(testFunc);
    thread_guard g(t);

    std::cout<<"Some sign"<<std::endl;
    for(int i = 0; i < 500; i++);
   // t.join();

    //at the end of this function - guard desroys
    //and in destructor we have a join method which will
    //wait till the end of the thread
}

int main_example() {
    f();
    return 0;
}
