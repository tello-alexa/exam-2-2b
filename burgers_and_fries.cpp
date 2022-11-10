#include <iostream>
#include <thread>
#include <semaphore.h>
#include <mutex>
#include <unistd.h>
#include <vector>
#include <algorithm>
using namespace std;
#define MAX_THREADS 100

#define BURGER 0
#define FRIES 1
const char* type_names[] = {"BURGER", "FRIES"};
#define pii pair<int, int>

int k;
sem_t sem;
//int type_to_check = -1;
vector<int> waiting;
mutex m[2];

// Do not change
void process_order() {
    sleep(2);
}

void place_order(int type) {
    // get number of orders currently processing
    int value;
    sem_getvalue(&sem, &value);

    // wait if k orders are processing
    if(value == 0) {
        cout << "Waiting: " << type_names[type] << endl;
        waiting.push_back(type);
        //m[type].lock();
    }
    sem_wait(&sem);

    // place and process order
    cout << "Order: " << type_names[type] << endl;
    process_order();        // Do not remove, simulates preparation

    // check for waiting orders of same type
    /*int index = *find(waiting.begin(), waiting.end(), type);
    if(index != *waiting.end()) {
        m[type].unlock();
    }*/
    sem_post(&sem);

}

int main() {
    
    // Read data: done for you, do not change
    pii incoming[MAX_THREADS];
    int _type, _arrival;
    int t;
    cin >> k;
    cin >> t;
    for (int i = 0; i < t; ++i) {
        cin >> _type >> _arrival;
        incoming[i].first = _type;
        incoming[i].second = _arrival;
    }

    // Initialize necessary variables, semaphores etc.
    sem_init(&sem, 0, k);

    // Create threads: done for you, do not change
    thread* threads[MAX_THREADS];
    for (int i = 0; i < t; ++i) {
        _type = incoming[i].first;
        threads[i] = new thread(place_order, _type);
        if (i < t - 1) {
            int _sleep = incoming[i + 1].second - incoming[i].second;
            sleep(_sleep);
        }
    }

    // Join threads: done for you, do not change
    for (int i = 0; i < t; ++i) {
        threads[i]->join();
        delete threads[i];
    }

    sem_destroy(&sem);
    return 0;
}