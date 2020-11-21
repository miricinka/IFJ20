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
#include "error.h"

// TODO returnTypes in functions
// TODO comments

/*************** Variable tree operations *****************/

/**
 * @brief Initializes the variable BST by setting its pointer to NULL.
 * 
 * @param RootPtr pointer to the variable BST
 */
void BSTInit (varNode *RootPtr) {
	*RootPtr = NULL;
}

/**
 * @brief Finds if given variable was declared.
 * 
 * @param RootPtr pointer to the variable BST
 * @param Key name of the searched variable
 * 
 * @return Bool value true if variable is in the BST, false otherwise
 */
bool isDeclared (varNode RootPtr, string Key) {
	if(BSTSearch(RootPtr, Key) == NULL)
		return false;
	return true;
}

/**
 * @brief Finds the variable and returns its type.
 * 
 * @param RootPtr pointer to the variable BST
 * @param Key name of the searched variable
 * 
 * @return int value of the variables type
 */
int getType (varNode RootPtr, string Key) {
	if(!RootPtr)
		return 666;

	else if ( strCmpString(&Key, &(RootPtr->name)) < 0)
		return getType (RootPtr->LPtr, Key);

	else if ( strCmpString(&Key, &(RootPtr->name)) > 0)
		return getType (RootPtr->RPtr, Key);

	return RootPtr->varStack->type;
}

/**
 * @brief Finds the variable in the BST.
 * 
 * @param RootPtr pointer to the variable binary BST
 * @param Key name of the searched variable
 * 
 * @return structure varNode of the searched variable
 */
varNode BSTSearch (varNode RootPtr, string Key)	{

	if(!RootPtr)
		return NULL;

	else if ( strCmpString(&Key, &(RootPtr->name)) < 0)
		return BSTSearch (RootPtr->LPtr, Key);

	else if ( strCmpString(&Key, &(RootPtr->name)) > 0)
		return BSTSearch (RootPtr->RPtr, Key);

	return RootPtr;
}

/**
 * @brief Adds a variable to the variable BST, prints error if the variable is already declared in the same scope.
 * 
 * @param RootPtr pointer to the variable BST
 * @param Key name of the new variable
 * @param Type type of the new variable
 * @param Scope current scope in the parser
 * 
 */
void BSTInsert (varNode *RootPtr, string Key, int Type, int scope)	{

	if( !*RootPtr ) {
		(*RootPtr) = (varNode)malloc(sizeof(struct varNode));
		if(RootPtr == NULL)
			return;

		(*RootPtr)->varStack = NULL;
		stackPush(RootPtr,Type,scope);
		strInit(&(*RootPtr)->name);
		
		strCopyString(&((*RootPtr)->name),&Key);

		(*RootPtr)->LPtr = (*RootPtr)->RPtr = NULL;
		return;
	}

	if ( strCmpString(&Key, &((*RootPtr)->name)) < 0) {
		BSTInsert ( &((*RootPtr)->LPtr), Key, Type, scope);
		return;
	}

	if ( strCmpString(&Key, &((*RootPtr)->name)) > 0) {
		BSTInsert ( &((*RootPtr)->RPtr), Key, Type, scope);
		return;
	}

	// Overwriting of the values, if the var exists
	if(scope > (*RootPtr)->varStack->scope)
		stackPush(RootPtr,Type,scope);
	else{
		fprintf(stderr,"ERROR 3: Redefinition of a variable in the same scope [%s]\n", Key.str);
		exit(3);
	}
}

/**
 * @brief Frees the whole variable BST
 * 
 * @param RootPtr pointer to the variable BST
 * 
 */
void BSTDispose (varNode *RootPtr) {

	if( *RootPtr != NULL){
        BSTDispose(&((*RootPtr)->LPtr));
        BSTDispose(&((*RootPtr)->RPtr));

		stackDelete(RootPtr);
		strFree(&((*RootPtr)->name));
        free(*RootPtr);
        *RootPtr = NULL;
    }
}

