#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void runMeFirst() {
    std::cout << "Run me first\n";
    // Notify that the first function has finished
    ready = true;
    cv.notify_one();
}

void runMeSecond() {
    std::unique_lock<std::mutex> lock(mtx);
    // Wait until the first function has finished
    cv.wait(lock, []{ return ready; });
    std::cout << "I run second\n";
}

int main() {
    // Create threads
    std::thread t1(runMeFirst);
    std::thread t2(runMeSecond);

    // Join threads to wait for their completion
    t1.join();
    t2.join();

    return 0;
}
