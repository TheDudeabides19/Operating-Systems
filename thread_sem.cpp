#include <iostream>
#include <thread>
#include <semaphore.h>
using namespace std;
sem_t sem;
31
void thread_func(int id) {
cout << "Thread " << id << " is waiting..." << endl;
sem_wait(&sem); // blocking call
cout << "Thread " << id << " has acquired the semaphore";
cout << endl;
// do some work here...
this_thread::sleep_for(chrono::seconds(2));
cout << "Thread " << id << " is releasing the semaphore";
cout << endl;
sem_post(&sem);
}
32
int main() {
const int num_threads = 5;
// initialize the semaphore with the number of threads
sem_init(&sem, 0, num_threads); // 0: thread, 1: process
thread threads[num_threads];
for (int i = 0; i < num_threads; ++i) {
threads[i] = thread(thread_func, i);
}
for (int i = 0; i < num_threads; ++i) {
threads[i].join();
}
sem_destroy(&sem); // destroy the semaphore
return 0;
}
33
