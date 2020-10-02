/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  precedence_stack.h
 * 
 * DESCRIPTION:
 *  Header file for stack used in precedence analisis
 * 
 * AUTHORS:
 *  Kolaříková Mirka    <xkolar76@stud.fit.vutbr.cz>
*/

#ifndef PREC_STACK
#define PREC_STACK

#include <stdio.h>
#include "str.h"

/**
* @struct structure representing one token
*
* @param tokenNum token number
* @param tokenStr token content
*/
typedef struct{
	int tokenNum;
	string tokenStr;
}struc_token;

/**
* @struct structure representing stack
*
* @param maxsize max set size of stack
* @param top top index
*/
typedef struct{
	int maxsize;
	int top;
	struc_token *token;
}struc_prec_stack;

/**
 * @brief creates new stack of specific size, returns pointer 
 * 
 * @param size set max size
 * 
 * @return pointer to crated stack 
 */
struc_prec_stack* create_precStack(int size);

/**
 * @brief safely frees stack and its data
 * 
 * @param stackPtr pointer to stack
 */
void free_precStack(struc_prec_stack *stackPtr);

/**
 * @brief returns size of stack at the moment
 * 
 * @param stackPtr pointer to stack
 * 
 * @return size of stack at the moment
 */
int size_precStack(struc_prec_stack *stackPtr);

/**
 * @brief check if stack is empty 
 * 
 * @param stackPtr pointer to stack
 * 
 * @return 1 if empty
 *         0 if not empty
 */
int isEmpty_precStack(struc_prec_stack *stackPtr);

/**
 * @brief check if stack is full 
 * 
 * @param stackPtr pointer to stack
 * 
 * @return 1 if full
 *         0 if not full 
 */
int isFull_precStack(struc_prec_stack *stackPtr);

/**
 * @brief returns pointer to token at top
 * 
 * @param stackPtr pointer to stack
 * 
 * @return pointer to token at top
 *         Null if empty
 */
struc_token* peek_precStack(struc_prec_stack *stackPtr);

/**
 * @brief adds new token (tokenNum, tokenStr) to stack top 
 * 
 * @param stackPtr pointer to stack
 * @param num defined token number
 * @param string token content
 * 
 * @return 0 if succes
 *         1 if fail 
 */
int push_precStack(struc_prec_stack *stackPtr, int num, string content);

/**
 * @brief pops token at the stack top 
 * 
 * @param stackPtr pointer to stack
 */
void pop_precStack(struc_prec_stack *stackPtr);

/**
 * @brief prints stack at format BOTTOM [{,}] TOP
 * 
 * @param stackPtr pointer to stack
 */
void print_precStack(struc_prec_stack *stackPtr);


#endif