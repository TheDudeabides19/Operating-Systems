#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;
mutex mtx; // declare mutex
condition_variable cv; // declare condition variable
bool ready = false; // shared flag
25
void print_message() {
unique_lock<mutex> lck(mtx);
while (!ready) cv.wait(lck); //cv.wait(lck,[]{return !ready;});
cout << "Hello, world!" << endl;
}
void prepare_message() {
// simulate work
this_thread::sleep_for( chrono::seconds(1));
ready = true;
cv.notify_one(); // notify waiting threads
}
26
int main() {
thread t1(print_message);
thread t2(prepare_message);
t1.join();
t2.join();
return 0;
}
27
