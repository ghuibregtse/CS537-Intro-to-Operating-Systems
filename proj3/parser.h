////////////////////////////////////////////////////////////////////////////////
// Main:        main.c
// File:        parser.h
//
// Names:       Garret Huibregtse Tyler Patton
// Email:       ghuibregtse@wisc.edu tpatton4@wisc.edu
//
// Description: This is the header for the parser module. It defines the
//              functions necessary to parse the Makefile. 
////////////////////////////////////////////////////////////////////////////////


#ifndef __parser_h__
#define __parser_h__

#include "graph.h"
#include <stdio.h>
Node * Parse(FILE *fp);

#endif  // __parser_h__
