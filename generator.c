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

/* generating numbers to use for original naming of labels */
static int SubstrCnt = 0;
static int OrdCnt = 0;
static int ChrCnt = 0;
static int inputCnt = 0;

/* connecting from parser */ 
tListOfInstr *list;

/* Save instruction into instruction list for future printing */
void generateInstruction(char* instType, char* addr1, char* addr2, char* addr3){
   tInstr I;
   I.instType = instType;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   listInsertLast(list, I);
}

/* function to rewrite string into supported version  */
char* stringconvertor(char* string_before){
    string string_after; strInit(&string_after); strClear(&string_after);

    size_t i = 0;
    while (string_before[i] != '\0') {    //loop trough chars

        if ((string_before[i] >= 0 && string_before[i] <= 32)
           || string_before[i] == 35 || string_before[i] == 92){
            strAddChar(&string_after, '\\');
            strAddChar(&string_after, '0');

            int secund_num = string_before[i]%10; 
            int first_num = (string_before[i] - secund_num)/10;
            strAddChar(&string_after,  first_num+'0');
            strAddChar(&string_after,  secund_num+'0');
        }
        else{
            strAddChar(&string_after, string_before[i]);
        }
        i++;
    }
    return string_after.str;
}

/* original label name creator */
char* makeLabel(int labelCount, char *labelType){
    /*len of labelType + int can be max 10 characters (1 character = 1 byte)*/
    char* resultLabel = (char*) malloc(strlen(labelType) + 10);

    sprintf(resultLabel,"%s%d", labelType, labelCount);
    return resultLabel;
}

