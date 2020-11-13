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

typedef struct varNode{
    int type;
	string name;

	struct varNode*LPtr;
	struct varNode*RPtr;
} *varNode;

typedef struct funNode{
	string name;

	bool isDeclared;
	bool isCalled;

	struct funListElement *params;
	struct funListElement *retrunCodes;

	struct funNode *LPtr;
	struct funNode *RPtr;
} *funNode;

typedef struct funListElement{
	int type;

	struct funList *NextPtr;
} *funListElement;

typedef struct {
    funListElement First;
	int elementCount;
} funList;

/* Prototypes of tree printing functions */

void Print_tree(varNodeTempTree);
void Print_tree2(varNodeTempTree, char* sufix, char fromdir);

/* Prototypes of variable operations */

void BSTInit   	(varNode*);
varNodeBSTSearch(varNode, string);
void BSTInsert 	(varNode, string, int);
void BSTDelete 	(varNode*, string);
void BSTDispose	(varNode*);
bool isDeclared (varNode, string);
int  getType	(varNode, string);

/* Prototypes of function operations */

void funBSTInit    (funNode *);
funNode funBSTSearch  (funNode, string);
void funBSTInsert  (funNode *, string, int);
void funBSTDelete  (funNode *, string);
void funBSTDispose (funNode *);
bool funIsDeclared (funNode, string);

/*Prototypes of function list operations*/
void funListInit (funList *L);
void funListAdd (funList *L, int val);
int* funListRead (funList *L);