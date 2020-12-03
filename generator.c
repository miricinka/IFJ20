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
*/

#include "generator.h"

//TODO instruction list
//TODO Label stack
//TODO ANDS ORS NOTS
//linking globals
//TODO concatenate strings (inspire by DEFVAR {completed})

int endif = 1;

static int labelCnt = 1;
static int inputCnt = 0;

tListOfInstr *list;
labelStack* elses = NULL;
labelStack* postifs = NULL;


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

char* makeLabel(int labelCount, char *labelType){
    /*len of labelType + int can be max 10 characters (1 character = 1 byte)*/
    char* resultLabel = (char*) malloc(strlen(labelType) + 10);

    sprintf(resultLabel,"%s%d", labelType, labelCount);
    return resultLabel;
}
/*  
    .IFJcode20
    generating global variables for concatenation
    generating global variables for input functions
    let's go to main function
*/
void genFileHead(){
    generateInstruction(".IFJcode20\n", NULL, NULL, NULL);
    generateInstruction("DEFVAR", "GF@concat1", NULL, NULL);
    generateInstruction("DEFVAR", "GF@concat2", NULL, NULL);
    generateInstruction("DEFVAR", "GF@concatfin", NULL, NULL);
    /* comparing in If-Else / for */
    generateInstruction("DEFVAR", "GF@compare", NULL, NULL);
    /* Checking correct type of a variable in inputs, inputi, inputf functions */
    generateInstruction("DEFVAR", "GF@getType", NULL, NULL);
    generateInstruction("DEFVAR", "GF@typeString", NULL, NULL);
    generateInstruction("DEFVAR", "GF@typeFloat", NULL, NULL);
    generateInstruction("DEFVAR", "GF@typeInt", NULL, NULL);
    generateInstruction("MOVE", "GF@typeInt", "string@int", NULL);
    generateInstruction("MOVE", "GF@typeFloat", "string@float", NULL);
    generateInstruction("MOVE", "GF@typeString", "string@string", NULL);
    generateInstruction("\nJUMP", "_main", NULL, NULL);  
}

/* start of main function */
void genMainFunc(){
    generateInstruction("\nLABEL", "_main", NULL, NULL);
    generateInstruction("CREATEFRAME", NULL, NULL, NULL);
}
/* end of main function */
void genMainEnd(){
    generateInstruction("CLEARS", NULL, NULL, NULL);
    generateInstruction("EXIT", "int@0", NULL, NULL);
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
    char* ans = (char*) malloc(sizeof(char) * strlen(content) * 10);
    if(type == NT_INT){
        sprintf(ans,"int@%s", content);
    }
    else if(type == NT_STR){
        content = stringconvertor(content);
        sprintf(ans,"string@%s", content);
    }
    else if(type == NT_FLOAT){
        double number = strtod(content,NULL);
        sprintf(ans,"float@%a", number);
    }
    else if(type == NT_ID){
        sprintf(ans,"TF@%s", content);
    }
    else{}
    generateInstruction("PUSHS", ans, NULL, NULL);
}

/* variable definition */
void genDefvar(char* variable){
    char* ans = (char*) malloc(sizeof(char) * strlen(variable) + 10);
    sprintf(ans, "TF@%s", variable);
    generateInstruction("DEFVAR", ans, NULL, NULL);
}

/* pop from stack */
void genPops(char* variable){
    char* ans = (char*) malloc(sizeof(char) * strlen(variable) + 10);
    sprintf(ans, "TF@%s", variable);
    generateInstruction("POPS", ans, NULL, NULL);
}

/* clear stack */
void genClears(){
    generateInstruction("CLEARS", NULL, NULL, NULL);
}

/* detecting input */
void genRead(int type, char* variable,  char* variable2){
    char* ans = (char*) malloc(sizeof(char) * strlen(variable) + 10);
    sprintf(ans, "TF@%s", variable);

    char* ans2 = (char*) malloc(sizeof(char) * strlen(variable2) + 10);
    sprintf(ans2, "TF@%s", variable2);


    if(type == F_INPUTI){
        char *label = makeLabel(++inputCnt,"inputi");
        generateInstruction("READ", ans, "int", NULL);
        generateInstruction("TYPE", "GF@getType", ans, NULL);
        generateInstruction("MOVE", ans2, "int@0", NULL);

        generateInstruction("JUMPIFEQ", label, "GF@getType", "GF@typeInt");
        generateInstruction("MOVE", ans2, "int@1", NULL);
        generateInstruction("LABEL", label, NULL, NULL);

    }else if(type == F_INPUTF){
        char *label = makeLabel(++inputCnt,"inputf");
        generateInstruction("READ", ans, "float", NULL);
        generateInstruction("TYPE", "GF@getType", ans, NULL);
        generateInstruction("MOVE", ans2, "int@0", NULL);

        generateInstruction("JUMPIFEQ", label, "GF@getType", "GF@typeFloat");
        generateInstruction("MOVE", ans2, "int@1", NULL);
        generateInstruction("LABEL", label, NULL, NULL);

    }else if(type == F_INPUTS){
        char *label = makeLabel(++inputCnt,"inputs");
        generateInstruction("READ", ans, "string", NULL);
        generateInstruction("TYPE", "GF@getType", ans, NULL);
        generateInstruction("MOVE", ans2, "int@0", NULL);

        generateInstruction("JUMPIFEQ", label, "GF@getType", "GF@typeString");
        generateInstruction("MOVE", ans2, "int@1", NULL);
        generateInstruction("LABEL", label, NULL, NULL);
    }
}

