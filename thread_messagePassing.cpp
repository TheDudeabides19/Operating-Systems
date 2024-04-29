#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;
mutex mtx;
condition_variable cv;
queue<int> msg_queue;
44
void producer() {
cout << "Producer thread started" << endl;
// simulate producing messages
for (int i = 1; i <= 5; ++i) {
this_thread::sleep_for( chrono::seconds(1));
{
lock_guard< mutex> lock(mtx);
msg_queue.push(i);
cout << "Produced message " << i << endl;
}
cv.notify_one();
}
}
45
void consumer() {
cout << "Consumer thread started" << endl;
// simulate processing messages
while (true) {
unique_lock< mutex> lock(mtx);
cv.wait(lock, []{return !msg_queue.empty();});
int message = msg_queue.front();
msg_queue.pop();
cout << "Consumed message " << message << endl;
lock.unlock();
if (message == 5) {
break;
}
}
} 46
int main() {
thread t1(producer);
thread t2(consumer);
t1.join();
t2.join();
cout << "Main thread finished" << endl;
return 0;
}
47