void ReplaceByRightmost (varNode PtrReplaced, varNode *RootPtr){

	if(!*RootPtr)
		return;

	if(( *RootPtr)->RPtr)
		ReplaceByRightmost(PtrReplaced, &((*RootPtr)->RPtr ));
	else{
		varNode delete_me = (*RootPtr);
		
		strClear(&(PtrReplaced->name));
		stackDelete(&PtrReplaced);

		strCopyString(&(PtrReplaced->name),&(delete_me->name));
		PtrReplaced->varStack = delete_me ->varStack;
		
		(*RootPtr) = (*RootPtr)->LPtr;
		
		strFree(&(delete_me->name));
		free(delete_me);
	}
}

void BSTDelete (varNode *RootPtr, string Key){
	if(!*RootPtr){
		return;
	}
	if (strCmpString(&Key, &((*RootPtr)->name)) < 0){
			BSTDelete( &((*RootPtr)->LPtr), Key);

	}else if (strCmpString(&Key, &((*RootPtr)->name)) > 0){
		BSTDelete( &((*RootPtr)->RPtr), Key);

	}else if ((*RootPtr)->LPtr && (*RootPtr)->RPtr)
		ReplaceByRightmost((*RootPtr), &((*RootPtr )->LPtr));

	else{
		varNode delete_me = (*RootPtr);

		if((*RootPtr)->LPtr)
			 *RootPtr = (*RootPtr)->LPtr;
		else
			 *RootPtr = (*RootPtr)->RPtr;
			 
		stackDelete(&delete_me);
		strFree(&(delete_me->name));
		free(delete_me);
	}
}

void BSTScopeDelete(varNode *RootPtr, int newScope){
	if(!*RootPtr)
		return;

	BSTScopeDelete(&((*RootPtr)->LPtr), newScope);
	BSTScopeDelete(&((*RootPtr)->RPtr), newScope);

	while ((*RootPtr)->varStack != NULL && (*RootPtr)->varStack->scope > newScope ){
		stackPop(RootPtr);
	}
	
	if ((*RootPtr)->varStack == NULL){
		BSTDelete(RootPtr, (*RootPtr)->name);
	}
}

/* Var tree stack */

void stackPop(varNode* s){
	if ((*s)->varStack != NULL){
		varStackElement temp = (*s)->varStack;
		(*s)->varStack = (*s)->varStack->previousElement;
		free(temp);
	}
}

void stackPush(varNode* s, int type, int scope){
	varStackElement newElement = (varStackElement) malloc(sizeof(struct varStackElement));
	newElement->type = type;
	newElement->scope = scope;

	newElement->previousElement = (*s)->varStack;

	(*s)->varStack = newElement;
}

