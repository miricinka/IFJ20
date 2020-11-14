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
 * AUTHOR:
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

#define PointerError 	-1
#define AllGood 		0
#define BadType 		1
#define BadOrder 		2
#define BadTypeAndOrder 3

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

	struct funListElement *parameters;
	struct funListElement *returnCodes;

	struct funNode *LPtr;
	struct funNode *RPtr;
} *funNode;

typedef struct funListElement{
	int type;
	int order;

	struct funList *NextPtr;
} *funListElement;

typedef struct {
    funListElement First;
	int elementCount;
} funList;

/* Prototypes of tree printing functions */

void Print_tree(varNode);
void Print_tree2(varNode, char* sufix, char fromdir);

/* Prototypes of variable operations */

void BSTInit   	(varNode*);
varNode BSTSearch(varNode, string);
void BSTInsert 	(varNode, string, int);
void BSTDispose	(varNode*);
bool isDeclared (varNode, string);
int  getType	(varNode, string);

/* Prototypes of function operations */

void funInit    (funNode *);
funNode funSearch  (funNode, string);
funNode addFunToTree  (funNode *, string);
void funDisposeTree (funNode *);
void addFunCall(funNode , string);
void addFunDec(funNode, string);
int addParam(funNode, string, int, int);
int addReturn(funNode, string, int, int);
string isFunCallDec(funNode);

/*Prototypes of function list operations*/

void funListInit (funList );
void funListAdd (funList , int);
funListElement funListSearch (funList, int);
void funListDelete (funList);
int processListElement(funNode *, string, int, int);
int funListElementCheck (funListElement, int, int);
