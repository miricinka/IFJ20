/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  precedence_stack.c
 * 
 * DESCRIPTION:
 *  Stack for precedence analisis
 * 
 * AUTHORS:
 *  Kolaříková Mirka    <xkolar76@stud.fit.vutbr.cz>
*/

#include "precedence_stack.h"

/* creates new stack of specific size, returns pointer */
struc_prec_stack* create_precStack(int size){
	struc_prec_stack* stackPtr = (struc_prec_stack*) malloc(sizeof(struc_prec_stack));
	if(stackPtr == NULL){ //malloc error
		exit(99);}
	stackPtr->maxsize = size;
	stackPtr->top = -1; //empty stack 
	stackPtr->token = (struc_token*) malloc(sizeof(struc_token) * size);
	if(stackPtr->token == NULL){ //malloc error
		exit(99);}

	return stackPtr;
}

/* safely frees stack and its data */
void free_precStack(struc_prec_stack *stackPtr){
	free(stackPtr->token);
	free(stackPtr);
}

/* returns actual size of stack */
int size_precStack(struc_prec_stack *stackPtr){
	return stackPtr->top+1;
}

/* check if stack is empty, return code: 1 = empty, 0 = not empty */
int isEmpty_precStack(struc_prec_stack *stackPtr){
	return size_precStack(stackPtr) == 0;
}

/* check if stack is full, return code: 1 = full, 0 = not full */
int isFull_precStack(struc_prec_stack *stackPtr){
	return size_precStack(stackPtr) == stackPtr->maxsize;
}

/* returns pointer to token at top, or NULL if empty*/
struc_token* peek_precStack(struc_prec_stack *stackPtr){
	if(!isEmpty_precStack(stackPtr)){
		return &stackPtr->token[stackPtr->top];
	}else{
		return NULL;
	}
}

/* adds new token (tokenNum, tokenStr) to stack top, return code: 1 = error, 0 = succes*/
int push_precStack(struc_prec_stack *stackPtr, int num, string content){
	if(isFull_precStack(stackPtr)){
		return 1; //is full
	}
	++stackPtr->top; //increment top
	stackPtr->token[stackPtr->top].tokenNum = num;
	strInit(&(stackPtr->token[stackPtr->top].tokenStr));
	strCopyString(&(stackPtr->token[stackPtr->top].tokenStr), &content);

	return 0; //succes

}

/* pops token at the stack top */
void pop_precStack(struc_prec_stack *stackPtr){
	if(!isEmpty_precStack(stackPtr)){
		strFree(&(stackPtr->token[stackPtr->top].tokenStr));
		--stackPtr->top;
	}
}

/* prints stack at format BOTTOM [{,}] TOP */
void print_precStack(struc_prec_stack *stackPtr){
	int max = stackPtr->top;
	printf("BOTTOM [");
	for(int i = 0;i<=max;i++){
		printf("{%d,%s}",stackPtr->token[i].tokenNum, stackPtr->token[i].tokenStr.str);
	}
	printf("] TOP\n");
}


