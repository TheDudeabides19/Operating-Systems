#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready1 = false;
bool ready2 = false;

void runMeFirst() {
    std::cout << "Run me first\n";
    // Notify that the first function has finished
    ready1 = true;
    cv.notify_all();
}

void runMeSecond() {
    std::unique_lock<std::mutex> lock(mtx);
    // Wait until the first function has finished
    cv.wait(lock, []{ return ready1; });
    std::cout << "I run second\n";
    // Notify that the second function has finished
    ready2 = true;
    cv.notify_one();
}

void runMeThird() {
    std::unique_lock<std::mutex> lock(mtx);
    // Wait until the second function has finished
    cv.wait(lock, []{ return ready2; });
    std::cout << "I run third\n";
}

int main() {
    // Create threads
    std::thread t1(runMeFirst);
    std::thread t2(runMeSecond);
    std::thread t3(runMeThird);

    // Join threads to wait for their completion
    t1.join();
    t2.join();
    t3.join();

    return 0;
}
