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
 *  Aleš Řezáč          <xrezac21@stud.fit.vutbr.cz>
 *  Taken from reference compiler by VUT FIT in Brno
*/
#include <stdio.h>
#include <stdlib.h>

#ifndef IFJ_ILIST_H
#define IFJ_ILIST_H

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
void printList(tListOfInstr *L); // print all saved instructions, delete list afterwards

#endif //IFJ_ILIST_H
