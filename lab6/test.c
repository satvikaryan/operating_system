#include <pthread.h>
#include<unistd.h>
#include <stdio.h>

void myturn(){
    int i = 0;

    while(1){
        i++;
        sleep(1);
        printf("my turn\n");
    }
    
}



void* yourturn(void *arg){
    int i = 0;
    while(1){
        sleep(1);
        i++;
        printf("your turn\n");
    }
    return NULL;
}


int main(){
    pthread_t newthread;

    pthread_create(&newthread, NULL,yourturn,NULL);
    pthread_join(newthread,NULL);
    myturn();
    // yourturn();
    return 0;
}
