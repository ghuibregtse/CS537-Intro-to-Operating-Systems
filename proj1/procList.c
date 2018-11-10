////////////////////////////////////////////////////////////////////////////////
// Main:        537ps.c
// File:        procList.c
//
// Names:       Garret Huibregtse Tyler Patton
// Email:       ghuibregtse@wisc.edu tpatton4@wisc.edu
//
// Description: This file holds holds the functions that process /proc and finds
//              the processes that are either directed to be displayed using -p
//              or all the current process being run by the user.
//
////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include<unistd.h>
#include<ctype.h>
#include "procList.h"
#include "parser.h"

static int const DIRECTORY_SIZE = 20;  // size of directory buffer
static int const BUFFER_SIZE = 50;  // size of status file parser buffer

/* @brief   This function will interate through the /proc directory, locate
 *          the process(es) being run by the user, and return a list that
 *          contains the PID of the process(es).
 * @param   flags array holding the status of all flags, and pid of process
            if -p is used
 */
void getProcList(int *flags) {
    int myUid = getuid();  // Uid unique to user to compare with processes
    struct dirent *procFile;  // object containing cur directory and info
    DIR *myDirectory;  // directory open in /proc 
    char currDir[DIRECTORY_SIZE];  // to concatenate the <PID> with a directory
    char lineBuf[BUFFER_SIZE];  // buffer to get each line of status file
    char *procUid;  // stores individual words of a status file line
    const char *delim = "\t";  // delimiter for strtok_r()
    FILE *fp;  // status file opened
    if (flags[0] == 1) {  // case where -p <PID>
        sprintf(currDir, "/proc/%i", flags[1]);
        myDirectory = opendir(currDir);
    } else {
        myDirectory = opendir("/proc");
    }
    if (myDirectory) {
        if (flags[0] == 1) {  // case where -p <PID>
            parse(flags); 
        } else {  // multiple processes is possible
            while ((procFile = readdir(myDirectory)) != NULL) {  // itr thru dir
                if (isdigit(procFile->d_name[0])) {  // check if proc is PID
                    sprintf(currDir, "/proc/%s/status", procFile->d_name);
                    if ((fp = fopen(currDir, "r")) == NULL) {
                        printf("Error opening file: %s\n", currDir);
                        exit(1);
                    }  // get to Uid line of status file
                    while ((fgets(lineBuf, BUFFER_SIZE, fp)) != NULL) {
                        procUid = strtok(lineBuf, delim);
                        if (strcmp(procUid, "Uid:") == 0) {
                            break;
                        }
                    }
                    procUid = strtok(NULL, delim);  // get Uid of proc
                    if (atoi(procUid) == myUid) {
                        flags[1] = atoi(procFile->d_name);
                        parse(flags);  
                    }
                    if (fclose(fp) != 0) {
                        printf("This file cannot be closed.\n");
                        exit(1);
                    }
                }
            }
        }

        if (closedir(myDirectory) != 0) {  // closes the directory
            printf("The directory cannot be closed.\n");
            exit(1);
        }
    } else {
        printf("Invalid directory, could not be opened.\n");
        exit(1);
    }
}
