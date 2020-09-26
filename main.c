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

	string *tokenStr = NULL;
	int i = 0;
	while(i != 51){
		i = get_new_token(tokenStr);
		printf("%d\n",i);
	}

	return 0;
}
