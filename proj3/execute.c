////////////////////////////////////////////////////////////////////////////////
// Main:        main.c
// File:        execute.c
//
// Names:       Garret Huibregtse Tyler Patton
// Email:       ghuibregtse@wisc.edu tpatton4@wisc.edu
//
// Description: This is the source file for the process creation and execution
//              module.
//
////////////////////////////////////////////////////////////////////////////////

#include "execute.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int Execute(CommandList *command) {
    pid_t pid;
    int status = 0;

    if ((pid = fork()) < 0) {  // could not create child
        fprintf(stderr, "ERROR creating child process.\n");
        exit(1);
    } else if (pid == 0) {  // child process
        for (int i = 0; i < command->numCmds - 1; i++) {
            printf("%s ", command->command[i]);
        }
        printf("\n");
        if (execvp(command->command[0], command->command) < 0) {
            fprintf(stderr, "ERROR executing command.\n");
            exit(1);
        }
    } else {  // parent process
        while (wait(&status) != pid) {
        }
    }
    return status;
}
