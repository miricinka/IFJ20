/***************************
 * PROJECT:
 *  IFJ20 - Compiler for imperative programming language IFJ20
 *
 * UNIVERSITY:
 *  Faculty of Information Technology, Brno University of Technology
 *
 * FILE:
 *  parser.h
 *
 * DESCRIPTION:
 *  Parser header file
 *
 * AUTHORS:
 *  Tomáš Hrúz          <xhruzt00@stud.fit.vutbr.cz>
 *  Kolaříková Mirka    <xkolar76@stud.fit.vutbr.cz>
 *  Aleš Řezáč          <xrezac21@stud.fit.vutbr.cz>
 *  Žovinec Martin      <xzovin00@stud.fit.vutbr.cz>
*/

#ifndef PARSER_H
#define PARSER_H


#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h>

#include "main.h"

void generateInstruction(int instType);
int parse(tListOfInstr *instrList);
int program();
int prolog();
int fun_def_list();

#endif /* PARSER_H*/