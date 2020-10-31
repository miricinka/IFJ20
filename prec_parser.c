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
			errorMsg(ERR_SYNTAX, "Incorrect expression token");
	}
	return 666; //program wont ever get here
}

/*converts NT to token or bool if bool*/
int end_datatype(struc_prec_stack *stackPtr){

	struc_token *top1 = peek1_precStack(stackPtr);
	switch(top1->tokenNum){
		case RULE_INT:
			return T_INT;
		case RULE_FLOAT:
			return T_FLOAT;
		case RULE_STR:
			return T_STRING;
		case TYPE_BOOL:
			return TYPE_BOOL;
		default:
		errorMsg(ERR_SYNTAX, "Problem on expression stack");
	}
	return 666; //program wont ever get here
}

/* pops 3 elements */
void pop3(struc_prec_stack *stackPtr){
	pop_precStack(stackPtr);
	pop_precStack(stackPtr);
	pop_precStack(stackPtr);
	return;
}

/* checks datatypes compability in arithmetic expressions */
void arithm_semantic_check(struc_prec_stack *stackPtr){
	string Str1; strInit(&Str1); strClear(&Str1);
	struc_token *top1 = peek1_precStack(stackPtr);
	struc_token *top3 = top3 = peek3_precStack(stackPtr);
		//semantic check int+int=int  str+str=str
	if(top1->tokenNum == RULE_INT && top3->tokenNum == RULE_INT){
		pop3(stackPtr);
		push_precStack(stackPtr, RULE_INT, Str1);
	//float+float=float
	}else if(top1->tokenNum == RULE_FLOAT && top3->tokenNum == RULE_FLOAT){
		pop3(stackPtr);
		push_precStack(stackPtr, RULE_FLOAT, Str1);
	//str+str=float	
	}else if(top1->tokenNum == RULE_STR && top3->tokenNum == RULE_STR){
		pop3(stackPtr);
		push_precStack(stackPtr, RULE_STR, Str1);
	}else{
		errorMsg(ERR_SEMANTIC_COMPATIBILITY, "Semantic error in expression - incompatible datatypes");
	}
	return;
}

/* gets nonterminal from stack - basicly if top is terminal it looks under it */
struc_token* get_NT(struc_prec_stack *stackPtr){
	struc_token *top = peek1_precStack(stackPtr);
	if(top->tokenNum >= RULE_ID){
		top = peek2_precStack(stackPtr);
	}
	return top;
}

/* reduces boolean expression on stack */
void reduce_boolean(struc_prec_stack *stackPtr){
	string Str1; strInit(&Str1); strClear(&Str1);
	struc_token *top1 = peek1_precStack(stackPtr);
	struc_token *top3 = top3 = peek3_precStack(stackPtr);

	if((top1->tokenNum == RULE_INT && top3->tokenNum == RULE_INT) ||
		(top1->tokenNum == RULE_FLOAT && top3->tokenNum == RULE_FLOAT) ||
		(top1->tokenNum == RULE_STR && top3->tokenNum == RULE_STR)){
		pop3(stackPtr);
		push_precStack(stackPtr, TYPE_BOOL, Str1);
	}else{
		errorMsg(ERR_SEMANTIC_COMPATIBILITY, "Semantic error in expression - incompatible datatypes");
	}
}

/* does rule E -> (E) */
void reduce_parenthesis(struc_prec_stack *stackPtr){
	string Str1; strInit(&Str1); strClear(&Str1);
	struc_token *top2 = peek2_precStack(stackPtr);
	int TMP_NT_NUM = top2->tokenNum;
	pop3(stackPtr);
	push_precStack(stackPtr, TMP_NT_NUM, Str1);

}

