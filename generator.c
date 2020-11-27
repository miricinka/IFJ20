/***************************
 * PROJECT:
 *  IFJ20 - Compiler for imperative programming language IFJ20
 *
 * UNIVERSITY:
 *  Faculty of Information Technology, Brno University of Technology
 *
 * FILE:
 *  generator.c
 *
 * DESCRIPTION:
 *  Main file
 *
 * AUTHORS:
 *  Aleš Řezáč          <xrezac21@stud.fit.vutbr.cz>
 *  Žovinec Martin      <xzovin00@stud.fit.vutbr.cz>
*/


#include "generator.h"

//TODO instruction list
//TODO Label stack
//TODO ANDS ORS NOTS
//výpis instruction listu
//linking globals
//TODO concatenate strings (inspire by DEFVAR {completed})
int labelcnt = 1;
int endif = 1;

tListOfInstr *list;

void genStackPop(labelStack* stack){
	if (stack != NULL){
		labelStack popMe = (*stack);
		(*stack) = (*stack)->previousElement;
		free(popMe);
	}
}

void genStackPush(labelStack* stack, int newLabel){
	labelStack newEStacklement = (labelStack) malloc(sizeof(struct labelStack));
	newEStacklement->labelCount = newLabel;

    if(*stack == NULL){
	    newEStacklement->previousElement = NULL;
    }else{
        newEStacklement->previousElement = (*stack);
    }
    
	(*stack) = newEStacklement;
}


void genStackDelete(labelStack* stack){
	while (*stack != NULL){
		genStackPop(stack);
	}
}

void genStackTest(labelStack* stack){

    printf("\n[TEST 01] Inicializing stack, pushing 1\n");
    *stack = NULL;
    genStackPush(stack,1);
    printf("Top stack is %d\n", (*stack)->labelCount);

    printf("\n[TEST 02] Popping stack, should be empty\n");
    genStackPop(stack);
    if(*stack == NULL){
        printf("empty stack\n");
    }else{
        printf("NOT empty stack\n");
    }

    printf("\n[TEST 03] Pushing 1 again, should NOT be empty\n");
    genStackPush(stack,1);
    printf("Top stack is %d\n", (*stack)->labelCount);
    if(*stack == NULL){
        printf("empty stack\n");
    }else{
        printf("NOT empty stack\n");
    }

    printf("\n[TEST 04] Pushing 2,3,4 and popping once, top should be 3.\n");
    genStackPush(stack,2);
    genStackPush(stack,3);
    genStackPush(stack,4);

    genStackPop(stack);
    printf("Top stack is %d\n", (*stack)->labelCount);


    printf("\n[TEST 05] Deleting the stack, should be empty.\n");
    genStackDelete(stack);
    if(*stack == NULL){
        printf("empty stack\n");
    }else{
        printf("NOT empty stack\n");
    }
}

/* Save instruction into instruction list */
void generateInstruction(char* instType, char* addr1, char* addr2, char* addr3){
   tInstr I;
   I.instType = instType;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   listInsertLast(list, I);
}

char* stringconvertor(char* string_before){
    string string_after; strInit(&string_after); strClear(&string_after);
    //printf("%s\n", string_before);

    size_t i = 0;
    while (string_before[i] != '\0') {    //loop trough chars
        //printf("%c\n", string_before[i]);  
        if ((string_before[i] >= 0 && string_before[i] <= 32)
           || string_before[i] == 35 || string_before[i] == 92){
            strAddChar(&string_after, '\\');
            strAddChar(&string_after, '0');

            int secund_num = string_before[i]%10; 
            int first_num = (string_before[i] - secund_num)/10;
            strAddChar(&string_after,  first_num+'0');
            strAddChar(&string_after,  secund_num+'0');

        }else{
            strAddChar(&string_after, string_before[i]);

        }

        i++;
    }
    return string_after.str;
}



/*  
    .IFJcode20
    generating global variables for concatenation
    let's go to main function
*/
void genFileHead(){
    generateInstruction(".IFJcode20\n", NULL, NULL, NULL);
    generateInstruction("DEFVAR", "GF@concat1", NULL, NULL);
    generateInstruction("DEFVAR", "GF@concat2", NULL, NULL);
    generateInstruction("DEFVAR", "GF@concatfin\n", NULL, NULL);
    generateInstruction("JUMP", "_main\n", NULL, NULL);  
}

/* start of main function */
void genMainFunc(){
    generateInstruction("LABEL", "_main", NULL, NULL);
    generateInstruction("CREATEFRAME", NULL, NULL, NULL);
}
/* end of main function */
void genMainEnd(){
    generateInstruction("CLEARS\n", NULL, NULL, NULL);
}

/* stack addition */
void genAdds(){
    generateInstruction("ADDS", NULL, NULL, NULL);
}

/* stack subtraction */
void genSubs(){
    generateInstruction("SUBS", NULL, NULL, NULL);
}

/* stack multiplicating */
void genMuls(){
    generateInstruction("MULS", NULL, NULL, NULL);
}

/* decimal stack dividing */
void genDivs(){
    generateInstruction("DIVS", NULL, NULL, NULL);
}

/* integers stack dividing */
void genIDivs(){
    generateInstruction("IDIVS", NULL, NULL, NULL);
}

