/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  str.h
 * 
 * DESCRIPTION:
 *  Lexical analysis
 * 
 * AUTHORS:
 *  Kolaříková Mirka    <xkolar76@stud.fit.vutbr.cz>
 *  Žovinec Martin      <xzovin00@stud.fit.vutbr.cz>
 *  Taken from reference compiler by VUT FIT in Brno 
*/

//#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


#ifndef IFJ_STRING_H
#define IFJ_STRING_H
typedef struct
{
  char* str;		// misto pro dany retezec ukonceny znakem '\0' place for string ended with char '\0'
  int length;		// real lenght of string
  int allocSize;	// size of allocated memory
} string;


int strInit(string *s);
void strFree(string *s);

void strClear(string *s);
int strAddChar(string *s1, char c);
int strAddChars(string *s1, char *c);
int strCopyString(string *s1, string *s2);
int strCmpString(string *s1, string *s2);
int strCmpConstStr(string *s1, char *s2);

char *strGetStr(string *s);
int strGetLength(string *s);

#endif //IFJ_STRING_H


