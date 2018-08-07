#include <iostream>
#include <thread>

using namespace std;

void testFunc() {
    for(int i = 0; i < 100500; i++) {
        std::cout<<i<<" ";
    }
}

int main_example()
{
    thread t1([]{
        for(int i =0; i < 100500; i++) {
            cout<<i<<" ";
        }
    });

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
    //detach - you've decided not to wait for the thread ending
    //join - thread completes execution before thread exits
}

//if you decided to wait the tread with join
//you have to take care about exceptions
//if exception has been thrown before join, you have to join in catch block

//before the listing 2.3!
