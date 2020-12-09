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


void generateInstruction(char* instType, char* addr1, char* addr2, char* addr3); // insert instruction
char* stringconvertor(char* srting_before); // convert string with ascii values <0;32>, {35} and {92} (ascii value 32 will be print as \032)
void genFileHead(); // .IFJCode20 needs to be printed on first line, followed by global frame variales and unconditioned jump to main function 
void genMainFunc(); // setting label for main function
void genMainEnd(); // code ends here, inserted exit to avoid entering other functions written after main

void genAdds();  //////							//////
void genSubs();  ////							  ////
void genMuls();  // mathematical operatins on stack //
void genDivs();  ////							  ////
void genIDivs(); //////							//////

void genPushs(int type, char* content); // insert integer/float/string/variable into stack
void genDefvar(char* variable); // definition of variable
void genPops(char* variable); // save integer/float/string/variable into variable from stack (gets deleted form stack top)
void genClears(); // clears whole stack
void genRead(int type, char* variable, char* variable2); // save input into variable as integer/float/string  

/* If-else */
void genIfHead(int elsecount); // conditional jump to else 
void genElseHead(int elsecount); // marking start of else
void genIfEnd(int elsecount); // unconditional jump after end of else
void genPostIf(int elsecount); // marking end of If-else

/* For cycle */
void genForHead(int forcount); // mark condition check label
void genForCheck(int forcount); // check condition, jump into body or jump after end of body 
void genForContinue(int forcount); // mark variable modification (third part of for head)
void genForContinueEnd(int forcount); // jump to check
void genForBody(int forcount); // mark start of body
void genForBodyEnd(int forcount); // jump to varible modification (third part of for head)
void genForEnd(int forcount); // mark end of for cycle

void genWrite(int Type, char* content); // printing into output 
void genConcat(); // string concatenation 

void genGTS(); ////				  //// 
void genLTS(); //  in-stack logical //
void genEQS(); //      operators    //
void genNOTS();////				  ////

void genCall(char* funcname); // enter function
void genFuncHead(char* funcname); // mark function's head (start)
void genFuncEnd(); // return back to main function

void genInt2Fl(); // convert integer to float 
void genFl2Int(); // convert float to integer

void genStrlen(char* number, int type, char* string); // get length of string
void genOrdFunc(int type1, char* content1, int type2, char* content2); // get ascii value of symbol
void genSubstrFunc(int type1, char* content1, int type2, char* content2, int type3, char* content3); // get substring form string
void genChr(int type, char* content); // get symbol from ascii value

#endif 