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
//<prog>	<opt_eol>	<prolog>  <fun_def_list>  <EOF>								
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
      return 0;

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

//<prolog>	package	main EOL									
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

//<fun_def_list>	<opt_eol>	<fun_def>	<fun_def_list>
int fun_def_list()
{
int result = 0;
token = get_new_token(&tokenStr);
switch (token)
  {
    case KW_FUNC:
      // nejprve zavolame funkci fun_def
      result = fun_def();      
      // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
      if (result != 0) return result;
      // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce fun_def_list
      return fun_def_list();

    case EOL:
    //prisiel EOL a rekurzivne zavolame fun_def_list kym nenajdeme funkciu alebo koniec suboru
      return fun_def_list();

    case ENDFILE:
      return result;
    break;
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return ERR_SYNTAX;
}

		
int fun_def()
{
  int result = 0;
  //rozhodneme ci je to main alebo ina funkcia
  token = get_new_token(&tokenStr);
  if (token == MAIN) //<fun_def>	func	main	(	)	{	<stat_list>	}		
  {
    token = get_new_token(&tokenStr);
    if (token != L_PAR) return ERR_SYNTAX;
    token = get_new_token(&tokenStr);
    if (token != R_PAR) return ERR_SYNTAX;
    token = get_new_token(&tokenStr);
    if (token != L_BR) return ERR_SYNTAX;
    //spracujeme stat_list a ak je v poriadku tak pokracujeme dalej
    result = stat_list();
    if (result != 0) return result;
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) return ERR_SYNTAX;
    return result;
  }
  else if(token == ID) //<fun_def>	func	ID	(	<fun_params>	)	(	<fun_returns>	)	{	<stat_list>	}
  {
    token = get_new_token(&tokenStr);
    if (token != L_PAR) return ERR_SYNTAX;  
    //spracujeme fun_params a ak je v poriadku tak pokracujeme dalej 
    fun_params();
    if (result != 0) return result;
    token = get_new_token(&tokenStr);
    if (token != R_PAR) return ERR_SYNTAX; 
    token = get_new_token(&tokenStr);
    if (token != L_PAR) return ERR_SYNTAX;
    //spracujeme fun_returns a ak je v poriadku tak pokracujeme dalej
    fun_returns();
    if (result != 0) return result;
    token = get_new_token(&tokenStr);
    if (token != R_PAR) return ERR_SYNTAX;
    token = get_new_token(&tokenStr);
    if (token != L_BR) return ERR_SYNTAX;   
    //spracujeme stat_list a ak je v poriadku tak pokracujeme dalej 
    stat_list();
    if (result != 0) return result;
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) return ERR_SYNTAX;
  }
  else return ERR_SYNTAX;
  
  
  return result;
}

