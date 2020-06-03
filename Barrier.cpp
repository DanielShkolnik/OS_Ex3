//
// Created by danie on 01/06/2020.
//

#include "Barrier.h"

pthread_mutex_t atomicLock;
sem_t barrierSem;
sem_t barrierOutSem;

Barrier::Barrier(unsigned int num_of_threads):numOfThreads(num_of_threads),numOfFinishedThreads(0),numOfOutFinishedThreads(0){
    pthread_mutex_init(&atomicLock, NULL);
    sem_init(&barrierSem, 0, 0);
    sem_init(&barrierOutSem, 0, 0);
}

void Barrier::wait(){
    pthread_mutex_lock(&atomicLock);
    this->numOfFinishedThreads++;
    if(this->numOfFinishedThreads==this->numOfThreads){
        sem_post(&barrierSem);
    }
    pthread_mutex_unlock(&atomicLock);
    sem_wait(&barrierSem);

    pthread_mutex_lock(&atomicLock);
    this->numOfFinishedThreads--;
    if(this->numOfFinishedThreads!=0){
        sem_post(&barrierSem);
    }
    pthread_mutex_unlock(&atomicLock);


    pthread_mutex_lock(&atomicLock);
    this->numOfOutFinishedThreads++;
    if(this->numOfOutFinishedThreads==this->numOfThreads){
        sem_post(&barrierOutSem);
    }
    pthread_mutex_unlock(&atomicLock);
    sem_wait(&barrierOutSem);

    pthread_mutex_lock(&atomicLock);
    this->numOfOutFinishedThreads--;
    if(this->numOfOutFinishedThreads!=0){
        sem_post(&barrierOutSem);
    }
    pthread_mutex_unlock(&atomicLock);

}

Barrier::~Barrier(){
    pthread_mutex_destroy(&atomicLock);
    sem_destroy(&barrierSem);
    sem_destroy(&barrierOutSem);
}
