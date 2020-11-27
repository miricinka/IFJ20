/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  error.c
 * 
 * DESCRIPTION:
 *  Error description
 * 
 * AUTHORS:
 *  Aleš Řezáč   <xrezac21@stud.fit.vutbr.cz>
*/

#include "error.h"

void errorMsg(int error_number, char* errorMessage){
    fprintf(stderr,"ERROR (%i): [%s]\n", error_number, errorMessage);
    exit(error_number);
}

