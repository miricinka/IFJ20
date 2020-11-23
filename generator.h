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
 *  Žovinec Martin      <xzovin00@stud.fit.vutbr.cz>
*/

#ifndef IFJ_ILIST_GENERATOR
#define IFJ_ILIST_GENERATOR

#include <stdio.h>
#include <stdlib.h>
#include "ilist.h"
#include "prec_parser.h"

    void generateInstruction(char* instType, char* addr1, char* addr2, char* addr3);
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
    void genRead(char* variable);
    void genReturn();
    void genReturn();

#endif 