////////////////////////////////////////////////////////////////////////////////
// Main:        main.c
// File:        execute.h
//
// Names:       Garret Huibregtse Tyler Patton
// Email:       ghuibregtse@wisc.edu tpatton4@wisc.edu
//
// Description: This is the header for the process creation and execution
//              module.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef __execute_h__
#define __execute_h__
// This function forks a child process which then executes the command
// passed into it if able to.
#include "graph.h"

int Execute(CommandList *command);


#endif  // __execute_h__
