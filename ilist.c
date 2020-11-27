/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  list.c
 * 
 * DESCRIPTION:
 *  List functions
 * 
 * AUTHORS:
 *  Aleš Řezáč <xrezac21@stud.fit.vutbr.cz>
 *  Taken from reference compiler by VUT FIT in Brno 
*/

#include "ilist.h"

void listInit(tListOfInstr *L)
// funkce inicializuje seznam instrukci
{
  L->first  = NULL;
  L->last   = NULL;
  L->active = NULL;
}
  
void listFree(tListOfInstr *L)
// funkce dealokuje seznam instrukci
{
  tListItem *ptr;
  while (L->first != NULL)
  {
    ptr = L->first;
    L->first = L->first->nextItem;
    // uvolnime celou polozku
    free(ptr);
  }
}

void listInsertLast(tListOfInstr *L, tInstr I)
// vlozi novou instruci na konec seznamu
{
  tListItem *newItem;
  newItem = malloc(sizeof (tListItem));
  newItem->Instruction = I;
  newItem->nextItem = NULL;
  if (L->first == NULL)
     L->first = newItem;
  else
     L->last->nextItem=newItem;
  L->last=newItem;
}

void listFirst(tListOfInstr *L)
// zaktivuje prvni instrukci
{
  L->active = L->first;
}

void listNext(tListOfInstr *L)
// aktivni instrukci se stane nasledujici instrukce
{
  if (L->active != NULL)
  L->active = L->active->nextItem;
}

void listGoto(tListOfInstr *L, void *gotoInstr)
// nastavime aktivni instrukci podle zadaneho ukazatele
// POZOR, z hlediska predmetu IAL tato funkce narusuje strukturu
// abstraktniho datoveho typu
{
  L->active = (tListItem*) gotoInstr;
}

void *listGetPointerLast(tListOfInstr *L)
// vrati ukazatel na posledni instrukci
// POZOR, z hlediska predmetu IAL tato funkce narusuje strukturu
// abstraktniho datoveho typu
{
  return (void*) L->last;
}

tInstr *listGetData(tListOfInstr *L)
// vrati aktivni instrukci
{
  if (L->active == NULL)
  {
    printf("Chyba, zadna instrukce neni aktivni");
    return NULL;
  }
  else return &(L->active->Instruction);
}

void printList(tListOfInstr *L)
{
  listFirst(L);
  while (L->active != NULL)
  {
    if(L->active->Instruction.instType != NULL){
      printf("%s", L->active->Instruction.instType);
    }
    else{
      listNext(L);
      printf("\n");
      continue;
    }
    if(L->active->Instruction.addr1 != NULL){
      printf(" %s", L->active->Instruction.addr1);
    }
    else{
      listNext(L);
      printf("\n");
      continue;
    }
    if(L->active->Instruction.addr2 != NULL){
      printf(" %s", L->active->Instruction.addr2);
    }
    else{
      listNext(L);
      printf("\n");
      continue;
    }
    if(L->active->Instruction.addr3 != NULL){
      printf(" %s\n", L->active->Instruction.addr3);
      listNext(L);
    }
    else{
      listNext(L);
      printf("\n");
      continue;
    }
  }
  listFree(L);
}