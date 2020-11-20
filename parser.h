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

#include <ctype.h> 

#include "main.h"
#include "prec_parser.h"
#include "symtable.h"

void generateInstruction(int instType);
int parse(tListOfInstr *instrList);
int program();
int prolog();
int fun_def_list();
int fun_def();
int stat_list(varNode * treePtr);
int fun_params(varNode * treePtr);
int fun_returns();
int stat(varNode * treePtr);
int ass_exps(varNode * treePtr);
int ass_stat(varNode * treePtr);
int fun_call_param(varNode * treePtr);
int print_params(varNode * treePtr);

#endif /* PARSER_H*/


