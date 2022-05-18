#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <mutex>
#include "doublebuffer.h"

using namespace std;
using namespace this_thread;
using namespace chrono;

#define BUFFER_SIZE 20
#define CONSUMERS 4
#define PRODUCERS 4
#define ROUNDS 2
#define REC_DELAY 20


DoubleBuffer buffer(BUFFER_SIZE);

mutex coutlck;
#define LOCK_STDOUT   coutlck.lock()
#define UNLOCK_STDOUT coutlck.unlock()

void produce(int id) {
    LOCK_STDOUT;
    cout << "Producer" << id << ": Starting to produce" << endl;
    UNLOCK_STDOUT;
    for(int i=0; i<ROUNDS; i++) {
        LOCK_STDOUT;
        cout << "Producer"<< id << ": Round #" << i << endl;
        UNLOCK_STDOUT;
        for(int j=0; j<BUFFER_SIZE;j++) {
            default_random_engine generator;
            uniform_int_distribution<int> distribution(42, 1337);
            buffer.write(j, distribution(generator));
            /* Simulate data recording */
            sleep_for (milliseconds(REC_DELAY));
        }
    }
    LOCK_STDOUT;
    cout << "Producer" << id << ": Finished with producing" << endl;
    UNLOCK_STDOUT;
}

void consume(int id) {
    int round = 0;
    int sequence = -1;
    int old_sequence;
    LOCK_STDOUT;
    cout << "Consumer" << id << ": Starting to consume" << endl;
    UNLOCK_STDOUT;
    while(round < ROUNDS) {
        LOCK_STDOUT;
        cout << "Consumer" << id << ": Waiting for data to be ready." << endl;
        UNLOCK_STDOUT;
        /* Wait until the producer has produced a full buffer */
        old_sequence = sequence;
        sequence = buffer.wait_until_full(sequence);
        LOCK_STDOUT;
        cout << "Consumer" << id << ": Data is ready. sequence changed:" << old_sequence << "->" << sequence << endl;
        UNLOCK_STDOUT;
        round++;
    }
    LOCK_STDOUT;
    cout << "Consumer" << id << ": Finished with consuming" << endl;
    UNLOCK_STDOUT;
}

int main() {
    thread threads[CONSUMERS+PRODUCERS];
    int i=0;
    for(; i<CONSUMERS; i++) {
        threads[i] = thread(consume, i);
    }
    /* Wait for all consumers */
    sleep_for (milliseconds(500));
    int aux = 0;
    for(; i<CONSUMERS+PRODUCERS; i++) {
        threads[i] = thread(produce, aux);
        aux ++;
    }
    /* Wait for all producers */
    sleep_for (milliseconds(500));
//    threads[CONSUMERS+PRODUCERS-1] = thread(produce);
    /* Join all threads */
    for(auto& th : threads) th.join();

    return 0;
}