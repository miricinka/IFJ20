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


int labelcnt = 1;
int endif = 1;

tListOfInstr *list;
void generateInstruction(char* instType, char* addr1, char* addr2, char* addr3){
   tInstr I;
   I.instType = instType;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   listInsertLast(list, I);
}

/*void genReturn(){

    generateInstruction(I_RETURN, NULL, NULL, NULL);
}

void genPrint(void *addr1){
    generateInstruction(I_WRITE, addr1, NULL, NULL);
}

void genDefvar(void *addr1){
    generateInstruction(I_DEFVAR, addr1, NULL, NULL);
}

void genFunc(void *addr1){
    generateInstruction(I_LABEL, addr1, NULL, NULL);
    generateInstruction(I_CREATEFRAME, NULL, NULL, NULL);
}

void genJump(void *addr1){
    generateInstruction(I_JUMP, addr1, NULL, NULL);
}

void Compare(void *addr1, void *addr2, void *addr3){
    switch (addr2){
        case "==":
            generateInstruction(I_JUMPIFNEQ, NULL, addr1, addr3);
            break;

        case "!=":
            generateInstruction(I_JUMPIFEQ, NULL, addr1, addr3);
            break;

        case ">":
            generateInstruction(I_GT, NULL, addr1, addr3);
            generateInstruction(I_JUMPIFEQ, NULL, "bool@true", NULL);
            break;

        case "<":
            generateInstruction(I_LT, NULL, addr1, addr3);
            generateInstruction(I_JUMPIFEQ, NULL, "bool@true", NULL);
            break;

        case "=>" || ">=":
            generateInstruction(I_LT, NULL, addr1, addr3);
            generateInstruction(I_JUMPIFNEQ, NULL, "bool@true", NULL);
            break;

        case "=<" || "<=":
            generateInstruction(I_GT, NULL, addr1, addr3);
            generateInstruction(I_JUMPIFNEQ, NULL, "bool@true", NULL);
            break;

        default:
            break;
    }
}


void genForhead(char *part1, char *part2, char *part3){
    generateInstruction(I_LABEL, NULL, NULL, NULL);
    if(part1 != NULL){ // variable declaration/initialization
        generateInstruction(I_DEFVAR, promena, hodnota, NULL;);
    }
    Compare(part2);
    if(part3 != NULL){
        if(part3 == '^++.*'){
            generateInstruction(I_ADD, promena, 1, NULL);
        }
        if(part3 == '^--.*'){
            generateInstruction(I_SUB, promena, 1, NULL);
        }
    }
}

void genIfhead(){
    generateInstruction(I_LABEL, NULL, NULL, NULL);

}*/


void genFileHead(){
    generateInstruction(".IFJcode20", NULL, NULL, NULL);
    generateInstruction("JUMP", "_main", NULL, NULL);
}

void genMainFunc(){
    generateInstruction("LABEL", "_main", NULL, NULL);
    generateInstruction("CREATEFRAME", NULL, NULL, NULL);
}

void genMainEnd(){
    generateInstruction("POPFRAME", NULL, NULL, NULL);
}

void genAdds(){
    generateInstruction("ADDS", NULL, NULL, NULL);
}

void genSubs(){
    generateInstruction("SUBS", NULL, NULL, NULL);
}

void genMuls(){
    generateInstruction("MULS", NULL, NULL, NULL);
}

void genDivs(){
    generateInstruction("DIVS", NULL, NULL, NULL);
}

void genIDivs(){
    generateInstruction("IDIVS", NULL, NULL, NULL);
}

void genPushs(int type, char* content){
    char* ans = NULL;
    if(type == NT_INT){
        sprintf(ans,"int@%s", content);
    }
    else if(type == NT_STR){
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

void genDefvar(char* variable){
    char* ans = NULL;
    sprintf(ans, "TF@%s", variable);
    generateInstruction("DEFVAR", ans, NULL, NULL);
}

void genPops(char* variable){
    char* ans = NULL;
    sprintf(ans, "TF@%s", variable);
    generateInstruction("POPS", ans, NULL, NULL);
}

void genClears(){
    generateInstruction("CLEARS", NULL, NULL, NULL);
}

void genRead(char* variable){
    char* ans = NULL;
    sprintf(ans, "TF@%s", variable);
    generateInstruction("READ", ans, "int", NULL);
}

void genReturn(){
    generateInstruction("RETURN", NULL, NULL, NULL);
}

void genIfElseHead(){
    char* labelname = NULL;
    sprintf(labelname, "_label%d", labelcnt);
    labelcnt++;
    generateInstruction("LABEL", labelname, NULL, NULL);
}

void genIfElseEnd(){
    char* finif = NULL;
    sprintf(finif, "_endif%d", endif);
    generateInstruction("JUMP", finif, NULL, NULL);
}

void genPostIf(){
    char* successor = NULL;
    sprintf(successor, "_endif%d", endif);
    endif++;
    generateInstruction("LABEL", successor, NULL, NULL);
}

void genAnds(){

}

void genPrint(char* content){
    generateInstruction("WRITE", content, NULL, NULL);
}

/*
int main(){
    genClears();
    listFirst(list);
    printf("%s\n", list->active->Instruction.instType);
    return 0;
}*/
