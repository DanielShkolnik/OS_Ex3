//
// Created by danie on 03/06/2020.
//

#include <iostream>
#include <semaphore.h>
#include <pthread.h>

int result;
void* do_calc(void* arg){
    int i;
    for(i=0; i<100; ++i) {
        result=result+1;
    }
    return NULL;
}
int main(){
    pthread_t threads[2];
    int i;
    result=0;
    for(i=0;i<2;++i)
        pthread_create(&threads[i],NULL,do_calc,NULL);
    for(i=0;i<2;++i)
        pthread_join(threads[i],NULL);
    printf("%d\n", result);
    return 0;
}
