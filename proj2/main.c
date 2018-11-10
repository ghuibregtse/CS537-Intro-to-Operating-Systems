////////////////////////////////////////////////////////////////////////////////
// Main File:       main.c
// 
// Names:           Garret Huibregtse Tyler Patton
// Emails:          ghuibregtse@wisc.edu tpatton4@wisc.edu
//
// Description:     This file holds the main function that will create queues
//                  for each thread and the threads themself. It waits to
//                  until all the threads it has created have terminated before
//                  it terminates itself.
////////////////////////////////////////////////////////////////////////////////

#include "queue.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "munch1.h"
#include "munch2.h"
#include "reader.h"
#include "writer.h"

const int QUEUE_SIZE = 10;  // size of queue
const int NUM_THREADS = 4;  // number of threads
const int NUM_QUEUES = 3;  // number of queues
const int READER = 0;  // tid of reader
const int MUNCH1 = 1;  // tid of munch1
const int MUNCH2 = 2;  // tid of munch2
const int WRITER = 3;  // tid of writer

int main() {
    pthread_t tid[NUM_THREADS];  // stores pointers to each thread
    Queue *q[NUM_QUEUES];  // array of queues for each thread
    for (int i = 0; i < NUM_QUEUES; i++)  // create the queue struct
        q[i] = CreateStringQueue(QUEUE_SIZE);
    Queues munchone = {q[READER], q[MUNCH1]};  // qin and qout for munch1
    Queues munchtwo = {q[MUNCH1], q[MUNCH2]};  // qin and qout for munch2


    // create threads for each module
    if (pthread_create(&tid[READER], NULL, Read, (void*) q[READER])) {  // reader
        fprintf(stderr, "ERROR CREATING THREAD.\n");
        exit(1);
    }
    if (pthread_create(&tid[MUNCH1], NULL, Munch1, (void*) &munchone)) {  // munch1
        fprintf(stderr, "ERROR CREATING THREAD.\n");
        exit(1);
    }

    if (pthread_create(&tid[MUNCH2], NULL, Munch2, (void*) &munchtwo)) {  // munch2
        fprintf(stderr, "ERROR CREATING THREAD.\n");
        exit(1);
    }

    if (pthread_create(&tid[WRITER], NULL, Write, (void*) q[MUNCH2])) {  // writer
        fprintf(stderr, "ERROR CREATING THREAD.\n");
        exit(1);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {  // check for all threads to finish
        if (pthread_join(tid[i], NULL)) {
            fprintf(stderr, "ERROR JOINING THREAD.\n");
            exit(1);
        }
    }
    fprintf(stderr, "Queue 1 usage statistics:\n");
    PrintQueueStats(q[READER]);
    fprintf(stderr, "Queue 2 usage statistics:\n");
    PrintQueueStats(q[MUNCH1]);
    fprintf(stderr, "Queue 3 usage statistics:\n");
    PrintQueueStats(q[MUNCH2]);
    for (int i = 0; i < NUM_QUEUES; i++) {
        free(q[i]->array);
        free(q[i]);
    }
    return 0;
}
