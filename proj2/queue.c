////////////////////////////////////////////////////////////////////////////////
// Main:        main.c
// File:        queue.c
//
// Names:       Garret Huibregtse Tyler Patton
// Emails:      ghuibregtse@wisc.edu tpatton4@wisc.edu
//
// Description: This file holds the implementation of the queue that we will be
//              using.
////////////////////////////////////////////////////////////////////////////////

#include "queue.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

/* @brief Initilizes the queue
 */
Queue *CreateStringQueue(int size) {
    Queue *q = (Queue*) malloc(sizeof(Queue));
    if (q == NULL) {
        fprintf(stderr, "Not enough memory for queue.\n");
        exit(1);
    }
    if (pthread_mutex_init(&q->lock, NULL)) {
        fprintf(stderr, "Error initilizing the monitor.\n");
        exit(1);
    }
    if (pthread_cond_init(&q->cond, NULL)) {
        fprintf(stderr, "Error initilizing the condition variable.\n");
    }
    q->enqueueCount = 0;
    q->dequeueCount = 0;
    q->enqueueBlockCount = 0;
    q->dequeueBlockCount = 0;
    q->QUEUE_SIZE = size;
    q->array = malloc(sizeof(char*) * size);
    if (q->array == NULL) {
        fprintf(stderr, "Not enough memory for queue.\n");
        exit(1);
    }
    return q;
}
/* @brief Enqueues a string into queue
 */
void EnqueueString(Queue *q, char *string) {
    if (pthread_mutex_lock(&q->lock)) {  // lock critical section
        fprintf(stderr, "Error locking critical section.\n");
        exit(1);
    }
    while (q->enqueueCount - q->dequeueCount >= q->QUEUE_SIZE) {  // queue full
        q->enqueueBlockCount++;  // increment enqueueBlockCount
        if (pthread_cond_wait(&q->cond, &q->lock)) {  // block the thread
            fprintf(stderr, "Error blocking thread.\n");
            exit(1);
        }
    }
    q->array[q->enqueueCount++ % q->QUEUE_SIZE] = string;  // add string 
    if (pthread_cond_signal(&q->cond)) {  // wake up thread
        fprintf(stderr, "Error waking up thread.\n");
        exit(1);
    }
    if (pthread_mutex_unlock(&q->lock)) {  // unlock critical section
        fprintf(stderr, "Error unlocking critical section.\n");
        exit(1);
    }
}
/* @brief Dequeues a string from queue
 */
char *DequeueString(Queue *q) {
    if (pthread_mutex_lock(&q->lock)) {  // lock critical section
        fprintf(stderr, "Error locking critical section.\n");
        exit(1);
    }
    while (q->enqueueCount - q->dequeueCount == 0) {  // queue is empty
        q->dequeueBlockCount++;  // increment dequeueBlockCount
        if (pthread_cond_wait(&q->cond, &q->lock)) {  // block the thread
            fprintf(stderr, "Error blocking thread.\n");
            exit(1);
        }
    }
    char *ret = q->array[q->dequeueCount++ % q->QUEUE_SIZE];
    if (pthread_cond_signal(&q->cond)) {  // wake up thread
        fprintf(stderr, "Error waking up thread.\n");
        exit(1);
    }
    if (pthread_mutex_unlock(&q->lock)) {  // unlock critical section
        fprintf(stderr, "Error unlocking critical section.\n");
        exit(1);
    }
    return ret;
}

/* @brief Prints queue statistics.
 */
void PrintQueueStats(Queue *q) {
    fprintf(stderr, "enqueueCount: %i\n", --(q->enqueueCount));
    fprintf(stderr, "dequeueCount: %i\n", --(q->dequeueCount));
    fprintf(stderr, "enqueueBlockCount: %i\n", q->enqueueBlockCount);
    fprintf(stderr, "dequeueBlockCount: %i\n", q->dequeueBlockCount);

}
