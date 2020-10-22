/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  Error.c
 * 
 * DESCRIPTION:
 *  Error description
 * 
 * AUTHORS:
 *  Rezac Ales    <xrezac21@stud.fit.vutbr.cz>
*/

#include "Error.h"

void errorMsg(int errnum, char* msg){
    fprintf(stderr,"Error (code: %i) (%s)\n", errnum, msg);
    exit(errnum);
}