#ifndef BARRIER_H_
#define BARRIER_H_

#include <semaphore.h>
#include <pthread.h>

class Barrier {
private:
    unsigned int numOfThreads;
    unsigned int numOfFinishedThreads;

public:
    Barrier(unsigned int num_of_threads);
    void wait();
    ~Barrier();
    unsigned int waitingThreads();

	// TODO: define the member variables
	// Remember: you can only use semaphores!
};

#endif // BARRIER_H_

