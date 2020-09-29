/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  main.h
 * 
 * DESCRIPTION:
 *  Main header file
 * 
 * AUTHORS:
 *  Tomáš Hrúz          <xhruzt00@stud.fit.vutbr.cz>
 *  Kolaříková Mirka    <xkolar76@stud.fit.vutbr.cz>
 *  Aleš Řezáč          <xrezac21@stud.fit.vutbr.cz>
 *  Žovinec Martin      <xzovin00@stud.fit.vutbr.cz>
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "scanner.h"
#include "str.h"

#define ERR_LEXICAL 1
#define ERR_SYNTAX 2
#define ERR_SEMANTIC_DEFINITION 3
#define ERR_SEMANTIC_DATATYPE 4
#define ERR_SEMANTIC_COMPATIBILITY 5
#define ERR_SEMANTIC_PARAM 6
#define ERR_SEMANTIC_OTHER 7
#define ERR_RUNTIME 9
#define ERR_INTERNAL 99
