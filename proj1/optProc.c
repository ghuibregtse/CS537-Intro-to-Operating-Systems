////////////////////////////////////////////////////////////////////////////////
// Main File: 537ps.c
// File: optProc.c
//
// Names:       Garret Huibregtse & Tyler Patton
// Emails:      ghuibregtse@wisc.edu & tpatton4@wisc.edu
//
// Description: This file processes the argument flags of our ps program.
//
////////////////////////////////////////////////////////////////////////////////

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include "optProc.h"
#include "procList.h"
#include <ctype.h>

#define p_FLAG      0
#define pid_FLAG    1
#define s_FLAG      2
#define U_FLAG      3
#define S_FLAG      4
#define v_FLAG      5
#define c_FLAG      6

/* @brief   This functionprints the usage of 537ps program.
 */
void printUsage()  {
    printf("Invalid argument\nUsage: ./537ps [-p <PID>] [-s] [-S] [-U] [-v] [-c]\n");
    exit(1);
}

/* @brief   This function determines which flags were set in the arguments
 * @return  Returns an array with all the flag information and process
 *          PID info if necessary.
 */
void optProc(int argc, char *argv[]) {
    char opt;  // ascii value of option
    int *flags = (int*) calloc(7, sizeof(int));  // list of containing status
    if (NULL == flags) {
        printf("Not enough memory so store flag states.\n");
        exit(1);
    }
    // these flags are defaulted to TRUE
    flags[U_FLAG] = 1; 
    flags[c_FLAG] = 1;
    while ((opt = getopt(argc, argv, ":s::U::S::v::c::p:")) != -1) {
        switch (opt) {  // determine which flag is present and sets status T/F
        case 'p' :
            for (unsigned int i = 0; i < strlen(optarg); i++) {
                if (!isdigit(optarg[i])) {
                    printf("Option -p requires a valid <PID>.\n");
                    exit(1);
                }
            }
            flags[p_FLAG] = 1;
            flags[pid_FLAG] = atoi(optarg);
            break;
        case 's' :
            if (optarg == NULL) {
                flags[s_FLAG] = 1;
            } else if (strcmp(optarg, "-") == 0) {
                flags[s_FLAG] = 0;
            } else {
                printUsage();
            }
            break;
        case 'U' :
            if (optarg == NULL || strcmp(optarg, "-") == 0) {
                flags[U_FLAG] = 0;
            } else {
                printUsage();
            }
            break;
        case 'S' :
            if (optarg == NULL) {
                flags[S_FLAG] = 1;
            } else if (strcmp(optarg, "-") == 0) {
                flags[S_FLAG] = 0;
            } else {
                printUsage();
            }
            break;        
        case 'v' :
            if (optarg == NULL) {
                flags[v_FLAG] = 1;
            } else if (strcmp(optarg, "-") == 0) {
                flags[v_FLAG] = 0;
            } else {
                printUsage();
            }
            break;
        case 'c' :
            if (optarg == NULL || strcmp(optarg, "-") == 0) {
                flags[c_FLAG] = 0;
            } else {
                printUsage();
            }
            break;
        case ':' :
            printf("Option -%c is missing a required <PID>.\n", optopt);
            exit(1);
            break;
        case '?' :
            printUsage();
            break;
        default  :
            printUsage();
            break;
        }
    }
    if (optind < argc) {
        printUsage();
        exit(1);
    }
    getProcList(flags);
    free(flags); 
}
