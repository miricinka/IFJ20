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
  if (token == ENDFILE)  errorMsg(ERR_SYNTAX, "Empty file");
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
      if (token != EOL && token != KW_PACKAGE) errorMsg(ERR_SYNTAX, "Wrong header");
      return program();
    break;
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  errorMsg(ERR_SYNTAX, "Wrong header");
  return ERR_SYNTAX;
}

//<prolog>	package	main EOL									
int prolog()
{
  int result = 0;
  //kontrola ci je za package token main
  token = get_new_token(&tokenStr);
  if (token != MAIN) errorMsg(ERR_SYNTAX, "Wrong header - missing main");
  //kontrola ci je za main eol 
  token = get_new_token(&tokenStr);
  if (token != EOL) errorMsg(ERR_SYNTAX, "Wrong header - missing EOL");
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
  errorMsg(ERR_SYNTAX, "fun_def_list error - not a function");
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
    if (token != L_PAR) errorMsg(ERR_SYNTAX, "Wrong main func signature - missing '(' ");
    token = get_new_token(&tokenStr);
    if (token != R_PAR) errorMsg(ERR_SYNTAX, "Wrong main func signature - missing ')' ");
    token = get_new_token(&tokenStr);
    if (token != L_BR) errorMsg(ERR_SYNTAX, "Wrong main func signature - missing '{' ");
    //vytvorenie stromu pre funkciu na lokalne premenne
    Node treePtr;
    BSTInit (&treePtr);
    //spracujeme stat_list a ak je v poriadku tak pokracujeme dalej
    result = stat_list(treePtr);
    if (result != 0) return result;
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) errorMsg(ERR_SYNTAX, "Wrong main func signature - missing '}' ");
    return result;
  }
  else if(token == ID) //<fun_def>	func	ID	(	<fun_params>	)	(	<fun_returns>	)	{	<stat_list>	}
  {
    token = get_new_token(&tokenStr);
    if (token != L_PAR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '(' ");  
    //spracujeme fun_params a ak je v poriadku tak pokracujeme dalej 
    result = fun_params();
    if (result != 0) return result;
    if (token != R_PAR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing ')' "); 
    token = get_new_token(&tokenStr);
    if (token != L_PAR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '(' ");
    //spracujeme fun_returns a ak je v poriadku tak pokracujeme dalej
    result = fun_returns();
    if (result != 0) return result;
    if (token != R_PAR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing ')' ");
    token = get_new_token(&tokenStr);
    if (token != L_BR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '{' ");   
    //vytvorenie stromu pre funkciu na lokalne premenne
    Node treePtr;
    BSTInit (&treePtr);
    //spracujeme stat_list a ak je v poriadku tak pokracujeme dalej 
    result = stat_list(treePtr);
    if (result != 0) return result;
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '}' ");
  }
  else errorMsg(ERR_SYNTAX, "Function is missing ID");
  
  
  return result;
}

int stat_list(Node treePtr)
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
      result = stat(treePtr);      
      // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
      if (result != 0) return result;
      // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce stat_list
      return stat_list(treePtr);

    case EOL:
    //prisiel EOL a rekurzivne zavolame stat_list kym nenajdeme funkciu alebo koniec suboru
      return stat_list(treePtr);
    case R_BR:
      return result;
    break;
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  errorMsg(ERR_SYNTAX, "Incorrect stat - neither id, if, for nor function");
  return result;
}

int stat(Node treePtr)
{
  int result = 0;
  prec_end_struct precResult;
  //<stat>	if	<exp>	{	<stat_list>	}	else	{	<stat_list>	}			
  if (token == KW_IF)
  {
    token = get_new_token(&tokenStr);
    if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID) 
        errorMsg(ERR_SYNTAX, "Incorrect token after IF statement");
    //zavolame precedencku na vyraz, posleme tam token a jeho string
    precResult = prec_parse(token, tokenStr);
    token = precResult.end_token;
    //v ife moze byt vyraz iba boolovskeh hodnoty inak nastava chyba 5
    if (precResult.end_datatype != TYPE_BOOL)
        errorMsg(ERR_SEMANTIC_COMPATIBILITY, "IF statement expression must be boolean");
    //token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati L_BR token 
    if (token != L_BR) errorMsg(ERR_SYNTAX, "IF statement - missing {");
    stat_list(treePtr);
    if (result != 0) return result;
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) errorMsg(ERR_SYNTAX, "IF statement - missing }");
    token = get_new_token(&tokenStr);
    if (token != KW_ELSE) errorMsg(ERR_SYNTAX, "IF statement - missing 'ELSE'");
    token = get_new_token(&tokenStr);
    if (token != L_BR) errorMsg(ERR_SYNTAX, "IF statement - missing { in ELSE");
    token = get_new_token(&tokenStr);
    if (token != EOL) errorMsg(ERR_SYNTAX, "IF statement - no EOL after ELSE");
        stat_list(treePtr);
    if (result != 0) return result;
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) errorMsg(ERR_SYNTAX, "IF statement - missing } in ELSE");
    token = get_new_token(&tokenStr);
    if (token != EOL) errorMsg(ERR_SYNTAX, "IF statement - missing EOL");
  }
  //<stat>	for	<var_def>	;	<exp>	;	<ass_stat>	<opt_eol>	{	<stat_list>	}		
  else if (token == KW_FOR)
  {
    token = get_new_token(&tokenStr);
    if (token != SEMICOL && token != ID) errorMsg(ERR_SYNTAX, "Incorrect token after FOR kw");
    if (token == ID) //moze byt aj premenna bez priradenia??????
    {
      token = get_new_token(&tokenStr);
      if (token != VAR_DEF) errorMsg(ERR_SYNTAX, "FOR statement - must be var def");
      token = get_new_token(&tokenStr);
      if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID) 
          errorMsg(ERR_SYNTAX, "FOR statement - incorrect var def");
      //zavolame precedencku na vyraz!!!
      precResult = prec_parse(token, tokenStr);
      token = precResult.end_token; // asi bude treba kontrolovat typ, pravdepodobne moze prejst len INT
      if (precResult.end_datatype == TYPE_BOOL)
        errorMsg(ERR_SEMANTIC_COMPATIBILITY, "FOR statement - var def cannot be boolean");
      //token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token 
      if (token != SEMICOL) errorMsg(ERR_SYNTAX, "FOR statement - semicolon missing");
    }
    token = get_new_token(&tokenStr);
    if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID) 
      errorMsg(ERR_SYNTAX, "FOR statement - incorrect expression");
    //zavolame precedencku na vyraz, posleme tam token a jeho string
    precResult = prec_parse(token, tokenStr);
    token = precResult.end_token;
    //v ife moze byt vyraz iba boolovskeh hodnoty inak nastava chyba 5
    if (precResult.end_datatype != TYPE_BOOL)
      errorMsg(ERR_SEMANTIC_COMPATIBILITY, "FOR statement - expression must be boolean");
    //token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token 
    if (token != SEMICOL) errorMsg(ERR_SYNTAX, "FOR statement - semicolon missing");
    token = get_new_token(&tokenStr);
    if (token != ID && token != L_BR) errorMsg(ERR_SYNTAX, "FOR statement - '{' missing");
    if (token == ID)
    {
      //zavolame precedencku na vyraz!!!
      token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token 
      if (token != ASSIGN) errorMsg(ERR_SYNTAX, "FOR statement - must be assign statement");
      token = get_new_token(&tokenStr);
      precResult = prec_parse(token, tokenStr);
      token = precResult.end_token; // asi bude treba kontrolovat typ, pravdepodobne moze prejst len INT
      if (precResult.end_datatype == TYPE_BOOL)
        errorMsg(ERR_SEMANTIC_COMPATIBILITY, "FOR statement - assign can not be boolean");
      //token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token 
      if (token != L_BR) errorMsg(ERR_SYNTAX, "FOR statement - '{' missing");
    }
    token = get_new_token(&tokenStr);
    if (token != EOL) errorMsg(ERR_SYNTAX, "FOR statement - EOL missing");
    stat_list(treePtr);
    if (result != 0) return result;
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) errorMsg(ERR_SYNTAX, "FOR statement - '}' missing");
    token = get_new_token(&tokenStr);
    if (token != EOL) errorMsg(ERR_SYNTAX, "FOR statement - EOL missing");    
  }
  //<stat>	<var_def>											
  //<stat>	<ass_stat>											
  //<stat>	<fun>											
  else if(token == ID)
  {
    bool isIDDeclared = isDeclared(treePtr, tokenStr);
    string stringID; strInit(&stringID); strCopyString(&stringID, &tokenStr);
    if (isIDDeclared == true)
    {
      printf("deklarovane\n");
    }
    else
    {
      printf("nedeklarovane\n");
    }
    
    token = get_new_token(&tokenStr);
    if (token != VAR_DEF && token != COMMA && token != ASSIGN && token != L_PAR) 
      errorMsg(ERR_SYNTAX, "Incorrect statement - bad token after ID");
    //<var_def>	ID	:=	<exp>									
    if (token == VAR_DEF)
    {
      if (isIDDeclared == true)
      {
        errorMsg(ERR_SEMANTIC_DEFINITION, "ID is already declared");
      }
      token = get_new_token(&tokenStr);
      if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID) 
        errorMsg(ERR_SYNTAX, "Incorrect statement - bad token after :=");       
      printf("%d\n",token);  
      printf("%s\n",tokenStr.str);          
      //zavolame precedencku na vyraz!!!
      precResult = prec_parse(token, tokenStr);
      token = precResult.end_token; // asi bude treba kontrolovat typ, pravdepodobne moze prejst len INT
      //token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token 
      BSTInsert(&treePtr, stringID, stringID, precResult.end_datatype);

      if (token != EOL) errorMsg(ERR_SYNTAX, "Incorrect statement declaration - missing EOL");
    }
    //<ass_stat>	<ass_ids>	=	<ass_exps> IBA PRE JEDNO PRIRADENIE								
    else if(token == ASSIGN)
    {
      token = get_new_token(&tokenStr);
      if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID) 
        errorMsg(ERR_SYNTAX, "Incorrect statement - bad token after =");
      //zavolame precedencku na vyraz!!!
      token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token
      if (token != EOL) errorMsg(ERR_SYNTAX, "Incorrect statement assign - missing EOL");
    }
    else if (token == COMMA)
    {
      return ass_stat();
    }
    //<fun>	ID	(	<fun_call_param>	)												
    else if(token == L_PAR)
    {
      return fun_call_param();
    }
  }
  //<stat>	return	<ass_exps>														
  else if(token == KW_RETURN)
  {
    return ass_exps();
  }

  return result;
}

