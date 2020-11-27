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
 *  Based on scanner for IFJ19 by the same authors
*/
#ifndef SCANNER_H
#define SCANNER_H


#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h>

#include "str.h"
#include "main.h"
#include "error.h"

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
#define KW_PACKAGE  9

/* Operators*/

#define ADD         10  //  +
#define SUB         11  //  -
#define DIV         12  //  /
#define MUL         13  //  *

#define EQ          20  //  ==
#define NEQ         21  //  !=
#define LESS        22  //  <
#define GREAT       23  //  >
#define LEQ         24  //  <=
#define GEQ         25  //  >=

/* Types */

#define T_INT        30
#define T_FLOAT      31
#define T_STRING     32 


/* Special Characters */

#define L_PAR       41  //  (
#define R_PAR       42  //  )
#define L_BR        43  //  {
#define R_BR        44  //  }
#define ASSIGN      45  //  =
#define COMMA       46  //  ,
#define SEMICOL     47  //  ;
#define VAR_DEF     48  //  :=

#define EOL         50
#define ENDFILE     51


/* Non-finate states */

#define START       60
#define S_ASSIGN    61
#define S_DIV       62
#define S_L         63
#define S_G         64
#define S_NEG       65
#define S_ID        66
#define S_KW        67
#define S_INT       68
#define S_FLOAT     69
#define S_FLOAT2    70
#define S_FLOAT_E   71
#define S_STRING    72
#define L_COM       73
#define S_VAR_DEF   74
#define S_BLOCK     75
#define S_ESC       76
#define S_HEX       77

/* Build-in functions */

#define MAIN        80

#define F_INPUTS    81
#define F_INPUTI    82
#define F_INPUTF    83
#define F_PRINT     84
#define F_INT2FLOAT 85
#define F_FLOAT2INT 86
#define F_LEN       87
#define F_SUBSTR    88
#define F_ORD       89
#define F_CHR       90

int get_new_token(string *tokenStr);
int hex_to_int(char ch);

#endif /* SCANNER_H*/