//
// Created by danie on 03/06/2020.
//

#include <iostream>
#include <semaphore.h>
#include <pthread.h>

int sum=0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
int ids[10]={1,2,3,4,5,6,7,8,9,10};
void* thread_workload(void* threadID){
    int* p_val = (int*) threadID;
    pthread_mutex_lock(&mutex);
    sum += *p_val;
    pthread_mutex_unlock(&mutex);
    return NULL;
}
int main(){
    pthread_t threads[10];
    //pthread_t t;
    int i;
    for(i=0;i<10;++i) {
        //pthread_create(&t, NULL, &thread_workload, (void *) (ids + i));
        pthread_create(&threads[i], NULL, &thread_workload, (void *) (ids + i));
    }
    //pthread_join(t,NULL);
    for (unsigned int j=0; j<10; j++){
        pthread_join(threads[j], NULL);
    }
    printf("%d\n", sum);
    return 0;
}