//<ass_stat>	<ass_ids>	=	<ass_exps>													
int ass_stat()
{
  int result = 0;
  token = get_new_token(&tokenStr);
  if (token != ID) errorMsg(ERR_SYNTAX, "ASSIGN statement - must be ID");
  token = get_new_token(&tokenStr);
  if (token != COMMA && token != ASSIGN) errorMsg(ERR_SYNTAX, "ASSIGN statement - token must be '=' or ',' for another ID");
  if (token == COMMA) return ass_stat();
  return ass_exps();
  return result;
}

//<ass_exps>	<exp>	<ass_exps>	
//<ass_exps>	,	<exp>	<ass_exps>
//<ass_exps>	<ass_ids>		
//<ass_exps>	<fun>		
int ass_exps()
// toto sa bude menit ked pride ID opytame sa ci existuje taka funkcia v tabulke symbolov
//ak existuje spracuvame ako funkciu inak posielame precedencke
{
  int result = 0;
  prec_end_struct precResult;
  token = get_new_token(&tokenStr);
  //printf("token je %d\n", token);
  if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID) errorMsg(ERR_SYNTAX, "Incorrect token after RETURN - must be ID, FLOAT, INT or STRING");
  precResult = prec_parse(token, tokenStr);
  token = precResult.end_token; // asi bude treba kontrolovat typ, pravdepodobne moze prejst len INT

  //kontrola toho ci spracovavame funkciu!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  /*if (token == L_PAR)
  {
    result = fun_call_param();
    if (result != 0) return result;
    if (token != R_PAR) errorMsg(ERR_SYNTAX, "FUNCTION CALL statement - ')' missing");
    token = get_new_token(&tokenStr);
  }*/

  if (precResult.end_datatype == TYPE_BOOL) errorMsg(ERR_SEMANTIC_COMPATIBILITY, "RETURN statement - return type can't be BOOL");
  //token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token 
  if (token != COMMA && token != EOL) errorMsg(ERR_SYNTAX, "RETURN statement - ',' or EOL missing");
  if (token == COMMA) return ass_exps();
  return result;
}


