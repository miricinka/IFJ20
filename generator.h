/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  generator.h
 * 
 * DESCRIPTION:
 *  Main file
 * 
 * AUTHORS:
 *  Aleš Řezáč          <xrezac21@stud.fit.vutbr.cz>
*/

#ifndef IFJ_ILIST_GENERATOR
#define IFJ_ILIST_GENERATOR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ilist.h"
#include "prec_parser.h"


typedef struct labelStack{
	int labelCount;

	struct labelStack *previousElement;
} *labelStack;

void genStackPop(labelStack* stack);
void genStackPush(labelStack* stack, int newLabel);
void genStackDelete(labelStack* stack);
void genStackTest(labelStack* stack);

void generateInstruction(char* instType, char* addr1, char* addr2, char* addr3);
char* stringconvertor(char* srting_before);
void genFileHead();
void genMainFunc();
void genMainEnd();
void genAdds();
void genSubs();
void genMuls();
void genDivs();
void genIDivs();
void genPushs(int type, char* content);
void genDefvar(char* variable);
void genPops(char* variable);
void genClears();
void genRead(int type, char* variable);
void genIfElseHead();
void genIfElseEnd();
void genPostIf();
void genWrite(int Type, char* content);
void genConcat();
void genGTS();
void genLTS();
void genEQS();
void genNOT();
void genCall(char* funcname);
void genFuncHead(char* funcname);
void genFuncEnd();
void genInt2Fl();
void genFl2Int();
void genStrlen(char* number, int type, char* string);

#endif 