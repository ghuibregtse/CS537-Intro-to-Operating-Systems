////////////////////////////////////////////////////////////////////////////////
// Main:        main.c
// File:        reader.c
//
// Names:       Garret Huibregtse Tyler Patton
// Email:       ghuibregtse@wisc.edu tpatton4@wisc.edu
//
// Description  This file holds the function for the reader thread to execute.
//              
////////////////////////////////////////////////////////////////////////////////

#include "reader.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

const int BUFFER = 1024;

void FlushBuf() {
    char temp;
    while ((temp = fgetc(stdin)) != '\n' && temp != EOF) {}
    fprintf(stderr, "Line was too long, buffer was flushed.\n");
    return;
}

void *Read(void *arg) {
    Queue *qout = (Queue*) arg;  // cast the arg as a queue
    char *string, ch;
    while (1) {
        string = (char *) calloc(BUFFER, sizeof(char));
        if (string == NULL) {
            fprintf(stderr, "Could not allocate space for string.\n");
            exit(1);
        }
        for (int i = 0; i < BUFFER; i++) {
            if ((ch = fgetc(stdin)) == EOF) {
                if (strlen(string) > 0) {
                    EnqueueString(qout, string);
                }
                EnqueueString(qout, NULL);
                pthread_exit(NULL);
            } else if (i < BUFFER)  {
                string[i] = ch;
                if (ch == '\n') {
                    EnqueueString(qout, string);
                    break;
                }
            }
            if (i + 1 == BUFFER) {
                FlushBuf();
                free(string);
                string = NULL;
                break;
            }
        }
    }
}
