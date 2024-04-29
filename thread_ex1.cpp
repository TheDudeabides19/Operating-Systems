#include <iostream>
#include <thread>
using namespace std;
void thread1(int & c) { for (int i=0; i<100; i++) c++; }
void thread2(int & c) { for (int i=0; i<100; i++) c++; }
int main() {
thread t1, t2;
int counter = 0;
t1 = thread(thread1, ref(counter));
t2 = thread(thread2, ref(counter));
t1.join(); t2.join();
return 0;
}
10
