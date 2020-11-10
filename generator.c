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

void generateInstruction(int instType, void *addr1, void *addr2, void *addr3){
   tInstr I;
   I.instType = instType;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   listInsertLast(list, I);
}

void genReturn(){
    generateInstruction(I_RETURN, NULL, NULL, NULL);
}

void genPrint(void *addr1){
    generateInstruction(I_DPRINT, addr1, NULL, NULL);
}


