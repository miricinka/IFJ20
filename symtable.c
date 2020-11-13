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

void Print_tree2(Node TempTree, char* sufix, char fromdir){
    if (TempTree != NULL){
		char* suf2 = (char*) malloc(strlen(sufix) + 4);
		strcpy(suf2, sufix);

        if (fromdir == 'L'){
	   		suf2 = strcat(suf2, "  |");
	   		printf("%s\n", suf2);
		}else
	   		suf2 = strcat(suf2, "   ");

		Print_tree2(TempTree->RPtr, suf2, 'R');
        printf("%s  +-[%s,%d]\n", sufix,  TempTree->name.str, TempTree->type);
		strcpy(suf2, sufix);

        if (fromdir == 'R')
	   		suf2 = strcat(suf2, "  |");	
		else
	   		suf2 = strcat(suf2, "   ");
		Print_tree2(TempTree->LPtr, suf2, 'L');

		if (fromdir == 'R') printf("%s\n", suf2);
			free(suf2);
    }
}

void Print_tree(Node TempTree){
  	printf("Struktura binarniho stromu:\n");
  	printf("\n");

  	if (TempTree != NULL)
     	Print_tree2(TempTree, "", 'X');
  	else
     printf("strom je prazdny\n");

  	printf("\n");
  	printf("=================================================\n");
} 

/*************** Variable tree operations *****************/

void BSTInit (Node *RootPtr) {
	*RootPtr = NULL;
}

bool isDeclared (Node RootPtr, string Key) {
	if(BSTSearch(RootPtr, Key) == NULL)
		return false;
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

void BSTInsert (Node RootPtr, string Key, int Type)	{

	if( !RootPtr ) {
		RootPtr = (Node)malloc(sizeof(struct Node));
		if(RootPtr == NULL)
			return;

		// DON'T MOVE THIS LINE OR THE TREE DIES !!!
		/**/RootPtr->type = Type;/**/
		// STAY WHERE YOU ARE FILTHY BUGGY CODE!

		strInit(&RootPtr->name);
	
		strCopyString(&(RootPtr->name),&Key);

		RootPtr->LPtr = RootPtr->RPtr = NULL;
		return;
	}


	if ( strCmpString(&Key, &(RootPtr->name)) < 0) {
		BSTInsert ( &(RootPtr->LPtr), Key, Type);
		return;
	}

	if ( strCmpString(&Key, &(RootPtr->name)) > 0) {
		BSTInsert ( &(RootPtr->RPtr), Key, Type);
		return;
	}

		RootPtr->type = Type;
		strCopyString(&(RootPtr->name), &Key);
}

void BSTDispose (Node *RootPtr) {

	if( *RootPtr != NULL){
        BSTDispose((*RootPtr)->LPtr);
        BSTDispose((*RootPtr)->RPtr);

		strFree(&((*RootPtr)->name));
		
        free( *RootPtr );
        *RootPtr = NULL;
    }
}

/*************** Function tree operations *****************/

void funBSTInit (funNode *RootPtr) {
	*RootPtr = NULL;
}

bool funIsDeclared (funNode RootPtr, string Key) {
	if(funBSTSearch(RootPtr, Key) == NULL)
		return false;
	return true;
}

funNode funBSTSearch (funNode RootPtr, string Key)	{

	if(!RootPtr)
		return NULL;

	else if (strCmpString(&Key, &(RootPtr->name)) < 0)
		return funBSTSearch(RootPtr->LPtr, Key);

	else if (strCmpString(&Key, &(RootPtr->name)) > 0)
		return funBSTSearch(RootPtr->RPtr, Key);

	return RootPtr;
}

void funBSTInsert (funNode *RootPtr, string Key, int Type)	{

	if( !*RootPtr ) {
		(*RootPtr) = (funNode)malloc(sizeof(struct funNode));
		if(RootPtr == NULL)
			return;

		strInit(&((*RootPtr)->name));
	
		strCopyString(&((*RootPtr)->name),&Key);

		(*RootPtr)->LPtr = (*RootPtr)->RPtr = NULL;
		return;
	}


	if ( strCmpString(&Key, &((*RootPtr)->name)) < 0) {
		funBSTInsert ( &((*RootPtr)->LPtr), Key, Type);
		return;
	}

	if ( strCmpString(&Key, &((*RootPtr)->name)) > 0) {
		funBSTInsert ( &((*RootPtr)->RPtr), Key, Type);
		return;
	}

		strCopyString(&((*RootPtr)->name), &Key);
}

void funBSTDispose (funNode *RootPtr) {

	if( *RootPtr != NULL){
        funBSTDispose(&(( *RootPtr )->LPtr));
        funBSTDispose(&(( *RootPtr )->RPtr));

		strFree(&((*RootPtr)->name));
		
        free( *RootPtr );
        *RootPtr = NULL;
    }
}

void funListInit (funList *L) {
    L->First = NULL;
	L->elementCount = 0;
}

void funListAdd (funList *L, int val){
	funListElement temp = L->First;

	while (temp->NextPtr != NULL){
		temp = temp->NextPtr;
	}

	funListElement listElement = (funListElement) malloc(sizeof(struct funListElement) );
	if( listElement == NULL ){
		DLError();
		return;
	}

	temp->NextPtr = listElement;
	listElement->type = val;	
	listElement->NextPtr = NULL;
	L->elementCount++;

	if (L->First == NULL){
		L->First = listElement;
	}
}

int* funListRead (funList *L){
	funListElement temp = L->First;
	int *typeArray;
	typeArray = (int*) malloc(L->elementCount * sizeof(int));

	for (int i = 0; temp != NULL; i++){
		temp = temp->NextPtr;
		typeArray[i] = temp->type;
	}
	return typeArray;
}
