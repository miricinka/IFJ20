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
void generateInstruction(char* instType, char* addr1, char* addr2, char* addr3){
   tInstr I;
   I.instType = instType;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   listInsertLast(list, I);
}

void genFileHead(){
    generateInstruction(".IFJcode20\n", NULL, NULL, NULL);
    generateInstruction("DEFVAR", "GF@concat1", NULL, NULL);
    generateInstruction("DEFVAR", "GF@concat2", NULL, NULL);
    generateInstruction("DEFVAR", "GF@concatfin\n", NULL, NULL);
    generateInstruction("JUMP", "_main\n", NULL, NULL);
        
}

void genMainFunc(){
    generateInstruction("LABEL", "_main", NULL, NULL);
    generateInstruction("CREATEFRAME", NULL, NULL, NULL);
}

void genMainEnd(){
    generateInstruction("POPFRAME", NULL, NULL, NULL);
    generateInstruction("CLEARS\n", NULL, NULL, NULL);
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
    char* ans = (char*) malloc(sizeof(char) * strlen(content));
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
    char* ans = (char*) malloc(sizeof(char) * strlen(variable));
    sprintf(ans, "TF@%s", variable);
    generateInstruction("DEFVAR", ans, NULL, NULL);
}

void genPops(char* variable){
    char* ans = (char*) malloc(sizeof(char) * strlen(variable));
    sprintf(ans, "TF@%s", variable);
    generateInstruction("POPS", ans, NULL, NULL);
}

void genClears(){
    generateInstruction("CLEARS", NULL, NULL, NULL);
}

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

void genReturn(){
    generateInstruction("RETURN", NULL, NULL, NULL);
}

void genIfElseHead(){
    char* labelname = (char*) malloc(sizeof(labelcnt));
    sprintf(labelname, "_label%d", labelcnt);
    labelcnt++;
    generateInstruction("LABEL", labelname, NULL, NULL);
}

void genIfElseEnd(){
    char* finif = (char*) malloc(sizeof(endif));
    sprintf(finif, "_endif%d", endif);
    endif++;
    generateInstruction("JUMP", finif, NULL, NULL);
}

void genPostIf(){
    char* successor = (char*) malloc(sizeof(endif));
    sprintf(successor, "_endif%d", endif);
    endif++;
    generateInstruction("LABEL", successor, NULL, NULL);
}

void genWrite(int Type, char* content){
    char* ans = (char*) malloc(sizeof(char) * strlen(content));
    if(Type == T_INT){
        sprintf(ans, "int@%s", content);
    }
    else if(Type == T_FLOAT){
        sprintf(ans, "float@%s", content);
    }
    else if(Type == T_STRING){
        sprintf(ans, "string@%s", content);
    }
    else if(Type == ID){
        sprintf(ans, "TF@%s", content);
    }
    else{}
    generateInstruction("WRITE", ans, NULL, NULL);
}

void genConcat(){
    generateInstruction("POPS", "GF@concat1", NULL, NULL);
    generateInstruction("POPS", "GF@concat2", NULL, NULL);
    generateInstruction("CONCAT", "GF@concatfin", "GF@concat1", "GF@concat2");
    generateInstruction("PUSHS", "GF@concatfin", NULL, NULL);
}
