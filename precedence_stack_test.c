/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  precedence_stack_test.h
 * 
 * DESCRIPTION:
 *  Prececence stack test
 * 
 * AUTHORS:
 *  Kolaříková Mirka    <xkolar76@stud.fit.vutbr.cz>
*/

#include "precedence_stack.h"

int main(){
	printf("//----------------/ Create stack /------------------//\n");
	struc_prec_stack *ptr = create_precStack(10);
	string testStr;
	strInit(&testStr);
	struc_token *top;
	int full = 0;
	printf("\n");

	printf("1.//----------------/ Push 1, test1 /------------------//\n");
	strAddChars(&testStr, "test1");
	full = push_precStack(ptr, 1, testStr);
	if(full){
		printf("ERROR: stack full\n");
		exit(99);
	}
	print_precStack(ptr);
	strClear(&testStr);
	printf("\n");

	printf("2.//----------------/ Push 2, test2 /------------------//\n");
	strAddChars(&testStr, "test2");
	full = push_precStack(ptr, 2, testStr);
	if(full){
		printf("ERROR: stack full\n");
		exit(99);
	}
	print_precStack(ptr);
	strClear(&testStr);
	printf("\n");

	printf("3.//--------------/ Push 4890, test4890 /---------------//\n");
	strAddChars(&testStr, "test4890");
	full = push_precStack(ptr, 4890, testStr);
	if(full){
		printf("ERROR: stack full\n");
		exit(99);
	}
	print_precStack(ptr);
	strClear(&testStr);
	printf("\n");

	printf("4.//------------------/ Stack top1 /-------------------//\n");
	top = peek1_precStack(ptr);
	printf("TOP1: {%d,%s}\n",top->tokenNum, top->tokenStr.str);
	printf("\n");

	printf("4.//------------------/ Stack top2 /-------------------//\n");
	top = peek2_precStack(ptr);
	printf("TOP2: {%d,%s}\n",top->tokenNum, top->tokenStr.str);
	printf("\n");

	printf("4.//------------------/ Stack top3 /-------------------//\n");
	top = peek3_precStack(ptr);
	printf("TOP3: {%d,%s}\n",top->tokenNum, top->tokenStr.str);
	printf("\n");

	printf("5.//----------------------/ Pop /----------------------//\n");
	pop_precStack(ptr);
	print_precStack(ptr);
	printf("\n");

	printf("4.//------------------/ Stack top3 - error /-------------------//\n");
	top = peek3_precStack(ptr);
	if(top == NULL){
		printf("NULL - ERROR ok ");
	}else{
		printf("TOP3: {%d,%s}\n",top->tokenNum, top->tokenStr.str);
	}
	printf("\n");

	printf("6.//-------------------/ Stack top /--------------------//\n");
	top = peek1_precStack(ptr);
	printf("TOP: {%d,%s}\n",top->tokenNum, top->tokenStr.str);
	printf("\n");

	printf("7.//---------------------/ Pop /----------------------//\n");
	pop_precStack(ptr);
	print_precStack(ptr);
	printf("\n");

	printf("8.//-----------------/ Free stack /------------------//\n");
	free_precStack(ptr);
	printf("\n");
	return 0;

}