/* IFJcode20 head */
void genFileHead(){
    generateInstruction(".IFJcode20\n", NULL, NULL, NULL);
    /* conccatenation variables */
    generateInstruction("DEFVAR", "GF@concat1", NULL, NULL);
    generateInstruction("DEFVAR", "GF@concat2", NULL, NULL);
    generateInstruction("DEFVAR", "GF@concatfin", NULL, NULL);
    /* crating '_' variable, which can throw away return values */
    generateInstruction("DEFVAR", "GF@_", NULL, NULL);
    /* comparing in If-Else / for and other logical operations */
    generateInstruction("DEFVAR", "GF@compare", NULL, NULL);
    /* Checking correct type of a variable in inputs, inputi, inputf functions */
    generateInstruction("DEFVAR", "GF@getType", NULL, NULL);
    generateInstruction("DEFVAR", "GF@typeString", NULL, NULL);
    generateInstruction("DEFVAR", "GF@typeFloat", NULL, NULL);
    generateInstruction("DEFVAR", "GF@typeInt", NULL, NULL);
    generateInstruction("MOVE", "GF@typeInt", "string@int", NULL);
    generateInstruction("MOVE", "GF@typeFloat", "string@float", NULL);
    generateInstruction("MOVE", "GF@typeString", "string@string", NULL);
    /* start at mainfunction */ 
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

/* push to stack, depending on type of content */
void genPushs(int type, char* content){
    char* ans = (char*) malloc(sizeof(char) * strlen(content) * 10);
    if(type == NT_INT){ // integer
        sprintf(ans,"int@%s", content);
    }
    else if(type == NT_STR){ // string, convertion necessary
        content = stringconvertor(content);
        sprintf(ans,"string@%s", content);
    }
    else if(type == NT_FLOAT){ // float, saved as double, printed in %a format
        double number = strtod(content,NULL);
        sprintf(ans,"float@%a", number);
    }
    else if(type == NT_ID){ // variable, saved in local frame
        sprintf(ans,"LF@%s", content);
    }
    else{}
    generateInstruction("PUSHS", ans, NULL, NULL);
}

/* variable definition */
void genDefvar(char* variable){
    char* ans = (char*) malloc(sizeof(char) * strlen(variable) + 10);
    sprintf(ans, "LF@%s", variable);  // set for local frame 
    generateInstruction("DEFVAR", ans, NULL, NULL);
}

/* pop from stack */
void genPops(char* variable){
    char* ans = (char*) malloc(sizeof(char) * strlen(variable) + 10);
    sprintf(ans, "LF@%s", variable);
    if(strcmp(variable, "_") == 0){ // if popping to '_' then it is set for '_' in global frame
        ans = "GF@_";
    }
    generateInstruction("POPS", ans, NULL, NULL);
}

/* clear stack */
void genClears(){
    generateInstruction("CLEARS", NULL, NULL, NULL);
}

/* detecting input */
void genRead(int type, char* variable,  char* variable2){
    char* ans = (char*) malloc(sizeof(char) * strlen(variable) + 10);
    sprintf(ans, "LF@%s", variable);
    if(strcmp(variable, "_") == 0){
        ans = "GF@_";
    }
    char* ans2 = (char*) malloc(sizeof(char) * strlen(variable2) + 10);
    sprintf(ans2, "LF@%s", variable2);
    if(strcmp(variable2, "_") == 0){
        ans2 = "GF@_";
    }

    if(type == F_INPUTI){ // input as integer 
        char *label = makeLabel(++inputCnt,"inputi");
        generateInstruction("READ", ans, "int", NULL);
        generateInstruction("TYPE", "GF@getType", ans, NULL);
        generateInstruction("MOVE", ans2, "int@0", NULL);

        generateInstruction("JUMPIFEQ", label, "GF@getType", "GF@typeInt");
        generateInstruction("MOVE", ans2, "int@1", NULL);
        generateInstruction("\nLABEL", label, NULL, NULL);

    }else if(type == F_INPUTF){ // input as float
        char *label = makeLabel(++inputCnt,"inputf");
        generateInstruction("READ", ans, "float", NULL);
        generateInstruction("TYPE", "GF@getType", ans, NULL);
        generateInstruction("MOVE", ans2, "int@0", NULL);

        generateInstruction("JUMPIFEQ", label, "GF@getType", "GF@typeFloat");
        generateInstruction("MOVE", ans2, "int@1", NULL);
        generateInstruction("\nLABEL", label, NULL, NULL);

    }else if(type == F_INPUTS){ // input s string
        char *label = makeLabel(++inputCnt,"inputs");
        generateInstruction("READ", ans, "string", NULL);
        generateInstruction("TYPE", "GF@getType", ans, NULL);
        generateInstruction("MOVE", ans2, "int@0", NULL);

        generateInstruction("JUMPIFEQ", label, "GF@getType", "GF@typeString");
        generateInstruction("MOVE", ans2, "int@1", NULL);
        generateInstruction("\nLABEL", label, NULL, NULL);
    }
}

/* if head, conditioned jump to else  */
void genIfHead(int elsecount){
    char *label = makeLabel(elsecount, "elselabel_");
    generateInstruction("\nPOPS", "GF@compare", NULL, NULL);
    generateInstruction("JUMPIFNEQ", label, "bool@true", "GF@compare");
}

/* else head, setting label */
void genElseHead(int elsecount){
    char *label = makeLabel(elsecount, "elselabel_");
    generateInstruction("\nLABEL", label, NULL, NULL);
}

/* end of if, jump behind same scope level else */
void genIfEnd(int elsecount){
    char *label = makeLabel(elsecount, "postif_");
    generateInstruction("JUMP", label, NULL, NULL);
}

/* setting label to continue from if */
void genPostIf(int elsecount){
    char *label = makeLabel(elsecount, "postif_");
    generateInstruction("\nLABEL", label, NULL, NULL);
    elsecount--;
}

/* for head, condition checking */
void genForHead(int forcount){
    char *label = makeLabel(forcount, "forcheck_");
    generateInstruction("\nLABEL", label, NULL, NULL);
}

/* for head, conditioned jumping after for-body or going into for-body */
void genForCheck(int forcount){
    char *label = makeLabel(forcount, "forend_");
    char *label2 = makeLabel(forcount, "forbody_");
    generateInstruction("POPS", "GF@compare", NULL, NULL);
    generateInstruction("JUMPIFNEQ", label, "bool@true", "GF@compare");
    generateInstruction("JUMP", label2, "\n", NULL);
}

/* for head, third part, vork with variables */
void genForContinue(int forcount){
    char *label = makeLabel(forcount, "forcontinue_");
    generateInstruction("\nLABEL", label, NULL, NULL);
}

/* for head, after work with variables, jump to check  */
void genForContinueEnd(int forcount){
    char *label = makeLabel(forcount, "forcheck_");
    generateInstruction("JUMP", label, "\n", NULL);
}

/* for body, main part */
void genForBody(int forcount){
    char *label = makeLabel(forcount, "forbody_");
    generateInstruction("\nLABEL", label, NULL, NULL);
}

/* for body, at the end work with variables in third for-head part is needed */
void genForBodyEnd(int forcount){
    char *label = makeLabel(forcount, "forcontinue_");
    generateInstruction("JUMP", label, "\n", NULL);
}

/* marking end of for */
void genForEnd(int forcount){
    char *label = makeLabel(forcount, "forend_");
    generateInstruction("LABEL", label, NULL, NULL);
}

/* print to output, depening on type of content */
void genWrite(int Type, char* content){
    char* ans = (char*) malloc(sizeof(char) * strlen(content) * 10);
    if(Type == T_INT){ // integer
        sprintf(ans, "int@%s", content);
    }
    else if(Type == T_FLOAT){ // float, saved as double, printed in %a format
        double number = strtod(content, NULL);
        sprintf(ans, "float@%a", number);
    }
    else if(Type == T_STRING){ // string, convertion needed
        content = stringconvertor(content);
        sprintf(ans, "string@%s", content);
    }
    else if(Type == ID){ // ID, set in local frame
        sprintf(ans, "LF@%s", content);
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

/* stack comparing '>' */
void genGTS(){
    generateInstruction("GTS", NULL, NULL, NULL);
}

/* stack comparing '<' */
void genLTS(){
    generateInstruction("LTS", NULL, NULL, NULL);
}

/* stack comparing '==' */
void genEQS(){
    generateInstruction("EQS", NULL, NULL, NULL);
}

/* stack compraing '!' */
void genNOTS(){
    generateInstruction("NOTS", NULL, NULL, NULL);
}

/* call a function */
void genCall(char* funcname){
    char* ans = (char*) malloc(sizeof(char) * strlen(funcname) + 10);
    sprintf(ans, "_%s", funcname);
    generateInstruction("CALL", ans, NULL, NULL);
    generateInstruction("POPFRAME", NULL, NULL, NULL);
}

/* start of function, label setting */
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

/* integer to float convertor function, input possible as integer value or variable value */
void genInt2Fl(char* number, int type, char* string){
    char* ans = (char*) malloc(sizeof(char) * strlen(number) + 10);
    sprintf(ans, "LF@%s", number);
    if(strcmp(number, "_") == 0){
        ans = "GF@_";
    }
    char* line = (char*) malloc(sizeof(char) * strlen(string) + 10);
    if(type == T_INT){
        sprintf(line, "int@%s", string);
    }
    else{
        sprintf(line, "LF@%s", string);
        if(strcmp(string, "_") == 0){
            line = "GF@_";
        }
    }
    generateInstruction("INT2FLOAT", ans, line, NULL);
}

/* float to integer convertor function, input possible as float value or vavriable value */
void genFl2Int(char* number, int type, char* string){
    char* ans = (char*) malloc(sizeof(char) * strlen(number) * 10);
    sprintf(ans, "LF@%s", number);
    if(strcmp(number, "_") == 0){
        ans = "GF@_";
    }
    char* line = (char*) malloc(sizeof(char) * strlen(string) * 10);
    if(type == T_FLOAT){ // float written in %a format
        double fl_number = strtod(string,NULL);
        sprintf(line, "float@%a", fl_number);
    }
    else{
        sprintf(line, "LF@%s", string);
        if(strcmp(string, "_") == 0){
            line = "GF@_";
        }
    }
    generateInstruction("FLOAT2INT", ans, line, NULL);
}

/* length of string funtion */
void genStrlen(char* number, int type, char* string){
    char* ans = (char*) malloc(sizeof(char) * strlen(number) + 10);
    sprintf(ans, "LF@%s", number);
    char* line = (char*) malloc(sizeof(char) * strlen(string) + 10);
    if(type == T_STRING){
        string = stringconvertor(string);
        sprintf(line, "string@%s", string);
    }
    else{
        sprintf(line, "LF@%s", string);
        if(strcmp(string, "_") == 0){
            line = "GF@_";
        }
    }
    generateInstruction("STRLEN", ans, line, NULL);
}

/* ascii value of symbol function */
void genOrdFunc(int type1, char* content1, int type2, char* content2){
    char* ans1 = (char*) malloc(sizeof(char) * strlen(content1) + 10);
    char* ans2 = (char*) malloc(sizeof(char) * strlen(content2) + 10);
    if(type1 == T_STRING){
        content1 = stringconvertor(content1);
        sprintf(ans1, "string@%s", content1);
    }
    else{
        sprintf(ans1, "LF@%s", content1);
    }
    if(type2 == T_INT){
        sprintf(ans2, "int@%s", content2);
    }
    else{
        sprintf(ans2, "LF@%s", content2);
    }
    char* label = makeLabel(++OrdCnt, "$ord_fail");
    char* label2 = makeLabel(OrdCnt, "$ord_end");
    
    generateInstruction("DEFVAR", "LF@$returnvar", NULL, NULL);
    generateInstruction("DEFVAR", "LF@$errorvar", NULL, NULL);
    generateInstruction("DEFVAR", "TF@length", NULL, NULL);
    generateInstruction("STRLEN", "TF@length", ans1, NULL);
    generateInstruction("PUSHS", "TF@length", NULL, NULL);
    generateInstruction("PUSHS", "int@1", NULL, NULL);
    generateInstruction("SUBS", NULL, NULL, NULL);
    generateInstruction("PUSHS", ans2, NULL, NULL);
    generateInstruction("GTS", NULL, NULL, NULL);
    generateInstruction("PUSHS", "int@-1", NULL, NULL);
    generateInstruction("PUSHS", ans2, NULL, NULL);
    generateInstruction("LTS", NULL, NULL, NULL);
    generateInstruction("ANDS", NULL, NULL, NULL);
    generateInstruction("POPS", "GF@compare", NULL, NULL);
    generateInstruction("JUMPIFNEQ", label, "bool@true", "GF@compare");
    generateInstruction("PUSHS", "int@0", NULL, NULL);
    generateInstruction("POPS", "LF@$errorvar", NULL, NULL);
    generateInstruction("STRI2INT", "LF@$returnvar", ans1, ans2);
    generateInstruction("JUMP", label2, NULL, NULL);
    generateInstruction("\nLABEL", label, NULL, NULL);
    generateInstruction("PUSH", "int@1", NULL, NULL);
    generateInstruction("POPS", "LF@$errorvar", NULL, NULL);
    generateInstruction("\nLABEL", label2, NULL, NULL);
    generateInstruction("PUSHS", "LF@$errorvar", NULL, NULL);
    generateInstruction("PUSHS", "LF@$returnvar", NULL, NULL);
}

/* getting substring from string function */
void genSubstrFunc(int type1, char* content1, int type2, char* content2, int type3, char* content3){
    char* ans1 = (char*) malloc(sizeof(char) * strlen(content1) + 10);
    char* ans2 = (char*) malloc(sizeof(char) * strlen(content2) + 10);
    char* ans3 = (char*) malloc(sizeof(char) * strlen(content3) + 10);
    if(type1 == T_STRING){
        content1 = stringconvertor(content1);
        sprintf(ans1, "string@%s", content1);
    }
    else{
        sprintf(ans1, "LF@%s", content1);
    }
    if(type2 == T_INT){
        sprintf(ans2, "int@%s", content2);
    }
    else{
        sprintf(ans2, "LF@%s", content2);
    }
    if(type3 == T_INT){
        sprintf(ans3, "int@%s", content3);
    }
    else{
        sprintf(ans3, "LF@%s", content3);
    }
    char* label = makeLabel(++SubstrCnt, "$substrfail");
    char* label2 = makeLabel(SubstrCnt, "$substrskip");
    char* label3 = makeLabel(SubstrCnt, "$substrwhile");
    char* label4 = makeLabel(SubstrCnt, "$substrwhileend");
    char* label5 = makeLabel(SubstrCnt, "$substrend");
    generateInstruction("DEFVAR", "TF@a", NULL, NULL);
    generateInstruction("DEFVAR", "TF@$letter", NULL, NULL);
    generateInstruction("DEFVAR", "LF@$returnvar", NULL, NULL);
    generateInstruction("DEFVAR", "LF@$errorvar", NULL, NULL);
    generateInstruction("DEFVAR", "TF@$length", NULL, NULL);
    generateInstruction("STRLEN", "TF@$length", ans1, NULL);
    generateInstruction("PUSHS", "TF@$length", NULL, NULL);
    generateInstruction("PUSHS", "int@1", NULL, NULL);
    generateInstruction("ADDS", NULL, NULL, NULL);
    generateInstruction("POPS", "TF@$length", NULL, NULL);
    generateInstruction("PUSHS", ans2, NULL, NULL);
    generateInstruction("PUSHS", "int@-1", NULL, NULL);
    generateInstruction("GTS", NULL, NULL, NULL);
    generateInstruction("PUSHS", ans2, NULL, NULL);
    generateInstruction("PUSHS", "TF@$length", NULL, NULL);
    generateInstruction("LTS", NULL, NULL, NULL);
    generateInstruction("ANDS", NULL, NULL, NULL);
    generateInstruction("PUSHS", ans3, NULL, NULL);
    generateInstruction("PUSHS", "int@0", NULL, NULL);
    generateInstruction("GTS", NULL, NULL, NULL);
    generateInstruction("ORS", NULL, NULL, NULL);
    generateInstruction("POPS", "GF@compare", NULL, NULL);
    generateInstruction("JUMPIFNEQ", label, "bool@true", "GF@compare");
    /* successful */
    generateInstruction("PUSHS", "int@0", NULL, NULL);
    generateInstruction("POPS", "LF@$errorvar", NULL, NULL);
    generateInstruction("PUSHS", ans3, NULL, NULL);
    generateInstruction("PUSHS", "TF@$length", NULL, NULL);
    generateInstruction("PUSHS", "int@1", NULL, NULL);
    generateInstruction("SUBS", NULL, NULL, NULL);
    generateInstruction("PUSHS", ans2, NULL, NULL);
    generateInstruction("SUBS", NULL, NULL, NULL);
    generateInstruction("GTS", NULL, NULL, NULL);
    generateInstruction("POPS", "GF@compare", NULL, NULL);
    generateInstruction("JUMPIFNEQ", label2, "bool@true", "GF@compare");
    /* if n > (len(s) - i), setting length of substring to len(s) - i */
    generateInstruction("PUSHS", "TF@$length", NULL, NULL);
    generateInstruction("PUSHS", ans2, NULL, NULL);
    generateInstruction("SUBS", NULL, NULL, NULL);
    generateInstruction("SUBS", NULL, NULL, NULL);
    generateInstruction("POPS", ans3, NULL, NULL);
    /* continue here if n <= (len(s) -i) */
    generateInstruction("\nLABEL", label2, NULL, NULL);
    generateInstruction("PUSHS", "int@0", NULL, NULL);
    generateInstruction("POPS", "TF@a", NULL, NULL);
    /* while function, writing string wwith length of n */
    generateInstruction("\nLABEL", label3, NULL, NULL);
    generateInstruction("PUSHS", "TF@a", NULL, NULL);
    generateInstruction("PUSHS", ans3, NULL, NULL);
    generateInstruction("GTS", NULL, NULL, NULL);
    generateInstruction("POPS", "GF@compare", NULL, NULL);
    generateInstruction("JUMPIFEQ", label4, "bool@true", "GF@compare");
    generateInstruction("GETCHAR", "TF@$letter", ans1, ans2);
    generateInstruction("SETCHAR", "LF@returnvar", "TF@a", "TF@$letter");
    /* move to other position in substring */
    generateInstruction("PUSHS", "TF@a", NULL, NULL);
    generateInstruction("PUSHS", "int@1", NULL, NULL);
    generateInstruction("ADDS", NULL, NULL, NULL);
    generateInstruction("POPS", "TF@a", NULL, NULL);
    /* move to other position in original string */
    generateInstruction("PUSHS", ans2, NULL, NULL);
    generateInstruction("PUSHS", "int@1", NULL, NULL);
    generateInstruction("ADDS", NULL, NULL, NULL);
    generateInstruction("POPS", ans2, NULL, NULL);
    generateInstruction("JUMP", label3, NULL, NULL);
    /* end of while */
    generateInstruction("\nLABEL", label4, NULL, NULL);
    /* jump to the end of substring function */
    generateInstruction("JUMP", label5, NULL, NULL);
    /* set error variable as positive */
    generateInstruction("\nLABEL", label, NULL, NULL);
    generateInstruction("PUSHS", "int@1", NULL, NULL);
    generateInstruction("POPS", "LF@$errorvar", NULL, NULL);
    /* end of function, save values to stack, which will be popped into variables */
    generateInstruction("\nLABEL", label5, NULL, NULL);
    generateInstruction("PUSHS", "LF@$errorvar", NULL, NULL);
    generateInstruction("PUSHS", "LF@$returnvar", NULL, NULL);
}

/* print ASCII value function */
void genChr(int type, char* content){
    char* ans = (char*) malloc(sizeof(char) * strlen(content) + 10);
    generateInstruction("DEFVAR", "LF@$returnvar", NULL, NULL); // answer
    generateInstruction("DEFVAR", "LF@$errorvar", NULL, NULL); // error occured
    if(type == T_INT){
        sprintf(ans, "int@%s", content);
    }
    else{
        sprintf(ans, "LF@%s", content);

    }
    char* label = makeLabel(++ChrCnt, "$fail");
    char* label2 = makeLabel(ChrCnt, "$chrend");
    generateInstruction("MOVE", "LF@$errorvar", "int@0", NULL);
    /* let's check if input is in range of <0;255> */
    generateInstruction("PUSHS", ans, NULL, NULL);
    generateInstruction("PUSHS", "int@-1", NULL, NULL);
    generateInstruction("GTS", NULL, NULL, NULL);
    generateInstruction("PUSHS", "int@256", NULL, NULL);
    generateInstruction("PUSHS", ans, NULL, NULL);
    generateInstruction("LTS", NULL, NULL, NULL);
    generateInstruction("ANDS", NULL, NULL, NULL);
    generateInstruction("POPS", "GF@compare", NULL, NULL);
    /* if it isn't, set error variable as 1 */
    generateInstruction("JUMPIFNEQ", label, "bool@true", "GF@compare");
    generateInstruction("INT2CHAR", "LF@$returnvar", ans, NULL);
    generateInstruction("JUMP", label2, NULL, NULL);
    generateInstruction("\nLABEL", label, NULL, NULL);
    generateInstruction("MOVE", "LF@$errorvar", "int@1", NULL);
    generateInstruction("\nLABEL", label2, NULL, NULL);
    /* save return vlues into stack, which will be popped right away */
    generateInstruction("PUSHS", "LF@$errorvar", NULL, NULL);
    generateInstruction("PUSHS", "LF@$returnvar", NULL, NULL);
}