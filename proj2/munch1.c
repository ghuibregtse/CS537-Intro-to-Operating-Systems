///////////////////////////////////////////////////////////////////////////////
// Main:        main.c
// File:        munch1.c
//
// Names:       Garret Huibregtse Tyler Patton
// Emails:      ghuibregtse@wisc.edu tpatton4@wisc.edu
// Description: This file will read through the strings and replace each space
//              character with an *.
///////////////////////////////////////////////////////////////////////////////

#include "munch1.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

void *Munch1(void *arg) {
    Queues *queues = (Queues*) arg;
    char *string, *c; 
    while ((string = DequeueString(queues->qin)) != NULL) {
        while ((c = index(string, ' ')) != NULL) {
            c[0] = '*';
        }
        EnqueueString(queues->qout, string);
    }
    EnqueueString(queues->qout, NULL);
    pthread_exit(NULL);
}
