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

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "str.h"

#define T_INT        30
#define T_FLOAT      31
#define T_STRING     32 

#define PointerError 	-1
#define AllGood 		0
#define BadType 		1
#define BadOrder 		2
#define BadTypeAndOrder 3

#define ParamList 1
#define ReturnList 2

#define MAX_LIST_LENGHT 100

/* Variable binary tree structure */

typedef struct varStackElement{
	int type;
	int scope;

	struct varStackElement *previousElement;
} *varStackElement;

typedef struct varNode{
	string name;
	struct varStackElement *varStack;

	struct varNode*LPtr;
	struct varNode*RPtr;
} *varNode;

/* Function binary tree structures */

typedef struct funNode{
	string name;

	bool isDeclared;
	bool isCalled;

	struct funList *parameters;
	struct funList *returnCodes;

	struct funNode *LPtr;
	struct funNode *RPtr;
} *funNode;

typedef struct funListElement{
	int type;
	int order;

	struct funListElement *NextPtr;
} *funListElement;

typedef struct funList{
    funListElement First;
	int elementCount;
} funList;

/* Prototypes of variable operations */

void BSTInit   	(varNode*);
varNode BSTSearch(varNode, string);
void BSTInsert 	(varNode*, string, int, int);
void BSTDispose	(varNode*);
bool isDeclared (varNode, string);
int  getType	(varNode, string);
void ReplaceByRightmost (varNode PtrReplaced, varNode *RootPtr);
void BSTDelete (varNode *RootPtr, string Key);
void BSTScopeDelete(varNode *RootPtr, int newScope);

/* Prototypes of variable stack operations */

void stackPop(varNode* varTree);
void stackPush(varNode* varTree, int type, int scope);
void stackDelete(varNode *varTree);

/* Prototypes of function operations */

void funInit (funNode *RootPtr);
funNode *funSearch (funNode *RootPtr, string Key);
void addFunToTree(funNode *RootPtr, string Key);
void funActualize (funNode *RootPtr, string Key, bool Declaration, bool Call, int paramCount, int returnCount);
void funDisposeTree (funNode *RootPtr);
void addFunCall(funNode *RootPtr, string Key, varNode varTree, int paramCount, int returnCount);
void addFunDec(funNode *RootPtr, string Key, int paramCount, int returnCount);
void addParam(funNode *RootPtr, string Key, int parameterType, int parameterOrder);
void addReturn(funNode *RootPtr, string Key, int returnType, int returnOrder);
int isFunCallDec(funNode RootPtr);
int parCount(funNode RootPtr,string name);
int retCount(funNode RootPtr,string name);

/*Prototypes of function list operations*/

void funListInit (funList *L);
void funListAdd (funList *L, int val, int order);
funListElement funListSearch (funList *L, int order);
void funListDelete(funList *L);
void checkListElement(funList *list, int type, int order);

/* Prototypes of datastructure printing functions */

char* printType(int typeNum);
void printVarTree(varNode);
void printVarTree2(varNode, char* sufix, char fromdir);
void printFunList(funList TL);
void printFunTree(funNode TempTree);
void printFunTree2(funNode TempTree, char* sufix, char fromdir);

#endif
