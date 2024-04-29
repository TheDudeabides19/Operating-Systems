#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
using namespace std;
mutex mtx;
condition_variable cv;
int counter = 0;
const int num_threads = 4;
39
void worker(int id) {
cout << "Thread " << id << " started" << endl;
this_thread::sleep_for( chrono::seconds(1));
unique_lock< mutex> lock(mtx);
counter++;
cout << "Thread " << id;
cout << " reached barrier (" << counter << "/" << num_threads << ")";
cout << endl;
if (counter == num_threads) {
cv.notify_all();
} else cv.wait(lock);
cout << "Thread " << id << " finished" << endl;
}
40
int main() {
cout << "Main thread started" << endl;
thread t1(worker, 1);
thread t2(worker, 2);
thread t3(worker, 3);
thread t4(worker, 4);
t1.join(); t2.join();
t3.join(); t4.join();
cout << "Main thread finished" << endl;
return 0;
}
41
