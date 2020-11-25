/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  list.h
 * 
 * DESCRIPTION:
 *  List functions
 * 
 * AUTHORS:
 *  Tomáš Hrúz          <xhruzt00@stud.fit.vutbr.cz>
 *  Aleš Řezáč          <xrezac21@stud.fit.vutbr.cz>
 *  Žovinec Martin      <xzovin00@stud.fit.vutbr.cz> 
*/
#include <stdio.h>
#include <stdlib.h>

#ifndef IFJ_ILIST_H
#define IFJ_ILIST_H

#define I_MOVE 1
#define I_CREATEFRAME 2
#define I_PUSHFRAME 3
#define I_POPFRAME 4
#define I_DEFVAR 5
#define I_CALL 6
#define I_RETURN 7
#define I_PUSHS 8
#define I_POPS 9
#define I_CLEARS 10
#define I_ADD 11
#define I_SUB 12
#define I_MUL 13
#define I_DIV 14
#define I_IDIV 15
#define I_ADDS 16
#define I_SUBS 17
#define I_MULS 18
#define I_DIVS 19
#define I_IDIVS 20
#define I_LT 21
#define I_GT 22
#define I_EQ 23
#define I_LTS 24
#define I_GTS 25
#define I_EQS 26
#define I_AND 27
#define I_OR 28
#define I_NOT 29
#define I_ANDS 30
#define I_ORS 31
#define I_NOTS 32
#define I_INT2FLOAT 33
#define I_FLOAT2INT 34
#define I_INT2CHAR 35
#define I_STRI2INT 36
#define I_INT2FLOATS 37
#define I_FLOAT2INTS 38
#define I_INT2CHARS 39
#define I_STRI2INTS 40
#define I_READ 41
#define I_WRITE 42
#define I_CONCAT 43
#define I_STRLEN 44
#define I_GETCHAR 45
#define I_SETCHAR 46
#define I_TYPE 47
#define I_LABEL 48
#define I_JUMP 49
#define I_JUMPIFEQ 50
#define I_JUMPIFNEQ 51
#define I_JUMPIFEQS 52
#define I_JUMPIFNEQS 53
#define I_EXIT 54
#define I_BREAK 55
#define I_DPRINT 56

typedef struct
{
  char* instType;  // typ instrukce
  char* addr1; // adresa 1
  char* addr2; // adresa 2
  char* addr3; // adresa 3
} tInstr;

typedef struct listItem
{
  tInstr Instruction;
  struct listItem *nextItem;
} tListItem;
    
typedef struct
{
  struct listItem *first;  // ukazatel na prvni prvek
  struct listItem *last;   // ukazatel na posledni prvek
  struct listItem *active; // ukazatel na aktivni prvek
} tListOfInstr;

void listInit(tListOfInstr *L);
void listFree(tListOfInstr *L);
void listInsertLast(tListOfInstr *L, tInstr I);
void listFirst(tListOfInstr *L);
void listNext(tListOfInstr *L);
void listGoto(tListOfInstr *L, void *gotoInstr);
void *listGetPointerLast(tListOfInstr *L);
tInstr *listGetData(tListOfInstr *L);
void printList(tListOfInstr *L);

#endif //IFJ_ILIST_H
