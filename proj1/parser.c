////////////////////////////////////////////////////////////////////////////////
// Main:        537ps.c
// File:        parser.c
//
// Names:       Garret Huibregtse Tyler Patton
// Emails:      ghuibregtse@wisc.edu tpatton4@wisc.eu
//
// Description: This file contains the functions that parse the stat, statm,
//              and cmdline files of a process and print out the wanted info
//              based on the flags that were toggled in the cmdline.
////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include "parser.h"
#include<dirent.h>
#include<sys/stat.h>

static int const DIRECTORY_SIZE = 100;
static int const BUFFER_SIZE = 2048;

void closeFile(FILE *fp) {
    if (fclose(fp) != 0) {
        printf("This file could not be closed.\n");
        exit(1);
    }
}

/* @brief   This function opens a file, and reads its contents.
 * @return  a string with the contents of the file.
 */
char *openRead(char *dir, int pid) {
    FILE *fp;
    char currDir[DIRECTORY_SIZE], statStrBuf[BUFFER_SIZE], *statStr;
    sprintf(currDir, dir, pid);
    if ((fp = fopen(currDir, "r")) == NULL) {
        printf("File could not be opened.\n");
        exit(1);
    }
    if ((statStr = fgets(statStrBuf, BUFFER_SIZE, fp)) == NULL) {
        printf("Could not read file.\n");
        exit(1);
    }
    closeFile(fp);
    return statStr;
}

/* @brief   This function parses through the stat, statm, and cmdline files
 *          and prints out info about the processes running.
 * <pid>: <stat> utime=<user time> stime=<system time> vmemory=<virtual memory>
 *        [<command line>]
 */
void parse(int *flags) {
    char *state, *utime, *stime, *vmemory;

    // opens, reads, and closes stat file
    char *line = openRead("/proc/%i/stat", flags[1]);
    strtok(line, " ");
    printf("%i: ", flags[1]);  // <pid>
    for (int i = 0; i < 2; i++) {
        state = strtok(NULL, " ");
    }
    if (flags[2])
        printf("%s ", state);  // <state>
    for (int i = 0; i < 11; i++) {
        utime = strtok(NULL, " ");
    }
    if (flags[3])
        printf("utime=%s ", utime);  // <user time>
    stime = strtok(NULL, " ");
    if (flags[4])
        printf("stime=%s ", stime);  // <system time>
    line = openRead("/proc/%i/statm", flags[1]);
    vmemory = strtok(line, " ");
    if (flags[5])
        printf("vmemory=%s ", vmemory);
    FILE *fp;
    char currDir[DIRECTORY_SIZE];
    sprintf(currDir, "/proc/%i/cmdline", flags[1]);
    if ((fp = fopen(currDir, "r")) == NULL) {
        printf("File could not be opened.\n");
        exit(1);
    }
    if (flags[6]) {
        char c, a;
        printf("[");
        while ((c = fgetc(fp)) != EOF) {
            if (c == '\0') {
                if ((a = fgetc(fp)) != EOF) {
                    printf(" %c", a);
                }
            } else {               
                printf("%c", c);
            }
        }
        printf("]");
    } 
    printf("\n");
    closeFile(fp);
}