/* conditions head */
void genIfHead(){
    genStackPush(elses, labelCnt);
    char *label = makeLabel((*elses)->labelCount,"elselabel_");
    labelCnt++;
    generateInstruction("POPS", "GF@compare", NULL, NULL);
    generateInstruction("JUMPIFNEQ", label, "bool@true", "GF@compare");
}

void genElseHead(){
    char *label = makeLabel((*elses)->labelCount,"elselabel_");
    generateInstruction("LABEL", label, NULL, NULL);
    genStackPop(elses);
}

/* end of for/if */
void genIfEnd(){
    genStackPush(postifs, endif);
    char *label = makeLabel((*postifs)->labelCount,"postif_");
    endif++;
    generateInstruction("JUMP", label, NULL, NULL);
}

/* label to continue from if/for */
void genPostIf(){
    char *label = makeLabel((*postifs)->labelCount,"postif_");
    generateInstruction("LABEL", successor, NULL, NULL);
    genStackPop(postifs);
}

void genForHead(){

    char* labelname = (char*) malloc(sizeof(labelCnt) + 10);
    genStackPush(elses, labelCnt);
    sprintf(labelname, "forlabel_%d", elses->labelcount);
    labelCnt++;
    generateInstruction("POPS", "GF@compare", NULL, NULL);
    generateInstruction("JUMPIFNEQ", labelname, "bool@true", "GF@compare");
}


/* print to output */
void genWrite(int Type, char* content){
    char* ans = (char*) malloc(sizeof(char) * strlen(content) * 10);
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
void genNOTS(){
    generateInstruction("NOTS", NULL, NULL, NULL);
}

/* call function */
void genCall(char* funcname){
    char* ans = (char*) malloc(sizeof(char) * strlen(funcname) + 10);
    sprintf(ans, "_%s", funcname);
    generateInstruction("CALL", ans, NULL, NULL);
    generateInstruction("POPFRAME", NULL, NULL, NULL);
}

/* start of function */
void genFuncHead(char* funcname){
    char* ans = (char*) malloc(sizeof(char) * strlen(funcname) + 10);
    sprintf(ans, "_%s", funcname);
    generateInstruction("\nLABEL", ans, NULL, NULL);
    generateInstruction("PUSHFRAME", NULL, NULL, NULL);
    generateInstruction("CREATEFRAME", NULL, NULL, NULL);
}

/* end of function, return to main */
void genFuncEnd(){
    generateInstruction("RETURN\n", NULL, NULL, NULL);
}

void genInt2Fl(char* number, int type, char* string){
    char* ans = (char*) malloc(sizeof(char) * strlen(number) + 10);
    sprintf(ans, "TF@%s", number);
    char* line = (char*) malloc(sizeof(char) * strlen(string) + 10);
    if(type == T_INT){
        sprintf(line, "int@%s", string);
    }
    else{
        sprintf(line, "TF@%s", string);
    }
    generateInstruction("INT2FLOAT", ans, line, NULL);
}

void genFl2Int(char* number, int type, char* string){
    char* ans = (char*) malloc(sizeof(char) * strlen(number) * 10);
    sprintf(ans, "TF@%s", number);
    char* line = (char*) malloc(sizeof(char) * strlen(string) * 10);
    if(type == T_FLOAT){
        double number = strtod(string,NULL);
        sprintf(line, "float@%a", number);
    }
    else{
        sprintf(line, "TF@%s", string);
    }
    generateInstruction("FLOAT2INT", ans, line, NULL);
}

void genStrlen(char* number, int type, char* string){
    char* ans = (char*) malloc(sizeof(char) * strlen(number) + 10);
    sprintf(ans, "TF@%s", number);
    char* line = (char*) malloc(sizeof(char) * strlen(string) + 10);
    if(type == T_STRING){
        string = stringconvertor(string);
        sprintf(line, "string@%s", string);
    }
    else{
        sprintf(line, "TF@%s", string);
    }
    generateInstruction("STRLEN", ans, line, NULL);
}