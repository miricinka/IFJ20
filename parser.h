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
*/

#ifndef PARSER_H
#define PARSER_H

#include <ctype.h> 

#include "main.h"
#include "prec_parser.h"
#include "symtable.h"
#include "generator.h"

int parse(tListOfInstr *instrList); //Function called in main that starts parsing
int program(); //First rule of LL grammar it will continuously dive deeper in LL grammar and than bubble back to main
int prolog(); //First line of program according to LL-grammar
int fun_def_list(); //Rule for list of function definitions
int fun_def(); //Rule for function definition
int stat_list(varNode * treePtr); //Rule for list of statements in function
int fun_params(varNode * treePtr); //Handling of parameters in definition of function
int fun_returns(); //handling of returns in definition of function
int stat(varNode * treePtr); //Rule for single statement
int ass_exps(varNode * treePtr, funList *assignVariablesList,int assignVarCounter, funList *assignAssignList,int assignAssignmentCounter, string readFunctionID, string nextID); //Right side of assign statement
int ass_stat(varNode * treePtr, funList *assignVariablesList, int assignVarCounter, string readFunctionID); //Left side of assign statement
int fun_call_param(varNode * treePtr); //Handling of paramters in calling of function
int print_params(varNode * treePtr); //Handling of print parameters
int return_values(varNode * treePtr); //Handling of return values

#endif /* PARSER_H*/


