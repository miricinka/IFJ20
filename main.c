/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  main.c
 * 
 * DESCRIPTION:
 *  Main file
 * 
 * AUTHORS:
 *  Tomáš Hrúz          <xhruzt00@stud.fit.vutbr.cz>
 *  Kolaříková Mirka    <xkolar76@stud.fit.vutbr.cz>
 *  Aleš Řezáč          <xrezac21@stud.fit.vutbr.cz>
 *  Žovinec Martin      <xzovin00@stud.fit.vutbr.cz>
*/


#include "main.h"

int main() {
	//initialize list of instructions
	tListOfInstr instrList;
   	listInit(&instrList);

   	int result = 0;
   	result = parse(&instrList); // provedeme syntaktickou analyzu
	   if (result != 0) errorMsg(result, "Něco se posralo");
	if (result == 0)
	{
		printf("NOICE\n");
	}
	listFree(&instrList);   
	return 0;
}
