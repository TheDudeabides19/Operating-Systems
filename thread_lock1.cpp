#include <iostream>
#include <thread>
#include <mutex>
using namespace std;
mutex mtx; // declare mutex
void increment_counter (int& counter) {
for (int i = 0; i < 100000; i++) {
mtx.lock(); // acquire lock
counter++;
mtx.unlock(); // release lock
}
}
18
int main() {
int counter = 0;
thread t1 (increment_counter, ref(counter));
thread t2 (increment_counter, ref(counter));
t1.join(); t2.join();
cout << "Counter value: " << counter << endl;
return 0;
}
19
