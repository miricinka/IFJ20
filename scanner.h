/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  scanner.h
 * 
 * DESCRIPTION:
 *  Lexical analysis
 * 
 * AUTHORS:
 *  Kolaříková Mirka    <xkolar76@stud.fit.vutbr.cz>
 *  Žovinec Martin      <xzovin00@stud.fit.vutbr.cz>
*/

#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h>

/*** State definitions ***/

#define ID          0

/* Key words */

#define KW_FUNC     1
#define KW_RETURN   2

#define KW_FLOAT64  3
#define KW_INT      4
#define KW_STRING   5

#define KW_FOR      6
#define KW_IF       7
#define KW_ELSE     8

/* Operators*/

#define ADD         10  //  +
#define SUB         11  //  -
#define DIV         12  //  /
#define MUL         13  //  *

#define EQ          20  //  ==
#define NEQ         21  //  !=
#define L           22  //  <
#define LEQ         23  //  >
#define G           24  //  <=
#define GEQ         25  //  >=

/* Types */

#define INT         30
#define FLOAT       31
#define STRING      32 

/* Special Characters */

#define L_BRACKET   41  //  (
#define R_BRACKET   42  //  )
#define ASSIGN      43  //  =
#define COMMA       44  //  ,

#define EOL         50
#define ENDFILE     51

/* Non-finate states */

#define START       60
#define S_EQ        61
#define S_DIV       62
#define S_L         63
#define S_G         64
#define S_NEG       65
#define S_ID        66
#define S_KW        67
#define S_INT       68
#define S_FLOAT     69
#define S_STRING    70

int get_next_char();