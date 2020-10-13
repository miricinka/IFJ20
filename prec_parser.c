/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  prec_parser.c
 * 
 * DESCRIPTION:
 *  Precedence parser
 * 
 * AUTHORS:
 *  Kolaříková Mirka    <xkolar76@stud.fit.vutbr.cz>
*/

#include "prec_parser.h"

int table [17][17] = {
/*             +    -    *    /    <   <=    >   >=   ==   !=    (    )    ID  INT FLOAT STR   $     */
/* + */     { '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '<', '<', '>' },
/* - */     { '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '<', 'X', '>' },
/* * */     { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '<', 'X', '>' },
/* / */     { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '<', 'X', '>' },
/* < */     { '<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '>' },
/* <= */    { '<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '>' },
/* > */     { '<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '>' },
/* >= */    { '<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '>' },
/* == */    { '<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '>' },
/* != */    { '<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '>' },
/* ( */     { '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', '<', '<', '<', 'X' },
/* ) */     { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', '>' },
/* ID */    { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', '>' },
/* INT */   { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', '>' },
/* FLOAT */ { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', '>' },
/* STR */   { '>', 'X', 'X', 'X', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', '>' },
/* $ */     { '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'X', '<', '<', '<', '<', '#' }
};

/*    +    -    *    /    <   <=    >   >=   ==   !=    (    )    ID  INT FLOAT STR   $     */
/* Converts token to symbol used in precedence table */
int token_to_NT(int token_num){
	switch(token_num){
		case ADD:
			return NT_ADD;
		case SUB:
			return NT_SUB;
		case MUL:
			return NT_MUL;
		case DIV:
			return NT_DIV;
		case LESS:
			return NT_LESS;
		case LEQ:
			return NT_LEQ;
		case GREAT:
			return NT_GREAT;
		case GEQ:
			return NT_GEQ;
		case EQ:
			return NT_EQ;
		case NEQ:
			return NT_NEQ;
		case L_PAR:
			return NT_LPAR;
		case R_PAR:
			return NT_RPAR;
		case ID:
			return NT_ID;
		case T_INT:
			return NT_INT;
		case T_FLOAT:
			return NT_FLOAT;
		case T_STRING:
			return NT_STR;
		//konec vyrazu: , \n { ; 
		case COMMA:
		case SEMICOL:
		case EOL:
		case L_BR:
		case ENDFILE:
			return NT_DOLLAR;
		default:
			exit(ERR_SYNTAX); //CHECK
	}
}

//gets nonterminal from stack - basicly if top is terminal it looks under it
struc_token* get_NT(struc_prec_stack *stackPtr){
	struc_token *top = peek1_precStack(stackPtr);
	if(top->tokenNum >= RULE_ID){
		top = peek2_precStack(stackPtr);
	}
	return top;
}

//konec vyrazu: , \n { ; 
int prec_parse(){

	string testStr; strInit(&testStr); strClear(&testStr);
	string tokenStr; strInit(&tokenStr);
	int fullstack = 0;

	//int end_token = 0; //help

	//init stack
	struc_prec_stack *ptrStack = create_precStack(1000);
	//add $ to stack
	fullstack = push_precStack(ptrStack, NT_DOLLAR, testStr);
	if(fullstack){exit(99);}
	print_precStack(ptrStack);

	struc_token *top = peek1_precStack(ptrStack);


	//current token
	int new_token = get_new_token(&tokenStr); //get from Tom
	printf("NT je %d \n", token_to_NT(new_token));
	do{
		//top symbol - nonterminal
		top = get_NT(ptrStack);
		printf("TOP1: {%d,%s}\n",top->tokenNum, top->tokenStr.str);
		new_token = get_new_token(&tokenStr);

		printf("NT je %d \n", token_to_NT(new_token));


	}while(token_to_NT(new_token) != NT_DOLLAR);




	printf("konec precedence\n");
	return 51; //ok

}