int stat_list()
{
  int result = 0;
  token = get_new_token(&tokenStr);
switch (token)
  {
    //doplnit vstavane funkcie
    //ak je daky prikaz tak idem do funkcie stat a tam ich spracujem
    case ID:
    case KW_IF:    
    case KW_FOR:
    case KW_RETURN:    
    case F_PRINT:    
    case F_LEN:      
    case F_SUBSTR:   
    case F_ORD:      
    case F_CHR:      
    case F_INT2FLOAT:
    case F_FLOAT2INT:
      // nejprve zavolame funkci stat
      result = stat();      
      // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
      if (result != 0) return result;
      // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce stat_list
      return stat_list();

    case EOL:
    //prisiel EOL a rekurzivne zavolame stat_list kym nenajdeme funkciu alebo koniec suboru
      return stat_list();
    case R_BR:
      return result;
    break;
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return ERR_SYNTAX;
}

int stat()
{
  int result = 0;
  //<stat>	if	<exp>	{	<stat_list>	}	else	{	<stat_list>	}			
  if (token == KW_IF)
  {
    token = get_new_token(&tokenStr);
    if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID) return ERR_SYNTAX;
    //zavolame precedencku na vyraz!!!
    token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati L_BR token 
    if (token != L_BR) return ERR_SYNTAX;
    stat_list();
    if (result != 0) return result;
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) return ERR_SYNTAX;
    token = get_new_token(&tokenStr);
    if (token != KW_ELSE) return ERR_SYNTAX;
    token = get_new_token(&tokenStr);
    if (token != L_BR) return ERR_SYNTAX;
    token = get_new_token(&tokenStr);
    if (token != EOL) return ERR_SYNTAX;
        stat_list();
    if (result != 0) return result;
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) return ERR_SYNTAX;
    token = get_new_token(&tokenStr);
    if (token != EOL) return ERR_SYNTAX;
  }
  //<stat>	for	<var_def>	;	<exp>	;	<ass_stat>	<opt_eol>	{	<stat_list>	}		
  else if (token == KW_FOR)
  {
    token = get_new_token(&tokenStr);
    if (token != SEMICOL && token != ID) return ERR_SYNTAX;
    if (token == ID) //moze byt aj premenna bez priradenia??????
    {
      token = get_new_token(&tokenStr);
      if (token != VAR_DEF) return ERR_SYNTAX;
      token = get_new_token(&tokenStr);
      if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID) return ERR_SYNTAX;
      //zavolame precedencku na vyraz!!!
      token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token 
      if (token != SEMICOL) return ERR_SYNTAX;
    }
    token = get_new_token(&tokenStr);
    if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID) return ERR_SYNTAX;
    //zavolame precedencku na vyraz!!!
    token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token 
    if (token != SEMICOL) return ERR_SYNTAX;
    token = get_new_token(&tokenStr);
    if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_BR) return ERR_SYNTAX;
    if (token != L_BR)
    {
      //zavolame precedencku na vyraz!!!
      token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token 
      if (token != L_BR) return ERR_SYNTAX;
    }
    token = get_new_token(&tokenStr);
    if (token != EOL) return ERR_SYNTAX;
    stat_list();
    if (result != 0) return result;
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) return ERR_SYNTAX;
    token = get_new_token(&tokenStr);
    if (token != EOL) return ERR_SYNTAX;    
  }
  //<stat>	<var_def>											
  //<stat>	<ass_stat>											
  //<stat>	<fun>											
  else if(token == ID)
  {
    token = get_new_token(&tokenStr);
    if (token != VAR_DEF && token != COMMA && token != ASSIGN && token != L_PAR) return ERR_SYNTAX;
    //<var_def>	ID	:=	<exp>									
    if (token == VAR_DEF)
    {
      token = get_new_token(&tokenStr);
      if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID) return ERR_SYNTAX;
      //zavolame precedencku na vyraz!!!
      token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token 
      if (token != EOL) return ERR_SYNTAX;
    }
    //<ass_stat>	<ass_ids>	=	<ass_exps> IBA PRE JEDNO PRIRADENIE								
    else if(token == ASSIGN)
    {
      token = get_new_token(&tokenStr);
      if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID) return ERR_SYNTAX;
      //zavolame precedencku na vyraz!!!
      token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token 
      if (token != EOL) return ERR_SYNTAX;
    }
    /*else if(token == COMMA)
    {
      ass_stat();
    }*/
    else if(token == L_PAR)
    {
      return fun_call_param();
    }
  }
  else if(token == KW_RETURN)
  {
    return ass_exps();
  }

  return result;
}

/*int ass_stat()
{
  int result = 0;
  token = get_new_token(&tokenStr);
  if (token != ID) return ERR_SYNTAX;
  token = get_new_token(&tokenStr);
  if (token != COMMA && token != ASSIGN) return ERR_SYNTAX;
  if (token == COMMA) return ass_stat();
  if (token != ASSIGN) return ERR_SYNTAX;
  token = get_new_token(&tokenStr);
  if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID) return ERR_SYNTAX;
  ass_exps();
  return result;
}*/

int ass_exps()
{
  int result = 0;
  token = get_new_token(&tokenStr);
  if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID) return ERR_SYNTAX;
  //zavolame precedencku na vyraz!!!
  token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati COMMA alebo EOL token 
  if (token != COMMA && token != EOL) return ERR_SYNTAX;
  if (token == COMMA) return ass_exps();
  return result;
}

int fun_call_param()
{
  int result = 0;
  int multipleParams = 0;
  if (token == COMMA) multipleParams = 1;

  token = get_new_token(&tokenStr);
  if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != R_PAR) return ERR_SYNTAX;
  if (token == R_PAR && multipleParams == 0) return result;
  else if (token == R_PAR && multipleParams == 1) return ERR_SYNTAX;
  token = get_new_token(&tokenStr);
  if (token != COMMA && token != R_PAR) return ERR_SYNTAX;
  if (token == COMMA) return fun_call_param();
  return result;
}

int fun_params()
{
  int result = 0;
  return result;
}

int fun_returns()
{
  int result = 0;
  return result;
}
