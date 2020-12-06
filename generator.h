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
void genDefvar(char* variable, int frame);
void genPops(char* variable, int frame);
void genClears();
void genRead(int type, char* variable, char* variable2, int frame);
void genIfHead(int elsecount);
void genElseHead(int elsecount);
void genIfEnd(int elsecount);
void genPostIf(int elsecount);
void genForHead(int forcount);
void genForCheck(int forcount);
void genForContinue(int forcount);
void genForContinueEnd(int forcount);
void genForBody(int forcount);
void genForBodyEnd(int forcount);
void genForEnd(int forcount);
void genWrite(int Type, char* content, int frame);
void genConcat();
void genGTS();
void genLTS();
void genEQS();
void genNOTS();
void genCall(char* funcname);
void genFuncHead(char* funcname);
void genFuncEnd();
void genInt2Fl();
void genFl2Int();
void genStrlen(char* number, int type, char* string);

#endif 