#include <iostream>
#include <thread>

using namespace std;

void testFunc() {
    cout << "Hello World!" << endl;
}

int main()
{
    thread t1([]{
        cout<<"Hello World!"<<endl;
    });

    int i = 0;
    try {
        i = 4/0;
    } catch (exception ) {
        cout<<"You can't divide"<<endl;
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