/* reduces expression on stack */
void reduce(struc_prec_stack *stackPtr, struc_token *topNT){

	//E -> id pushs id
	if(topNT->tokenNum == NT_ID){
		topNT->tokenNum = RULE_ID;
		printf("#####INTRUCTION PUSHS ID\n");
	//E -> int pushs int
	}else if(topNT->tokenNum == NT_INT){ 
		topNT->tokenNum = RULE_INT;
		printf("#####INTRUCTION PUSHS %s\n", topNT->tokenStr.str);
	//E -> float pushs float
	}else if(topNT->tokenNum == NT_FLOAT){
		topNT->tokenNum = RULE_FLOAT;
		printf("#####INTRUCTION PUSHS %s\n", topNT->tokenStr.str);
	//E -> str pushs str
	}else if(topNT->tokenNum == NT_STR){
		topNT->tokenNum = RULE_STR;
		printf("#####INTRUCTION PUSHS %s\n", topNT->tokenStr.str);
	//reduce 3 symbols
	}else if(topNT->tokenNum == NT_ADD){ //E -> E+E TODO str+str CONCAT
		printf("#####INTRUCTION ADDS\n");
		arithm_semantic_check(stackPtr);
	//E -> E+E
	}else if(topNT->tokenNum == NT_MUL){
		printf("#####INTRUCTION MULS\n");
		arithm_semantic_check(stackPtr);
	//E -> E-E
	}else if(topNT->tokenNum == NT_SUB){
		printf("#####INTRUCTION SUBS\n");
		arithm_semantic_check(stackPtr);
	//E -> E/E
	}else if(topNT->tokenNum == NT_DIV){
		struc_token *top1 = peek1_precStack(stackPtr);
		struc_token *top3 = top3 = peek3_precStack(stackPtr);

		//E -> E/0 division by zero constant error
		if(strcmp(top1->tokenStr.str,"0") == 0){
			errorMsg(ERR_RUNTIME, "division by zero constant");
		}

		if(top1->tokenNum == RULE_INT && top3->tokenNum == RULE_INT){
			printf("#####INTRUCTION DIVS\n");
			arithm_semantic_check(stackPtr);
			//print_precStack(stackPtr);

		}else if((top1->tokenNum == RULE_FLOAT && top3->tokenNum == RULE_FLOAT) || 
			(top1->tokenNum == RULE_INT && top3->tokenNum == RULE_FLOAT) ||
			(top1->tokenNum == RULE_FLOAT && top3->tokenNum == RULE_INT)){
			printf("#####INTRUCTION IDIVS\n");
			string Str1; strInit(&Str1); strClear(&Str1);
			pop3(stackPtr);
			push_precStack(stackPtr, RULE_FLOAT, Str1);

		}else{
			errorMsg(ERR_SEMANTIC_COMPATIBILITY, "Semantic error in expression - incompatible datatypes");
		}
	//E -> (E)
	}else if(topNT->tokenNum == NT_RPAR){
		reduce_parenthesis(stackPtr);

	//E -> E==E
	}else if(topNT->tokenNum == NT_EQ){
		printf("#####INTRUCTION EQS\n");
		print_precStack(stackPtr);
		reduce_boolean(stackPtr);
		print_precStack(stackPtr);
	//E -> E<E
	}else if(topNT->tokenNum == NT_LESS){
		printf("#####INTRUCTION LTS\n");
		reduce_boolean(stackPtr);
	//E -> E>E
	}else if(topNT->tokenNum == NT_GREAT){
		printf("#####INTRUCTION GTS\n");
		reduce_boolean(stackPtr);
	//E -> E!=E
	}else if(topNT->tokenNum == NT_NEQ){
		printf("#####INTRUCTION EQS\n");
		printf("#####INTRUCTION NOTS\n");
		reduce_boolean(stackPtr);
	//E -> E>=E
	}else if(topNT->tokenNum == NT_LEQ){
		printf("#####INTRUCTION LTS\n");
		printf("#####INTRUCTION NOT\n");
		reduce_boolean(stackPtr);
	//E -> E<=E
	}else if(topNT->tokenNum == NT_GEQ){
		printf("#####INTRUCTION GTS\n");
		printf("#####INTRUCTION NOT\n");
		reduce_boolean(stackPtr);

	}else{
		//print_precStack(stackPtr);
		errorMsg(ERR_SYNTAX, "Non existing rule for expression");
	}

	return;
}

/* main funtion
 checks syntax and semantics of expression
 returns final datatype and end token */
prec_end_struct prec_parse(int new_token, string tokenStr){

	string testStr; strInit(&testStr); strClear(&testStr);
	//string tokenStr; strInit(&tokenStr);
	int fullstack = 0;

	struc_prec_stack *ptrStack = create_precStack(1000);
	//add $ to stack
	fullstack = push_precStack(ptrStack, NT_DOLLAR, testStr);
	if(fullstack){exit(99);}

	struc_token *topNT = peek1_precStack(ptrStack);

	//current token
	//GET FROM TOM
	//int new_token = get_new_token(&tokenStr);

	bool prec_analisis_end = false;

	//checks first token
	if(token_to_NT(new_token) < 10 ||  token_to_NT(new_token) == NT_RPAR){
		errorMsg(ERR_SYNTAX, "Incorrect first expression token");
	}

	//main parse loop
	do{
		//top symbol - nonterminal
		topNT = get_NT(ptrStack);
	
		switch(table[topNT->tokenNum][token_to_NT(new_token)]){
			case '#':
				prec_analisis_end = true;
				break;
			case '=': 
			case '<':
				if(push_precStack(ptrStack, token_to_NT(new_token), tokenStr)){exit(99);}
				new_token = get_new_token(&tokenStr);
				break;
			case '>':
				reduce(ptrStack, topNT);
				break;
			default:
				errorMsg(ERR_SEMANTIC_COMPATIBILITY, "Semantic error in expression - incompatible datatypes");
		}

	}while(prec_analisis_end != true);

	//prepare structure to hold final datatype of expression and final token

	prec_end_struct end_struct;
	end_struct.end_token = new_token;
	end_struct.end_datatype = end_datatype(ptrStack);


	return end_struct; //send structure to parser

}


