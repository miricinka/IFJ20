/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  Error.h
 * 
 * DESCRIPTION:
 *  Error description
 * 
 * AUTHORS:
 *  Rezac Ales    <xrezac21@stud.fit.vutbr.cz>
*/
#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

#define ERR_LEXICAL 1
#define ERR_SYNTAX 2
#define ERR_SEMANTIC_DEFINITION 3
#define ERR_SEMANTIC_DATATYPE 4
#define ERR_SEMANTIC_COMPATIBILITY 5
#define ERR_SEMANTIC_PARAM 6
#define ERR_SEMANTIC_OTHER 7
#define ERR_RUNTIME 9
#define ERR_INTERNAL 99

void errorMsg (int errnum, char* msg);

#endif /* ERROR_H */