/* push to stack */
void genPushs(int type, char* content){
    char* ans = (char*) malloc(sizeof(char) * strlen(content));
    if(type == NT_INT){
        sprintf(ans,"int@%s", content);
    }
    else if(type == NT_STR){
        content = stringconvertor(content);
        sprintf(ans,"string@%s", content);
    }
    else if(type == NT_FLOAT){
        sprintf(ans,"float@%s", content);
    }
    else if(type == NT_ID){
        sprintf(ans,"TF@%s", content);
    }
    else{}
    generateInstruction("PUSHS", ans, NULL, NULL);
}

/* variable definition */
void genDefvar(char* variable){
    char* ans = (char*) malloc(sizeof(char) * strlen(variable));
    sprintf(ans, "TF@%s", variable);
    generateInstruction("DEFVAR", ans, NULL, NULL);
}

/* pop from stack */
void genPops(char* variable){
    char* ans = (char*) malloc(sizeof(char) * strlen(variable));
    sprintf(ans, "TF@%s", variable);
    generateInstruction("POPS", ans, NULL, NULL);
}

/* clear stack */
void genClears(){
    generateInstruction("CLEARS", NULL, NULL, NULL);
}

/* detecting input */
void genRead(int type, char* variable){
    char* ans = (char*) malloc(sizeof(char) * strlen(variable));
    sprintf(ans, "TF@%s", variable);
    if(type == F_INPUTI){
        generateInstruction("READ", ans, "int", NULL);
    }
    else if(type == F_INPUTF){
        generateInstruction("READ", ans, "float", NULL);
    }
    else if(type == F_INPUTS){
        generateInstruction("READ", ans, "string", NULL);
    }
    else{}
}

/* conditions head */
void genIfElseHead(){
    char* labelname = (char*) malloc(sizeof(labelcnt));
    sprintf(labelname, "_label%d", labelcnt);
    labelcnt++;
    generateInstruction("LABEL", labelname, NULL, NULL);
}

/* end of for/if */
void genIfElseEnd(){
    char* finif = (char*) malloc(sizeof(endif));
    sprintf(finif, "_endif%d", endif);
    endif++;
    generateInstruction("JUMP", finif, NULL, NULL);
}

/* label to continue from if/for */
void genPostIf(){
    char* successor = (char*) malloc(sizeof(endif));
    sprintf(successor, "_endif%d", endif);
    endif++;
    generateInstruction("LABEL", successor, NULL, NULL);
}

/* print to output */
void genWrite(int Type, char* content){
    char* ans = (char*) malloc(sizeof(char) * strlen(content));
    if(Type == T_INT){
        sprintf(ans, "int@%s", content);
    }
    else if(Type == T_FLOAT){
        double number = strtod(content, NULL);
        sprintf(ans, "float@%a", number);
    }
    else if(Type == T_STRING){
        content = stringconvertor(content);
        sprintf(ans, "string@%s", content);
    }
    else if(Type == ID){
        sprintf(ans, "TF@%s", content);
    }
    else{}
    generateInstruction("WRITE", ans, NULL, NULL);
}

/* concatenate */
void genConcat(){
    generateInstruction("POPS", "GF@concat1", NULL, NULL);
    generateInstruction("POPS", "GF@concat2", NULL, NULL);
    generateInstruction("CONCAT", "GF@concatfin", "GF@concat1", "GF@concat2");
    generateInstruction("PUSHS", "GF@concatfin", NULL, NULL);
}

/* stack comparing > */
void genGTS(){
    generateInstruction("GTS", NULL, NULL, NULL);
}

/* stack comparing < */
void genLTS(){
    generateInstruction("LTS", NULL, NULL, NULL);
}

/* stack comparing == */
void genEQS(){
    generateInstruction("EQS", NULL, NULL, NULL);
}

/* stack compraing ! */
void genNOT(){
    generateInstruction("NOT", NULL, NULL, NULL);
}

/* call function */
void genCall(char* funcname){
    char* ans = (char*) malloc(sizeof(char) * strlen(funcname));
    sprintf(ans, "_%s", funcname);
    generateInstruction("CALL", ans, NULL, NULL);
    generateInstruction("POPFRAME", NULL, NULL, NULL);
}

/* start of function */
void genFuncHead(char* funcname){
    char* ans = (char*) malloc(sizeof(char) * strlen(funcname));
    sprintf(ans, "_%s", funcname);
    generateInstruction("LABEL", ans, NULL, NULL);
    generateInstruction("PUSHFRAME", NULL, NULL, NULL);
    generateInstruction("CREATEFRAME", NULL, NULL, NULL);
}

/* end of function, return to main */
void genFuncEnd(){
    generateInstruction("RETURN", NULL, NULL, NULL);
}

void genInt2Fl(){
    generateInstruction("INT2FLOATS", NULL, NULL, NULL);
}

void genFl2Int(){
    generateInstruction("FLOAT2INTS", NULL, NULL, NULL);
}

void genStrlen(char* number, int type, char* string){
    char* ans = (char*) malloc(sizeof(char) * strlen(number));
    sprintf(ans, "TF@%s", number);
    char* line = (char*) malloc(sizeof(char) * strlen(string));
    if(type == T_STRING){
        string = stringconvertor(string);
        sprintf(line, "string@%s", string);
    }
    else{
        sprintf(line, "TF@%s", string);
    }
    generateInstruction("STRLEN", ans, line, NULL);
}