////////////////////////////////////////////////////////////////////////////////
// Main:        main.c
// File:        writer.c
//
// Names:       Garret Huibregtse Tyler Patton
// Email:       ghuibregtse@wisc.edu tpatton4@wisc.edu
//
// Description  This file holds the function for the writer thread to execute.
//              It prints the modified string lines to stdout that are passed 
//              in via a queue.
////////////////////////////////////////////////////////////////////////////////

#include "writer.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

void *Write(void *arg) {
    Queue *qin = (Queue*) arg;
    char *string;
    int count = 0;
    while ((string = DequeueString(qin)) != NULL) {
        count++;
        printf("%s", string);
        free(string);
    }
    printf("\nNumber of strings processed: %i\n", count);
    free(string);
    
    pthread_exit(NULL);
}

