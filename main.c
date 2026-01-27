#include <stdio.h>
#include <pthread.h>
#include "Application.h"

int main()
{
    pthread_t thread_id; 
    uint8_t ucResult = pthread_create(&thread_id, NULL, ApplicationServerThread, NULL);
    if (ucResult) {
        printf("ERROR: return code from pthread_create() is %d\n", ucResult);
        return 1;
    }

    pthread_join(thread_id, NULL);
    return 0;
}