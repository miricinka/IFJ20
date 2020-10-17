/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  parser.c
 * 
 * DESCRIPTION:
 * Parser file
 * 
 * AUTHORS:
 *  Tomáš Hrúz          <xhruzt00@stud.fit.vutbr.cz>
 *  Kolaříková Mirka    <xkolar76@stud.fit.vutbr.cz>
 *  Aleš Řezáč          <xrezac21@stud.fit.vutbr.cz>
 *  Žovinec Martin      <xzovin00@stud.fit.vutbr.cz>
*/
#include "parser.h"

tListOfInstr *list; // globalni promenna uchovavajici seznam instrukci
string tokenStr; //globalna premenna pre string instrukcie


int token = 0;

void generateInstruction(int instType)
// vlozi novou instrukci do seznamu instrukci
{
   tInstr I;
   I.instType = instType;
   //I.addr1 = addr1;
   //I.addr2 = addr2;
   //I.addr3 = addr3;
   listInsertLast(list, I);
}

int parse(tListOfInstr *instrList)
{
  //inicializujem si string, premenne, zoznam
  strInit(&tokenStr);
  int result = 0;
  list = instrList;
  //kontrola prveho lexemu ak to je v poriadku zavola sa prvy non terminal <program> a ten spracuje dalej
  token = get_new_token(&tokenStr);
  //ak bude prazdny subor hodi chybu
  if (token == ENDFILE)  result = ERR_SYNTAX;
  else
  {
  //generateInstruction(token);
  result = program();
  }
  
     
  strFree(&tokenStr);
  return result;	
}
//<prog>	<opt_eol>,	<prolog>,  <fun_def_list>,  <EOF>								
int program()
{
  int result = 0;
  switch (token)
  {
    case KW_PACKAGE:
      // nejprve zavolame funkci prolog
      result = prolog();      
      // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
      if (result != 0) return result;
      // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce fun_def_list
      return fun_def_list();

    case EOL:
    //prisiel EOL pytame si dalsi token a musi to byt EOL alebo package a rekurzivne
    //zavolame program a ak bol dalsi token package vybavi sa to v druhom case ak eol opakujeme
      token = get_new_token(&tokenStr);
      if (token != EOL && token != KW_PACKAGE) return ERR_SYNTAX;
      return program();
    break;
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return ERR_SYNTAX;
}

//<prolog>	package,	main, 	EOL									
int prolog()
{
  int result = 0;
  //kontrola ci je za package token main
  token = get_new_token(&tokenStr);
  if (token != MAIN) return ERR_SYNTAX;
  //kontrola ci je za main eol 
  token = get_new_token(&tokenStr);
  if (token != EOL) return ERR_SYNTAX;
  return result;
}

int fun_def_list()
{
int result = 0;
printf("hi\n");
return result;
}