//<fun_call_param>	ID	<fun_call_param>														
//<fun_call_param>	,	ID	<fun_call_param>													
//<fun_call_param>	e															
int fun_call_param()
{
  int result = 0;
  int multipleParams = 0;
  if (token == COMMA) multipleParams = 1;

  token = get_new_token(&tokenStr);
  if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != R_PAR) 
    errorMsg(ERR_SYNTAX, "Incorrect token in func call parameters");
  if (token == R_PAR && multipleParams == 0) return result;
  else if (token == R_PAR && multipleParams == 1) 
    errorMsg(ERR_SYNTAX, "Incorrect token in func call parameters - missing param");
  token = get_new_token(&tokenStr);
  if (token != COMMA && token != R_PAR) 
    errorMsg(ERR_SYNTAX, "Incorrect token in func call parameters");
  if (token == COMMA) return fun_call_param();
  return result;
}
//<fun_params>	<par>	<par_next>														
//<fun_params>	e															
//<par_next>	,	<par>	<par_next>													
//<par_next>	e															
//<par>	ID	<type>														
//<type>	FLOAT64															
//<type>	INT															
//<type>	STRING															
int fun_params()
{
  int result = 0;
  int multipleParams = 0;
  if (token == COMMA) multipleParams = 1;

  token = get_new_token(&tokenStr);
  if (token != ID && token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect params");
  if (token == R_PAR && multipleParams == 0) return result;
  else if (token == R_PAR && multipleParams == 1) errorMsg(ERR_SYNTAX, "Incorrect params");
  token = get_new_token(&tokenStr);
  if (token != KW_FLOAT64 && token != KW_INT && token != KW_STRING) errorMsg(ERR_SYNTAX, "Incorrect or missing param type");
  token = get_new_token(&tokenStr);
  if (token != COMMA && token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect params");
  if (token == COMMA) return fun_params();
  return result;
}
//<fun_returns>	<ret>	<ret_next>														
//<fun_returns>	e	 														
//<ret_next>	, 	<ret>	<ret_next>													
//<ret_next>	e	
//<ret>	<type>															
int fun_returns()
{
  int result = 0;
  int multipleParams = 0;
  if (token == COMMA) multipleParams = 1;

  token = get_new_token(&tokenStr);
  if (token != KW_FLOAT64 && token != KW_INT && token != KW_STRING && token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect return params");
  if (token == R_PAR && multipleParams == 0) return result;
  else if (token == R_PAR && multipleParams == 1) errorMsg(ERR_SYNTAX, "Incorrect return params");
  token = get_new_token(&tokenStr);
  if (token != COMMA && token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect return params");
  if (token == COMMA) return fun_returns();
  return result;
}
