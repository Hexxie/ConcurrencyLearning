#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex m;

void testFunc() {
    unique_lock<mutex>lk(m);
    for(int i = 0; i < 100500; i++) {
        std::cout<<i<<" ";
    }
}

void funcWithParam(int i, std::string const& s) {
    unique_lock<mutex>lk(m);
    for(int j = 0; j< i; j++)
        cout<<j<<" ";
    cout<<s<<endl;
}

int main()
{
    thread t1([]{
        unique_lock<mutex>lk(m);
        for(int i =0; i < 10; i++) {
            cout<<i<<" ";
        }
    });

    //Thread with parameter
    thread t2(funcWithParam, 4, "ThisIsParam");

    int i = 0;
    try {
        throw 20;
    } catch (int e) {
        cout<<"Throwed an exception!"<<endl;
        //This will wait until thread count till the end of the cycle
        //Otherwize it terminates under the throw
        t1.join();
        throw;
    }

    t1.join();
    t2.join();
    //detach - you've decided not to wait for the thread ending
    //join - thread completes execution before thread exits
}

//if you decided to wait the tread with join
//you have to take care about exceptions
//if exception has been thrown before join, you have to join in catch block

//before the listing 2.3!
