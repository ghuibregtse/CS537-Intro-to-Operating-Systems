////////////////////////////////////////////////////////////////////////////////
// Main: 
// File:        queue.h
//
// Names:       Garret Huibregtse Tyler Patton
// Email:       ghuibregtse@wisc.edu tpatton4@wisc.edu
//
// Description: This is the header for the queue module. It defines the 
//              functions necessary to implement a queue and the queue data
//              structure itself.
////////////////////////////////////////////////////////////////////////////////


#ifndef __queue_h__
#define __queue_h__

#include <pthread.h>

typedef struct Queue {
    pthread_mutex_t lock;  // monitor lock
    pthread_cond_t cond;  // condition variable
    int enqueueCount;  // a count of the numbers of strings enqueued on queue.
    int dequeueCount;  // count of strings dequeued from queue.
    int enqueueBlockCount;  // count of number of times enqueue was blocked.
    int dequeueBlockCount;  // count of number of times dequeue was blocked.
    int QUEUE_SIZE;  // size of queue
    char **array;  // array of strings in queue
} Queue;

typedef struct Queues {
    Queue *qin;
    Queue *qout;

} Queues;

/* @brief   Dynamically allocate a new Queue structure and initialize it with an
 *          array of character points of length size. That means you'll malloc 
 *          the queue structure and then malloc the char * array pointed to from 
 *          that structure. Also remember to any state and synchronization 
 *          variables used in this structure.
 */
Queue *CreateStringQueue(int size);

/* @brief   This function places the pointer to the string at the end of queue.
 *          If queue is full, then this function blocks until there is space.
 */
void EnqueueString(Queue *q, char *string);

/* @brief   This function removes a pointer to a string from the beginning of
 *          queue q. If the queue is empty, then this function blocks until 
 *          there is a string placed into the queue. This function returns
 *          the pinter that was removed from the queue.
 */
char *DequeueString(Queue *q);

/* @brief   This function prints the statistics for this queue.
 */
void PrintQueueStats(Queue *q);

#endif // __queue_h__
