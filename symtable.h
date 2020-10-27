/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  symtable.h
 * 
 * DESCRIPTION:
 *  Header file for symtable.c
 * 
 * AUTHORS:
 *  Žovinec Martin      <xzovin00@stud.fit.vutbr.cz>
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "str.h"

#define KW_FUNC       1
#define T_INT        30
#define T_FLOAT      31
#define T_STRING     32 

/* Binary tree node structure */

typedef struct Node{
    int type;
	string name;
    string value;

	struct Node *LPtr;
	struct Node *RPtr;
} *Node;

/* function prototypes */

void BSTInit   (Node *);
Node BSTSearch (Node, string);
void BSTInsert (Node *, string, string, int);
void BSTDelete (Node *, string);
void BSTDispose(Node *);
bool isDeclared (Node, string);
int getType(Node, string);
