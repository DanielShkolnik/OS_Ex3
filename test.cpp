//
// Created by danie on 01/06/2020.
//

#include "Barrier.h"
#include <iostream>

pthread_mutex_t atomicLockk;

#define N 100
int i = 0;
Barrier barrier(N);

void* print(void* arg) {
    pthread_mutex_lock(&atomicLockk);
    std::cout << "Start Thread number " << i << std::endl;
    i++;
    pthread_mutex_unlock(&atomicLockk);
    barrier.wait();
    pthread_mutex_lock(&atomicLockk);
    i--;
    std::cout << "Finish Thread number " << i << std::endl;
    pthread_mutex_unlock(&atomicLockk);
    return NULL;
}

int main() {
    pthread_mutex_init(&atomicLockk, NULL);
    pthread_t threads[N];
    for (unsigned int j=0; j<N; j++){
        pthread_create(&threads[j], NULL, print, NULL);
    }
    for (unsigned int j=0; j<N; j++){
        pthread_join(threads[j], NULL);
    }
    return 0;
}




