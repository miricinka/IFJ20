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

	struct Node *LPtr;
	struct Node *RPtr;
} *Node;

typedef struct funNode{
	string name;

	Node *params;
	Node *retrunCodes;

	struct funNode *LPtr;
	struct funNode *RPtr;
} *funNode;

/* Prototypes of tree printing functions */

void Print_tree(Node TempTree);
void Print_tree2(Node TempTree, char* sufix, char fromdir);

/* Prototypes of variable operations */

void BSTInit   	(Node *);
Node BSTSearch 	(Node, string);
void BSTInsert 	(Node *, string, int);
void BSTDelete 	(Node *, string);
void BSTDispose	(Node *);
bool isDeclared (Node, string);
int  getType	(Node, string);

/* Prototypes of function operations */

void funBSTInit    (funNode *);
funNode funBSTSearch  (funNode, string);
void funBSTInsert  (funNode *, string, int);
void funBSTDelete  (funNode *, string);
void funBSTDispose (funNode *);
bool funIsDeclared (funNode, string);
