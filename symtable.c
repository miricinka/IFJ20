/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  symtable.c
 * 
 * DESCRIPTION:
 *  Implementation of symbol table using binary tree
 * 
 * AUTHORS:
 *  Žovinec Martin      <xzovin00@stud.fit.vutbr.cz>
*/

#include "symtable.h"

void BSTInit (Node *RootPtr) {
	*RootPtr = NULL;
}

bool isDeclared (Node RootPtr, string Key) {
	if(!RootPtr)
		return false;

	else if ( strCmpString(&Key, &(RootPtr->name)) < 0)
		return isDeclared (RootPtr->LPtr, Key);

	else if ( strCmpString(&Key, &(RootPtr->name)) > 0)
		return isDeclared (RootPtr->RPtr, Key);

	return true;
}

int getType (Node RootPtr, string Key) {
	if(!RootPtr)
		return 666;

	else if ( strCmpString(&Key, &(RootPtr->name)) < 0)
		return getType (RootPtr->LPtr, Key);

	else if ( strCmpString(&Key, &(RootPtr->name)) > 0)
		return getType (RootPtr->RPtr, Key);

	return RootPtr->type;
}

Node BSTSearch (Node RootPtr, string Key)	{

	if(!RootPtr)
		return NULL;

	else if ( strCmpString(&Key, &(RootPtr->name)) < 0)
		return BSTSearch (RootPtr->LPtr, Key);

	else if ( strCmpString(&Key, &(RootPtr->name)) > 0)
		return BSTSearch (RootPtr->RPtr, Key);

	return RootPtr;
}

void BSTInsert (Node *RootPtr, string Key, int Type)	{

	if( !*RootPtr ) {
		(*RootPtr) = (Node)malloc(sizeof(struct Node));
		if(RootPtr == NULL)
			return;


		// DON'T MOVE THIS LINE OR THE TREE DIES !!!
		/**/(*RootPtr)->type = Type;/**/
		// STAY WHERE YOU ARE FILTHY BUGGY CODE!

		strInit(&((*RootPtr)->name));
	
		strCopyString(&((*RootPtr)->name),&Key);

		(*RootPtr)->LPtr = (*RootPtr)->RPtr = NULL;
		return;
	}


	if ( strCmpString(&Key, &((*RootPtr)->name)) < 0) {
		BSTInsert ( &((*RootPtr)->LPtr), Key, Type);
		return;
	}

	if ( strCmpString(&Key, &((*RootPtr)->name)) > 0) {
		BSTInsert ( &((*RootPtr)->RPtr), Key, Type);
		return;
	}

		(*RootPtr)->type = Type;
		strCopyString(&((*RootPtr)->name), &Key);
}

void BSTDispose (Node *RootPtr) {

	if( *RootPtr != NULL){
        BSTDispose(&(( *RootPtr )->LPtr));
        BSTDispose(&(( *RootPtr )->RPtr));

		strFree(&((*RootPtr)->name));
		
        free( *RootPtr );
        *RootPtr = NULL;
    }
}

