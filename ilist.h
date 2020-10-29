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
 *  Hrúz Tomáš    <xhruzt00@stud.fit.vutbr.cz>
 *  
*/
#include <stdio.h>
#include <malloc.h>

#ifndef IFJ_ILIST_H
#define IFJ_ILIST_H
// typy jednotlivych instrukci
//
//
//
//
//!!!!!!

typedef struct
{
  int instType;  // typ instrukce
  void *addr1; // adresa 1
  void *addr2; // adresa 2
  void *addr3; // adresa 3
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

#endif //IFJ_ILIST_H