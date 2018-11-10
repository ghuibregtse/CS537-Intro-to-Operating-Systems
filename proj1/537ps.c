////////////////////////////////////////////////////////////////////////////////
// Main File: 537ps.c
// 
// Names:       Garret Huibregtse & Tyler Patton
// Emails:      ghuibregtse@wisc.edu & tpatton4@wisc.edu
//
// Description: This file houses the main function for the 537ps program which
//              is our version of the linux ps command that displays the user's
//              processes and info about them. This main function calls the 
//              processing and parsing modules and stores the lists that the
//              processing and parsing modules generate.
////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include "optProc.h"
#include "procList.h"
#include "parser.h"

/* @brief This is the main function of the 537ps program.
 *
 */
int main(int argc, char *argv[]) {
    optProc(argc, argv);  // processes the command args, sets flags
    return 0;
}
