/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  prec_parser.h
 * 
 * DESCRIPTION:
 *  Head file for precedence parser
 * 
 * AUTHORS:
 *  Kolaříková Mirka    <xkolar76@stud.fit.vutbr.cz>
*/

#ifndef PREC_PARSER_H
#define PREC_PARSER_H

#include <stdio.h>
#include "str.h"
#include "scanner.h"
#include "precedence_stack.h"


//precedence table symbol 
#define NT_ADD         0
#define NT_SUB         1
#define NT_MUL         2
#define NT_DIV         3
#define NT_LESS        4
#define NT_LEQ         5
#define NT_GREAT       6
#define NT_GEQ         7
#define NT_EQ          8
#define NT_NEQ         9
#define NT_LPAR        10
#define NT_RPAR        11
#define NT_ID          12
#define NT_INT         13
#define NT_FLOAT       14
#define NT_STR         15
#define NT_DOLLAR      16


//terminal 
#define RULE_ID       20  //E -> id
#define RULE_INT      21  //E -> int
#define RULE_FLOAT    22  //E -> float
#define RULE_STR      23  //E -> str
#define RULE_PAR      24  //E -> (E)
#define RULE_ADD      25  //E -> E+E
#define RULE_SUB      26  //E -> E-E
#define RULE_MUL      27  //E -> E*E
#define RULE_DIV      28  //E -> E/E
#define RULE_LESS     29  //E -> E<E
#define RULE_GREAT    30  //E -> E>E
#define RULE_LEQ      31  //E -> E>=E
#define RULE_GEQ      32  //E -> E<=E
#define RULE_EQ       33  //E -> E==E
#define RULE_NEQ      34  //E -> E!=E

int prec_parse();
struc_token* get_NT(struc_prec_stack *stackPtr);
//int main();


#endif //PREC_PARSER_H
