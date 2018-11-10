///////////////////////////////////////////////////////////////////////////////
// Main:        main.c
// File:        munch2.c
//
// Names:       Garret Huibregtse Tyler Patton
// Emails:      ghuibregtse@wisc.edu tpatton4@wisc.edu
// Description: This file will read through the strings and replace each lower
//              case character with the same letter but upper case.
///////////////////////////////////////////////////////////////////////////////

#include "munch2.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
void *Munch2(void *arg) {
    Queues *queues = (Queues*) arg;
    char *string;
    while ((string = DequeueString(queues->qin)) != NULL) {
        for (unsigned int i = 0; i < strlen(string); i++) {
            if (string[i] != '\0') {
                if (islower((unsigned char) string[i]))
                    string[i] = toupper((unsigned char) string[i]);
            }
        }
        EnqueueString(queues->qout, string);
    }
    EnqueueString(queues->qout, NULL);
    pthread_exit(NULL);
}
