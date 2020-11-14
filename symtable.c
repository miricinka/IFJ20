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
 * AUTHOR:
 *  Žovinec Martin      <xzovin00@stud.fit.vutbr.cz>
 */

#include "symtable.h"

void Print_tree2(varNode TempTree, char* sufix, char fromdir){
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

void Print_tree(varNode TempTree){
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

void BSTInit (varNode *RootPtr) {
	*RootPtr = NULL;
}

bool isDeclared (varNode RootPtr, string Key) {
	if(BSTSearch(RootPtr, Key) == NULL)
		return false;
	return true;
}

int getType (varNode RootPtr, string Key) {
	if(!RootPtr)
		return 666;

	else if ( strCmpString(&Key, &(RootPtr->name)) < 0)
		return getType (RootPtr->LPtr, Key);

	else if ( strCmpString(&Key, &(RootPtr->name)) > 0)
		return getType (RootPtr->RPtr, Key);

	return RootPtr->type;
}

varNode BSTSearch (varNode RootPtr, string Key)	{

	if(!RootPtr)
		return NULL;

	else if ( strCmpString(&Key, &(RootPtr->name)) < 0)
		return BSTSearch (RootPtr->LPtr, Key);

	else if ( strCmpString(&Key, &(RootPtr->name)) > 0)
		return BSTSearch (RootPtr->RPtr, Key);

	return RootPtr;
}

void BSTInsert (varNode RootPtr, string Key, int Type)	{

	if( !RootPtr ) {
		RootPtr = (varNode)malloc(sizeof(struct varNode));
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
}

void BSTDispose (varNode *RootPtr) {

	if( *RootPtr != NULL){
        BSTDispose((*RootPtr)->LPtr);
        BSTDispose((*RootPtr)->RPtr);

		strFree(&((*RootPtr)->name));
		
        free( *RootPtr );
        *RootPtr = NULL;
    }
}

/*************** Function tree operations *****************/

void funInit (funNode *RootPtr) {
	*RootPtr = NULL;
	funListInit ((*RootPtr)->parameters);
	funListInit ((*RootPtr)->returnCodes);
}

funNode funSearch (funNode RootPtr, string Key)	{

	if(!RootPtr)
		return NULL;

	else if (strCmpString(&Key, &(RootPtr->name)) < 0)
		return funSearch(RootPtr->LPtr, Key);

	else if (strCmpString(&Key, &(RootPtr->name)) > 0)
		return funSearch(RootPtr->RPtr, Key);

	return RootPtr;
}

funNode addFunToTree(funNode *RootPtr, string Key){

	// function will be inserted into the tree
	if(!*RootPtr){
		(*RootPtr) = (funNode)malloc(sizeof(struct funNode));
		if(RootPtr == NULL)
			return;

		strInit(&((*RootPtr)->name));
	
		strCopyString(&((*RootPtr)->name),&Key);

		(*RootPtr)->LPtr = (*RootPtr)->RPtr = NULL;
		return RootPtr;
	}

	if (strCmpString(&Key, &((*RootPtr)->name)) < 0) {
		addFunToTree ( &((*RootPtr)->LPtr), Key);
		return;
	}

	if (strCmpString(&Key, &((*RootPtr)->name)) > 0) {
		addFunToTree ( &((*RootPtr)->RPtr), Key);
		return;
	}

	// function was already called or declared
}

void funDisposeTree (funNode *RootPtr) {

	if( *RootPtr != NULL){
        funDisposeTree(&(( *RootPtr )->LPtr));
        funDisposeTree(&(( *RootPtr )->RPtr));

		strFree(&((*RootPtr)->name));
		funListDelete((*RootPtr)->parameters);
		funListDelete((*RootPtr)->returnCodes);

        free(*RootPtr);

        *RootPtr = NULL;
    }
}

void addFunCall(funNode RootPtr, string Key){
	// is it ok to reuse RootPtr?
	RootPtr = addFunToTree(RootPtr,Key);
	RootPtr->isCalled = true;
}

void addFunDec(funNode RootPtr, string Key){
	// is it ok to reuse RootPtr?
	RootPtr = addFunToTree(RootPtr,Key);
	RootPtr->isDeclared = true;
}

int addParam(funNode RootPtr, string Key, int parameterType, int parameterOrder){
	return processListElement(RootPtr, Key, parameterType, parameterOrder);
}

int addReturn(funNode RootPtr, string Key, int returnType, int returnOrder){
	return processListElement(RootPtr, Key, returnType, returnOrder);
}

string isFunCallDec(funNode RootPtr){

	if(RootPtr->LPtr)
		return isFunCallDec(RootPtr->LPtr);

	if(RootPtr->RPtr)
		return isFunCallDec(RootPtr->RPtr);

	if (RootPtr->isCalled && !RootPtr->isDeclared ){
		return RootPtr->name;
	}
}

/*************** Function list operations *****************/

void funListInit (funList *L) {
    L->First = NULL;
	L->elementCount = 0;
}

void funListAdd (funList *L, int val, int order){
	funListElement temp = L->First;

	if (L->First != NULL){
		while (temp->NextPtr != NULL){
			temp = temp->NextPtr;
		}
	}
	
	funListElement listElement = (funListElement) malloc(sizeof(struct funListElement) );
	if( listElement == NULL ){
		return;
	}

	listElement->type = val;	
	listElement->order = order;
	listElement->NextPtr = NULL;

	L->elementCount++;

	if (L->First == NULL){
		L->First = listElement;
	} else{
		temp->NextPtr = listElement;
	}
}

funListElement funListSearch (funList *L, int order){
	funListElement temp = L->First;

	// switch for and if statement
	for (int elementNum = 1; temp != NULL; elementNum++){
		temp = temp->NextPtr;
		if (elementNum = order){
			return temp;
		}
	}

	return NULL;
}

void funListDelete(funList *L){
	funListElement temp = L->First;
	while (temp != NULL){
		temp = temp->NextPtr;
		free(L->First);
		L->First = temp;
	}
}

int processListElement(funNode RootPtr, string Key, int type, int order){
	RootPtr = funSearch (RootPtr, Key);

	funListElement tempListElement = (funListElement) malloc(sizeof(struct funListElement) );
	if( tempListElement == NULL )
		return PointerError;

	tempListElement = funListSearch (RootPtr->parameters, order);

	// if element was not found, then we can simply insert it at the end of the list
	if(tempListElement == NULL){
		funListAdd(RootPtr->parameters,type,order);
		// is this free good?
		free(tempListElement);
		return AllGood;
	// if element was found, the we have to check if order and type match
	} else{
		return funListElementCheck (tempListElement, type, order);
	}
}

int funListElementCheck (funListElement ListElement, int Type, int Order){
	int returnMe = AllGood;

	if (ListElement->order != Order){
		
		if (ListElement->type != Type){
			returnMe = BadTypeAndOrder;
		} else{
			returnMe = BadOrder;
		}

	} else if (ListElement->type != Type){
		returnMe = BadType;
	} 

	free(ListElement);
	return returnMe;
}

/*
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
*/