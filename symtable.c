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

void BSTInsert (varNode *RootPtr, string Key, int Type)	{

	if( !*RootPtr ) {
		(*RootPtr) = (varNode)malloc(sizeof(struct varNode));
		if(RootPtr == NULL)
			return;

		// DON'T MOVE THIS LINE OR THE TREE DIES !!!
		/**/(*RootPtr)->type = Type;/**/
		// STAY WHERE YOU ARE FILTHY BUGGY CODE!

		strInit(&(*RootPtr)->name);
	
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
}

void BSTDispose (varNode *RootPtr) {

	if( *RootPtr != NULL){
        BSTDispose(&((*RootPtr)->LPtr));
        BSTDispose(&((*RootPtr)->RPtr));

		strFree(&((*RootPtr)->name));
        free( *RootPtr );
        *RootPtr = NULL;
    }
}

/*************** Function tree operations *****************/

void funInit (funNode *RootPtr) {
	*RootPtr = NULL;
	
}

funNode *funSearch (funNode *RootPtr, string Key)	{

	if(!*RootPtr)
		return NULL;

	else if (strCmpString(&Key, &((*RootPtr)->name)) < 0)
		return funSearch(&((*RootPtr)->LPtr), Key);

	else if (strCmpString(&Key, &((*RootPtr)->name)) > 0)
		return funSearch(&((*RootPtr)->RPtr), Key);

	return RootPtr;
}

void addFunToTree(funNode *RootPtr, string Key, bool Declaration, bool Call){


	// function will be inserted into the tree
	if(!*RootPtr){
		(*RootPtr) = (funNode)malloc(sizeof(struct funNode));
		if(RootPtr == NULL)
			return;

		(*RootPtr)->parameters = malloc(sizeof(struct funList));
		(*RootPtr)->parameters->First = NULL;
		(*RootPtr)->parameters->elementCount = 0;

		(*RootPtr)->returnCodes = malloc(sizeof(struct funList));
		(*RootPtr)->returnCodes->First = NULL;
		(*RootPtr)->returnCodes->elementCount = 0;
		//printf("%d\n",(*RootPtr)->parameters->elementCount);
		
		strInit(&((*RootPtr))->name);
		strCopyString(&((*RootPtr)->name),&Key);

		if( Declaration == true){
			(*RootPtr)->isDeclared = true;
			//printf("%s is set to Dec %d\n",(*RootPtr)->name.str, (*RootPtr)->isDeclared );
		}
		if (Call == true) {
			(*RootPtr)->isCalled = true;
			//printf("%s is set to Call %d\n",(*RootPtr)->name.str, (*RootPtr)->isCalled );
		}

		(*RootPtr)->LPtr = (*RootPtr)->RPtr = NULL;
		return;
	}

	if (strCmpString(&Key, &((*RootPtr)->name)) < 0) {
		addFunToTree ( &((*RootPtr)->LPtr), Key, Declaration, Call);
		return;
	}else if (strCmpString(&Key, &((*RootPtr)->name)) > 0) {
		addFunToTree ( &((*RootPtr)->RPtr), Key, Declaration, Call);
		return;
	}
	// function was already called or declared

	if(Declaration == true && ((*RootPtr)->isDeclared == true)){
        //printf("pokus o redeklaraci funkce %s\n",Key.str);
		fprintf(stderr,"ERROR 3: Redefinition of function [%s]\n", Key.str);		
    }

	if(Declaration == true && !((*RootPtr)->isDeclared == true)){
		(*RootPtr)->isDeclared = true;
		//printf("%s dec is actualized to %d\n",(*RootPtr)->name.str, (*RootPtr)->isCalled );
	}

	if (Call == true && !((*RootPtr)->isCalled ==true)) {
		(*RootPtr)->isCalled = true;
		//printf("%s call is actualized to %d\n",(*RootPtr)->name.str, (*RootPtr)->isCalled );
	}
}

void funDisposeTree (funNode *RootPtr) {

	if( *RootPtr != NULL){
        funDisposeTree(&((*RootPtr)->LPtr));
        funDisposeTree(&((*RootPtr)->RPtr));

		strFree(&((*RootPtr)->name));
		funListDelete((*RootPtr)->parameters);
		funListDelete((*RootPtr)->returnCodes);

        free(*RootPtr);

        *RootPtr = NULL;
    }
}


void addFunCall(funNode *RootPtr, string Key,varNode varTree){
	if(BSTSearch (varTree, Key)){
		fprintf(stderr,"Error - %s is also a variable in the same scope!\n", Key.str);
		exit(3);
	}
	addFunToTree(RootPtr, Key, false, true);
}

void addFunDec(funNode *RootPtr, string Key){
	addFunToTree(RootPtr, Key, true, false);
}

funList *findList(funNode *RootPtr, string Key, bool findParameterList){
	funNode *tempTree;
	tempTree = funSearch(RootPtr, Key);
	if(findParameterList)
		return (*tempTree)->parameters;
	else
		return (*tempTree)->returnCodes;
}

int addParam(funNode *RootPtr, string Key, int parameterType, int parameterOrder){
	funList* tempList;
	tempList = findList(RootPtr, Key, true);
	return processListElement(tempList, parameterType, parameterOrder);
}

int addReturn(funNode *RootPtr, string Key, int returnType, int returnOrder){
	funList* tempList;
	tempList = findList(RootPtr, Key, false);
	return processListElement(tempList, returnType, returnOrder);
}

int isFunCallDec(funNode RootPtr){

	if(RootPtr->LPtr)
		isFunCallDec(RootPtr->LPtr);

	if(RootPtr->RPtr)
		isFunCallDec(RootPtr->RPtr);

	if (RootPtr->isCalled && !RootPtr->isDeclared ){
		fprintf(stderr,"Error - the function %s is called but not declared!\n", RootPtr->name.str);
		return 1;
		//exit(3); 
	}
	return 0;
}

/*************** Function list operations *****************/

void funListInit (funList *L) {
	L = malloc(sizeof(struct funList));
	
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
	
	if( L->First == NULL){
		return NULL;
	}
	
	funListElement temp = L->First;

	for (int elementNum = 1; temp != NULL; elementNum++){
		
		if (elementNum == order){
			return temp;
		}
		temp = temp->NextPtr;
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
	// TODO free L segfaults
}

int processListElement(funList *list, int type, int order){
	funListElement tempListElement;

	tempListElement = funListSearch (list, order);

	// if element was not found, then we can simply insert it at the end of the list
	if(tempListElement == NULL){
		funListAdd(list,type,order);
		
	// if element was found, the we have to check if types match
	} else if (tempListElement->type != type){
		return BadType;
	}
	return AllGood; 
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

/*************** Functions for printing datastructures *****************/

void printVarTree2(varNode TempTree, char* sufix, char fromdir){
    if (TempTree != NULL){
		char* suf2 = (char*) malloc(strlen(sufix) + 4);
		strcpy(suf2, sufix);

        if (fromdir == 'L'){
	   		suf2 = strcat(suf2, "  |");
	   		printf("%s\n", suf2);
		}else
	   		suf2 = strcat(suf2, "   ");

		printVarTree2(TempTree->RPtr, suf2, 'R');
        printf("%s  +-[%s,%d]\n", sufix,  TempTree->name.str, TempTree->type);
		strcpy(suf2, sufix);

        if (fromdir == 'R')
	   		suf2 = strcat(suf2, "  |");	
		else
	   		suf2 = strcat(suf2, "   ");
		printVarTree2(TempTree->LPtr, suf2, 'L');

		if (fromdir == 'R') printf("%s\n", suf2);
			free(suf2);
    }
}

void printVarTree(varNode TempTree){
  	printf("Struktura binarniho stromu:\n");
  	printf("\n");

  	if (TempTree != NULL)
     	printVarTree2(TempTree, "", 'X');
  	else
     printf("strom je prazdny\n");

  	printf("\n");
  	printf("=================================================\n");
} 

void printFunList(funList TL){
	
	funList TempList=TL;
	int CurrListLength = 0;
	printf("\t -----------------\n");
	if(TempList.First == NULL ){
		printf("\t  list je prazdny\n");
	}
	while ((TempList.First!=NULL) && (CurrListLength<MAX_LIST_LENGHT))	{
		
		printf("\t |type %d, order %d|\n",TempList.First->type,TempList.First->order);
		
		TempList.First=TempList.First->NextPtr;
		CurrListLength++;
	}
    if (CurrListLength>=MAX_LIST_LENGHT){
        printf("List exceeded maximum length!\n");
	}
	printf("\t -----------------\n");     
}

void printFunTree(funNode TempTree){
  	printf("Struktura binarniho stromu:\n");
  	printf("\n");

  	if (TempTree != NULL)
     	printFunTree2(TempTree, "", 'X');
  	else
     printf("strom je prazdny\n");

  	printf("\n");
  	printf("=================================================\n");
} 

void printFunTree2(funNode TempTree, char* sufix, char fromdir){
	funListElement tempElement;
    if (TempTree != NULL){
		char* suf2 = (char*) malloc(strlen(sufix) + 4);
		strcpy(suf2, sufix);

        if (fromdir == 'L'){
	   		suf2 = strcat(suf2, "  |");
	   		printf("%s\n", suf2);
		}else
	   		suf2 = strcat(suf2, "   ");

		printFunTree2(TempTree->RPtr, suf2, 'R');
        printf("%s  +-[%s,D%d,C%d", sufix,  TempTree->name.str,TempTree->isDeclared,TempTree->isCalled);
		for ( int i = 1; (tempElement = funListSearch(TempTree->parameters, i)) != NULL; i++){
			printf(",PT%d",tempElement->type);
		}
		for ( int i = 1; (tempElement = funListSearch(TempTree->returnCodes, i)) != NULL; i++){
			printf(",RT%d",tempElement->type);
		}
		printf("]\n");

		strcpy(suf2, sufix);

        if (fromdir == 'R')
	   		suf2 = strcat(suf2, "  |");	
		else
	   		suf2 = strcat(suf2, "   ");
		printFunTree2(TempTree->LPtr, suf2, 'L');

		if (fromdir == 'R') printf("%s\n", suf2);
			free(suf2);
    }
}