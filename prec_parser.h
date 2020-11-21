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
#include "error.h"
#include "symtable.h"

/* structure to return datatype and final token*/
typedef struct{
	int end_token;
	int end_datatype;
}prec_end_struct;


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

#define TYPE_BOOL     40

/* main funtion
 checks syntax and semantics of expression
 returns final datatype and end token */
prec_end_struct prec_parse(varNode *treePtr, int new_token, string tokenStr);

/* Converts token to symbol used in precedence table */
int token_to_NT(int token_num);

/*converts NT to token or bool if bool*/
int end_datatype(struc_prec_stack *stackPtr);

/* pops 3 elements */
void pop3(struc_prec_stack *stackPtr);

/* checks datatypes compability in arithmetic expressions */
void arithm_semantic_check(struc_prec_stack *stackPtr);

/* gets nonterminal from stack */
struc_token* get_NT(struc_prec_stack *stackPtr);

/* reduces boolean expression on stack */
void reduce_boolean(struc_prec_stack *stackPtr);

/* does rule E -> (E) */
void reduce_parenthesis(struc_prec_stack *stackPtr);

/* reduces expression on stack */
void reduce(varNode *treePtr, struc_prec_stack *stackPtr, struc_token *topNT);


#endif //PREC_PARSER_H


