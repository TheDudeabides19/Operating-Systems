#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <random>
#include <chrono>
#include <string>

using namespace std;

// Structure for request
struct RequestStructure {
    int request_id;
    string ip_address;
    string page_requested;
};

// Global variables
mutex mtx;
condition_variable cv;
queue<RequestStructure> msg_queue;
string webPages[10] = {"google.com", "yahoo.com", "bing.com", "wikipedia.org", "amazon.com", 
                       "facebook.com", "twitter.com", "linkedin.com", "instagram.com", "reddit.com"};
int request_counter = 0;
bool stop_flag = false;

// Function to simulate listening for requests
void listen() {
    while (!stop_flag) {
        // Sleep for a random time between 1-3 seconds
        this_thread::sleep_for(chrono::seconds(rand() % 3 + 1));

        // Generate a request
        RequestStructure request;
        request.request_id = ++request_counter;
        request.ip_address = "";
        request.page_requested = webPages[rand() % 10];

        // Lock the mutex before modifying shared data
        unique_lock<mutex> lock(mtx);
        // Add request to the message queue
        msg_queue.push(request);
        // Notify one thread to consume the request
        cv.notify_one();
    }
}

// Function to process requests
void do_request(int thread_id) {
    while (true) {
        // Lock the mutex before accessing shared data
        unique_lock<mutex> lock(mtx);
        // Wait if the message queue is empty
        cv.wait(lock, [] { return !msg_queue.empty() || stop_flag; });

        // Check if it's time to stop
        if (stop_flag && msg_queue.empty()) {
            return;
        }

        // Retrieve the request from the queue
        RequestStructure request = msg_queue.front();
        msg_queue.pop();

        // Release the lock before printing
        lock.unlock();

        // Display the request processing
        cout << "Thread " << thread_id << " completed request " << request.request_id
             << " requesting webpage " << request.page_requested << endl;
    }
}

int main() {
    // Number of consumer threads
    const int num_threads = 5;
    // Array to hold consumer threads
    thread consumer_threads[num_threads];

    // Start the listener thread
    thread listener_thread(listen);

    // Start consumer threads
    for (int i = 0; i < num_threads; ++i) {
        consumer_threads[i] = thread(do_request, i + 1);
    }

    // Join listener thread
    listener_thread.join();

    // Set stop flag and notify consumer threads to stop
    {
        lock_guard<mutex> lock(mtx);
        stop_flag = true;
    }
    cv.notify_all();

    // Join consumer threads
    for (int i = 0; i < num_threads; ++i) {
        consumer_threads[i].join();
    }

    return 0;
}