void stackDelete(varNode *s){
	while ((*s)->varStack != NULL){
		stackPop(s);
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

void addFunToTree(funNode *RootPtr, string Key){
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
		
		strInit(&((*RootPtr))->name);
		strCopyString(&((*RootPtr)->name),&Key);

		(*RootPtr)->LPtr = (*RootPtr)->RPtr = NULL;
		return;
	}

	if (strCmpString(&Key, &((*RootPtr)->name)) < 0) {
		addFunToTree ( &((*RootPtr)->LPtr), Key);
		return;
	}else if (strCmpString(&Key, &((*RootPtr)->name)) > 0) {
		addFunToTree ( &((*RootPtr)->RPtr), Key);
		return;
	}
}

void funActualize (funNode *RootPtr, string Key, bool Declaration, bool Call, int paramCount, int returnCount){
	if(!*RootPtr){
		//printf("Function %s was not found!\n",Key.str);
		return;
	}

	if (strCmpString(&Key, &((*RootPtr)->name)) < 0) {
		funActualize( &((*RootPtr)->LPtr), Key, Declaration, Call, paramCount, returnCount);
		return;
	}else if (strCmpString(&Key, &((*RootPtr)->name)) > 0) {
		funActualize( &((*RootPtr)->RPtr), Key, Declaration, Call, paramCount, returnCount);
		return;
	}

	// function was found
	//printf("%d /\\ %d\n",(*RootPtr)->parameters->elementCount, paramCount);
	if((*RootPtr)->parameters->elementCount != paramCount){
		fprintf(stderr,"ERROR 6: Function has wrong amount of parameters [%s]\n", Key.str);
		exit(6);	// TODO check type
	}

	if((*RootPtr)->returnCodes->elementCount != returnCount){
		fprintf(stderr,"ERROR 6: Function has wrong amount of return types [%s]\n", Key.str);
		exit(3);	// TODO check type
	}

	if(Declaration == true && ((*RootPtr)->isDeclared == true)){
		fprintf(stderr,"ERROR 3: Redefinition of function [%s]\n", Key.str);
		exit(3);
    }

	if(Declaration == true && !((*RootPtr)->isDeclared == true)){
		(*RootPtr)->isDeclared = true;
	}

	if (Call == true && !((*RootPtr)->isCalled ==true)) {
		(*RootPtr)->isCalled = true;
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

void addFunCall(funNode *RootPtr, string Key, varNode varTree, int paramCount, int returnCount){
	if(BSTSearch (varTree, Key)){
		fprintf(stderr,"Error 3: Function is also a variable in the same scope! [%s]\n", Key.str);
		exit(3);
	}
	funActualize(RootPtr, Key, false, true, paramCount, returnCount);
}

void addFunDec(funNode *RootPtr, string Key, int paramCount, int returnCount){
	funActualize(RootPtr, Key, true, false, paramCount, returnCount);
}

void addParam(funNode *RootPtr, string Key, int parameterType, int parameterOrder){
	funNode *tempTree;
	tempTree = funSearch(RootPtr, Key);
	if((*tempTree)->isCalled == false && (*tempTree)->isDeclared == false ){
		funListAdd((*tempTree)->parameters, parameterType, parameterOrder);
	}else{
		checkListElement((*tempTree)->parameters,parameterType,parameterOrder);
	}
}

void addReturn(funNode *RootPtr, string Key, int returnType, int returnOrder){
	funNode *tempTree;
	tempTree = funSearch(RootPtr, Key);
	if((*tempTree)->isCalled == false && (*tempTree)->isDeclared == false ){
		funListAdd((*tempTree)->parameters, returnType, returnOrder);
	}else{
		checkListElement((*tempTree)->parameters, returnType, returnOrder);
	}
}

int isFunCallDec(funNode RootPtr){
	if(RootPtr != NULL){
		isFunCallDec(RootPtr->LPtr);
		isFunCallDec(RootPtr->RPtr);

		if (RootPtr->isCalled && !RootPtr->isDeclared ){
			fprintf(stderr,"Error - the function %s is called but not declared!\n", RootPtr->name.str);
			exit(3); 
		}
		return 0;
	}
	return 0;
}

int parCount(funNode RootPtr,string name){
	funNode *temp;
	temp = funSearch ( &RootPtr,  name);	
	return (*temp)->parameters->elementCount;
}

int retCount(funNode RootPtr, string name){
	funNode *temp;
	temp = funSearch ( &RootPtr,  name);	
	return (*temp)->returnCodes->elementCount;
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
}

void checkListElement(funList *list, int type, int order){
	funListElement tempListElement;
	tempListElement = funListSearch (list, order);
	
	if (tempListElement == NULL){
		//fprintf(stderr,"ERROR 6: Function has wrong amount of parameters/returns\n");
		//exit(6);	// TODO check type
		return; // delete me if exit above is uncommented
	}
	
	if (tempListElement->type != type){
		fprintf(stderr,"Error 6: Wrong return/parameter type of a function\n");	
		exit(6);
	}
}

/*************** Functions for printing datastructures *****************/

char* printType(int typeNum){
	if (typeNum == T_INT){
		return "int";
	}else if (typeNum == T_FLOAT){
		return "float";
	}else if (typeNum == T_STRING){
		return "string";
	}else{
		return "Unknown type";
	}
}

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
        printf("%s  +-[%s,%s,S%d]\n", sufix,  TempTree->name.str, printType(TempTree->varStack->type),TempTree->varStack->scope);
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
		
		printf("\t |%s, order %d|\n", printType(TempList.First->type),TempList.First->order);
		
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
			printf(",P %s",printType(tempElement->type));
		}
		for ( int i = 1; (tempElement = funListSearch(TempTree->returnCodes, i)) != NULL; i++){
			printf(",R %s",printType(tempElement->type));
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