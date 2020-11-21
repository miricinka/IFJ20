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
funNode funTree; //globalna premenna pre strom na funkcie
int funParamCounter; //globalna premenna pre pocet paramterov //TODO
int funReturnCounter; //globalna premenna pre pocet navratovych hodnot //TODO
string funName; //globalna premenna pre predavanie nazvu funkcie //TODO
int globalScope = 1; //globalna premenna pre globalny scope
int token = 0; //globalna premenna na ukladanie tokenov

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
  //inicializujeme string na mena funkcii
  strInit(&funName); //TODO
  //inicializujem si string, premenne, zoznam
  strInit(&tokenStr);
  //inicializujeme strom na funkcie
  funInit(&funTree);
  int result = 0;
  list = instrList;
  //kontrola prveho lexemu ak to je v poriadku zavola sa prvy non terminal <program> a ten spracuje dalej
  token = get_new_token(&tokenStr);
  //ak bude prazdny subor hodi chybu
  if (token == ENDFILE)  errorMsg(ERR_SYNTAX, "Empty file");
  else
  {
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
      
      printFunTree(funTree);  
      //kontrola ci program obsahuje main
      string tempMain; strInit (&tempMain); strAddChars(&tempMain,"main");
      if(!funSearch(&funTree, tempMain)) {errorMsg(ERR_SEMANTIC_DEFINITION, "Program is missing \"main\" function");}
      //kontrola ci volane funkcie boli deklarovane 
      if(isFunCallDec(funTree) != 0){
			    errorMsg(ERR_SEMANTIC_DEFINITION, "Function was not defined");
		  }
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
    //pridanie mainu do stromu a vytvorenie pomocneho stringu na main
    string tempMain; strInit (&tempMain); strAddChars(&tempMain,"main");
    addFunToTree(&funTree, tempMain);
    addFunDec(&funTree, tempMain,  funParamCounter, funReturnCounter);
    strFree(&tempMain);
    token = get_new_token(&tokenStr);
    if (token != L_PAR) errorMsg(ERR_SYNTAX, "Wrong main func signature - missing '(' ");
    token = get_new_token(&tokenStr);
    if (token != R_PAR) errorMsg(ERR_SYNTAX, "Wrong main func signature - missing ')' ");
    token = get_new_token(&tokenStr);
    if (token != L_PAR && token != L_BR) errorMsg(ERR_SYNTAX, "Wrong main func signature - return params or '{' ");
    //optional return params in main function
    if (token == L_PAR)
    {
    token = get_new_token(&tokenStr);
    if (token != R_PAR) errorMsg(ERR_SYNTAX, "Wrong main func signature - missing ')' ");
    token = get_new_token(&tokenStr);
    }
    
    if (token != L_BR) errorMsg(ERR_SYNTAX, "Wrong main func signature - missing '{' ");
    //vytvorenie stromu pre funkciu na lokalne premenne
    varNode treePtr;
    //printf("treeptr num before init: %p\n", &treePtr);
    BSTInit (&treePtr);
    //printf("treeptr num after init: %p\n", &treePtr);
    //spracujeme stat_list a ak je v poriadku tak pokracujeme dalej
    //printf("treeptr num before stat_list call: %p\n", &treePtr);
    result = stat_list(&treePtr);
    if (result != 0) return result;
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) errorMsg(ERR_SYNTAX, "Wrong main func signature - missing '}' ");
    return result;
  }
  else if(token == ID) //<fun_def>	func	ID	(	<fun_params>	)	(	<fun_returns>	)	{	<stat_list>	}
  {
    //ulozenie nazvu funkcie do premennej 
    strClear(&funName); //TODO
    strCopyString(&funName, &tokenStr); //TODO
    //pridanie funkcie do stromu
    addFunToTree(&funTree, funName);
    token = get_new_token(&tokenStr);
    if (token != L_PAR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '(' ");  
    //vytvorenie stromu pre funkciu na lokalne premenne
    varNode treePtr;
    //printf("treeptr num before init: %p\n", &treePtr);
    BSTInit (&treePtr);
    //printf("treeptr num after init: %p\n", &treePtr);
    //spracujeme fun_params a ak je v poriadku tak pokracujeme dalej 
    //counter nastavime na 0
    funParamCounter = 0;
    result = fun_params(&treePtr);
    if (result != 0) return result;
    if (token != R_PAR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing ')' "); 
    token = get_new_token(&tokenStr);
    //definicia funkcie nemusi obsahovat zatvorku s navratovymi hodnotami
    if (token != L_PAR && token != L_BR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '(' or '{' ");
    if (token == L_PAR)
    {
      //if (token != L_PAR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '(' ");
      //spracujeme fun_returns a ak je v poriadku tak pokracujeme dalej
      //counter nastavime na 0
      funReturnCounter = 0;
      result = fun_returns();
      if (result != 0) return result;
      if (token != R_PAR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing ')' ");
      //pridane paratemetre a returny do funkcie
      token = get_new_token(&tokenStr);
    }
    addFunDec(&funTree, funName, funParamCounter, funReturnCounter);
    if (token != L_BR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '{' ");   
    //spracujeme stat_list a ak je v poriadku tak pokracujeme dalej 
    result = stat_list(&treePtr);
    if (result != 0) return result;
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '}' ");
    //Dispose tree for variables because they are only local for function
    BSTDispose(&treePtr);
  }
  //if we try to redefine functions of language GO => semantic error 3
  else if (token == F_INPUTS || token == F_INPUTI || token == F_INPUTF || token == F_PRINT || token == F_INT2FLOAT || token == F_FLOAT2INT || token == F_LEN || token == F_SUBSTR || token == F_ORD || token == F_CHR )
  {
    errorMsg(ERR_SEMANTIC_DEFINITION, "Redefinition of function");
  }
  else errorMsg(ERR_SYNTAX, "Function is missing ID");
  
  
  return result;
}

int stat_list(varNode * treePtr)
{
  //printf("treeptr num after stat_list call: %p\n", &treePtr);
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
    case F_INPUTF:
    case F_INPUTS:
    case F_INPUTI:
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

int stat(varNode * treePtr)
{
  int result = 0;
  prec_end_struct precResult;
  //<stat>	if	<exp>	{	<stat_list>	}	else	{	<stat_list>	}			
  if (token == KW_IF)
  {
    //scope pre if
    globalScope++;
    token = get_new_token(&tokenStr);
    if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) 
        errorMsg(ERR_SYNTAX, "Incorrect token after IF statement");
    //zavolame precedencku na vyraz, posleme tam token a jeho string
    precResult = prec_parse(treePtr, token, tokenStr);
    token = precResult.end_token;
    //v ife moze byt vyraz iba boolovskeh hodnoty inak nastava chyba 5
    if (precResult.end_datatype != TYPE_BOOL)
        errorMsg(ERR_SEMANTIC_COMPATIBILITY, "IF statement expression must be boolean");
    //token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati L_BR token 
    if (token != L_BR) errorMsg(ERR_SYNTAX, "IF statement - missing {");
    result = stat_list(treePtr);
    if (result != 0) return result;
    //koniec if scope
    globalScope--;
    BSTScopeDelete(treePtr,globalScope);
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) errorMsg(ERR_SYNTAX, "IF statement - missing }");
    token = get_new_token(&tokenStr);
    if (token != KW_ELSE) errorMsg(ERR_SYNTAX, "IF statement - missing 'ELSE'");
    //scope pre else
    globalScope++;
    token = get_new_token(&tokenStr);
    if (token != L_BR) errorMsg(ERR_SYNTAX, "IF statement - missing { in ELSE");
    token = get_new_token(&tokenStr);
    if (token != EOL) errorMsg(ERR_SYNTAX, "IF statement - no EOL after ELSE");
    result = stat_list(treePtr);
    if (result != 0) return result;
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) errorMsg(ERR_SYNTAX, "IF statement - missing }");
    //koniec else scope
    globalScope--;
    BSTScopeDelete(treePtr,globalScope);
    token = get_new_token(&tokenStr);
    if (token != EOL) errorMsg(ERR_SYNTAX, "IF statement - missing EOL");
  }
  //<stat>	for	<var_def>	;	<exp>	;	<ass_stat>	<opt_eol>	{	<stat_list>	}		
  else if (token == KW_FOR)
  {
    token = get_new_token(&tokenStr);
    //scope pre for hlavicku
    globalScope++;
    if (token != SEMICOL && token != ID) errorMsg(ERR_SYNTAX, "Incorrect token after FOR kw");
    if (token == ID) //moze byt aj premenna bez priradenia??????
    {
      //ulozime id premennej do stringID
      string stringID; strInit(&stringID); strCopyString(&stringID, &tokenStr);
      token = get_new_token(&tokenStr);
      if (token != VAR_DEF) errorMsg(ERR_SYNTAX, "FOR statement - must be var def");
      token = get_new_token(&tokenStr);
      if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) 
          errorMsg(ERR_SYNTAX, "FOR statement - incorrect var def");
      //zavolame precedencku na vyraz!!!
      precResult = prec_parse(treePtr, token, tokenStr);
      token = precResult.end_token; // asi bude treba kontrolovat typ, pravdepodobne moze prejst len INT
      if (precResult.end_datatype == TYPE_BOOL)
        errorMsg(ERR_SEMANTIC_COMPATIBILITY, "FOR statement - var def cannot be boolean");
      //token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token 
      if (token != SEMICOL) errorMsg(ERR_SYNTAX, "FOR statement - semicolon missing");
      // vlozime premennu do stromu
      BSTInsert(treePtr, stringID, precResult.end_datatype, globalScope);
      strFree(&stringID);
    }
    token = get_new_token(&tokenStr);
    if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) 
      errorMsg(ERR_SYNTAX, "FOR statement - incorrect expression");
    //zavolame precedencku na vyraz, posleme tam token a jeho string
    precResult = prec_parse(treePtr, token, tokenStr);
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
      if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) 
      errorMsg(ERR_SYNTAX, "FOR statement - incorrect expression");
      precResult = prec_parse(treePtr, token, tokenStr);
      token = precResult.end_token; // asi bude treba kontrolovat typ, pravdepodobne moze prejst len INT

      if (precResult.end_datatype == TYPE_BOOL) errorMsg(ERR_SEMANTIC_COMPATIBILITY, "FOR statement - assign can not be boolean");

      //token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token 
      if (token != L_BR) errorMsg(ERR_SYNTAX, "FOR statement - '{' missing");
    }
    //scope pre telo foru
    globalScope++;
    token = get_new_token(&tokenStr);
    if (token != EOL) errorMsg(ERR_SYNTAX, "FOR statement - EOL missing");
    result = stat_list(treePtr);
    if (result != 0) return result;
    //pravy bracket sme nacitali uz v stat_list
    if (token != R_BR) errorMsg(ERR_SYNTAX, "FOR statement - '}' missing");
    token = get_new_token(&tokenStr);
    if (token != EOL) errorMsg(ERR_SYNTAX, "FOR statement - EOL missing");
    //koniec oboch for scopeov
    globalScope = globalScope - 2;

    BSTScopeDelete(treePtr,globalScope);
  }
  //<stat>	<var_def>											
  //<stat>	<ass_stat>											
  //<stat>	<fun>											
  else if(token == ID)
  {
    //pozrieme do stromu ci tam id premennej je
    bool isIDDeclared = isDeclared(*treePtr, tokenStr);
    string stringID; strInit(&stringID); strCopyString(&stringID, &tokenStr);
   
    token = get_new_token(&tokenStr);
    if (token != VAR_DEF && token != COMMA && token != ASSIGN && token != L_PAR) 
      errorMsg(ERR_SYNTAX, "Incorrect statement - bad token after ID");
    //<var_def>	ID	:=	<exp>									
    if (token == VAR_DEF)
    {
      if (strcmp(stringID.str, "_") == 0){errorMsg(ERR_SEMANTIC_DEFINITION, "Can not declare '_'");}
      //kontrola ak je id uz deklarovane hodime chybu 3
      /*if (isIDDeclared == true)
      {
        errorMsg(ERR_SEMANTIC_DEFINITION, "ID is already declared");
      }*/
      token = get_new_token(&tokenStr);
      if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) 
        errorMsg(ERR_SYNTAX, "Incorrect statement - bad token after :=");          
      //zavolame precedencku na vyraz!!!
      precResult = prec_parse(treePtr, token, tokenStr);
      token = precResult.end_token; // asi bude treba kontrolovat typ, pravdepodobne moze prejst len INT
      //token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token 

      //vysledok precedencky nemoze byt BOOL
      if (precResult.end_datatype == TYPE_BOOL){errorMsg(ERR_SEMANTIC_DATATYPE, "ASSIGN statement - assign can not be boolean");}
      

      //deklaracia prebehla a ID a typ premennej ulozime do stromu
      BSTInsert(treePtr, stringID, precResult.end_datatype, globalScope);
      //printf("BST after inserting:\n");
      //kontrola ci deklaracia konci EOLom
      if (token != EOL) errorMsg(ERR_SYNTAX, "Incorrect statement declaration - missing EOL");
    }
    //<ass_stat>	<ass_ids>	=	<ass_exps> IBA PRE JEDNO PRIRADENIE								
    else if(token == ASSIGN)
    {
      //ak je ID '_' nemusime kontrolovat ci je v strome
      if(strcmp(stringID.str, "_") != 0)
      {
        //kontrola ci je v strome ID
        if (isIDDeclared == false)
        {
          errorMsg(ERR_SEMANTIC_DEFINITION, "ID is not declared");
        }
      }

      token = get_new_token(&tokenStr);
      if (token == F_INPUTS || token == F_INPUTI || token == F_INPUTF || token == F_INT2FLOAT || token == F_FLOAT2INT || token == F_LEN || token == F_SUBSTR || token == F_ORD || token == F_CHR )
      {
       //TODO 
      }
      if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) 
        errorMsg(ERR_SYNTAX, "Incorrect statement - bad token after =");
      //zavolame precedencku na vyraz!!!
      precResult = prec_parse(treePtr, token, tokenStr);
      token = precResult.end_token;
      if (token != EOL) errorMsg(ERR_SYNTAX, "Incorrect statement assign - missing EOL");
      //kontrola typov ci priradovany typ odpoveda typu premennej
      int variableType = getType(*treePtr,stringID);
      if (strcmp(stringID.str, "_") != 0)
      {
        if (variableType != precResult.end_datatype)
        {
          errorMsg(ERR_SEMANTIC_COMPATIBILITY, "Incorrect statement assign - wrong type assigment");
        }
        //BSTInsert(treePtr, stringID, precResult.end_datatype, globalScope);
      }
      //v pripade ze priradujeme do '_' tak nebudeme insertovat inak ano      
    }
    else if (token == COMMA)
    {
      return ass_stat(treePtr);
    }
    //<fun>	ID	(	<fun_call_param>	)												
    else if(token == L_PAR)
    {

      //ulozenie nazvu funkcie do premennej
      strClear(&funName); //TODO
      strCopyString(&funName, &stringID); //TODO
      funReturnCounter = 0; //TODO
      funParamCounter = 0; //TODO
      addFunToTree(&funTree, stringID);
      return fun_call_param(treePtr);
    }
  }
  //<stat>	return	<ass_exps>														
  else if(token == KW_RETURN)
  {
    return ass_exps(treePtr);
  }
  //sekcia vstavanych funkcii

  //input funkcie myslim nemozu byt ako samostatny prikaz
  else if (token == F_INPUTF)
  {
    errorMsg(ERR_SEMANTIC_OTHER, "Statement inputf can not be lonely on line, it will be sad");
    // token = get_new_token(&tokenStr);
    // if (token != L_PAR) errorMsg(ERR_SYNTAX, "INPUTF statement - '(' missing");
    // token = get_new_token(&tokenStr);
    // if (token != R_PAR) errorMsg(ERR_SYNTAX, "INPUTF statement - ')' missing");
    // token = get_new_token(&tokenStr);
    // if (token != EOL) errorMsg(ERR_SYNTAX, "INPUTF statement - EOL missing");
  }
  else if (token == F_INPUTI)
  {
    errorMsg(ERR_SEMANTIC_OTHER, "Statement inputi can not be lonely on line, it will be sad");
    // token = get_new_token(&tokenStr);
    // if (token != L_PAR) errorMsg(ERR_SYNTAX, "INPUTI statement - '(' missing");
    // token = get_new_token(&tokenStr);
    // if (token != R_PAR) errorMsg(ERR_SYNTAX, "INPUTI statement - ')' missing");
    // token = get_new_token(&tokenStr);
    // if (token != EOL) errorMsg(ERR_SYNTAX, "INPUTI statement - EOL missing");
  }
  else if (token == F_INPUTS)
  {
    errorMsg(ERR_SEMANTIC_OTHER, "Statement inputs can not be lonely on line, it will be sad");
    // token = get_new_token(&tokenStr);
    // if (token != L_PAR) errorMsg(ERR_SYNTAX, "INPUTS statement - '(' missing");
    // token = get_new_token(&tokenStr);
    // if (token != R_PAR) errorMsg(ERR_SYNTAX, "INPUTS statement - ')' missing");
    // token = get_new_token(&tokenStr);
    // if (token != EOL) errorMsg(ERR_SYNTAX, "INPUTS statement - EOL missing");
  }
  else if (token == F_FLOAT2INT)
  {
    errorMsg(ERR_SEMANTIC_OTHER, "Statement float2int can not be lonely on line, it will be sad");
  }
  else if (token == F_INT2FLOAT)
  {
    errorMsg(ERR_SEMANTIC_OTHER, "Statement int2float can not be lonely on line, it will be sad");
  }
  else if (token == F_LEN)
  {
    errorMsg(ERR_SEMANTIC_OTHER, "Statement len can not be lonely on line, it will be sad");
  }
  else if (token == F_ORD)
  {
    errorMsg(ERR_SEMANTIC_OTHER, "Statement ord can not be lonely on line, it will be sad");
  }
  else if (token == F_PRINT)
  {
    token = get_new_token(&tokenStr);
    if (token != L_PAR) errorMsg(ERR_SYNTAX, "PRINT statement - '(' missing");    
    result = print_params(treePtr);
    if (result != 0) return result;
    token = get_new_token(&tokenStr);
    if (token != EOL) errorMsg(ERR_SYNTAX, "PRINT statement - EOL missing");   
  }
  else if (token == F_SUBSTR)
  {
    errorMsg(ERR_SEMANTIC_OTHER, "Statement substr can not be lonely on line, it will be sad");
  }
  else if (token == F_CHR)
  {
    errorMsg(ERR_SEMANTIC_OTHER, "Statement chr can not be lonely on line, it will be sad");
  }

  return result;
}

//<ass_stat>	<ass_ids>	=	<ass_exps>													
int ass_stat(varNode * treePtr)
{
  int result = 0;
  token = get_new_token(&tokenStr);
  if (token != ID) errorMsg(ERR_SYNTAX, "ASSIGN statement - must be ID");
  token = get_new_token(&tokenStr);
  if (token != COMMA && token != ASSIGN) errorMsg(ERR_SYNTAX, "ASSIGN statement - token must be '=' or ',' for another ID");
  if (token == COMMA) return ass_stat(treePtr);
  return ass_exps(treePtr);
  return result;
}

//<ass_exps>	<exp>	<ass_exps>	
//<ass_exps>	,	<exp>	<ass_exps>
//<ass_exps>	<ass_ids>		
//<ass_exps>	<fun>		
int ass_exps(varNode * treePtr)
// toto sa bude menit ked pride ID opytame sa ci existuje taka funkcia v tabulke symbolov
//ak existuje spracuvame ako funkciu inak posielame precedencke
{
  int result = 0;
  prec_end_struct precResult;
  token = get_new_token(&tokenStr);
  //printf("token je %d\n", token);
  if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) errorMsg(ERR_SYNTAX, "Incorrect token after RETURN - must be ID, FLOAT, INT or STRING");
  precResult = prec_parse(treePtr, token, tokenStr);
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
  if (token == COMMA) return ass_exps(treePtr);
  return result;
}


//<fun_call_param>	ID	<fun_call_param>														
//<fun_call_param>	,	ID	<fun_call_param>													
//<fun_call_param>	e															
int fun_call_param(varNode *treePtr)
{
  int result = 0;
  //int multipleParams = 0;
  //if (token == COMMA) multipleParams = 1;
  token = get_new_token(&tokenStr);
  if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != R_PAR) {errorMsg(ERR_SYNTAX, "Incorrect token in func call parameters");}
  //funkcia je bez parametrov
  if (token == R_PAR) { return result; }
  if (token == ID) 
  {
  bool isIDDeclared = isDeclared(*treePtr, tokenStr);
  if (isIDDeclared == false)
  {
    errorMsg(ERR_SEMANTIC_DEFINITION, "ID not declared");
  }
  }
    //koniec spracovania parametrov
  /*if (token == R_PAR && multipleParams == 0)
  {
    printf("hello friend\n");
    printf("%d /\\ %d \n",funParamCounter, parCount(funTree, funName));
    if (funParamCounter != parCount(funTree, funName)) {errorMsg(ERR_SEMANTIC_PARAM, "Incorrect parameter count");} //TODO
    return result;
  } */
  
  //else if (token == R_PAR && multipleParams == 1) errorMsg(ERR_SYNTAX, "Incorrect token in func call parameters - missing param");
  //pocitame pocet parametrov
  funParamCounter++; //TODO
  if (token == ID)  //TODO
  { 
    addParam(&funTree, funName, getType(*treePtr, tokenStr), funParamCounter);
  }
  else
  {
    addParam(&funTree, funName, token, funParamCounter);
  }

  token = get_new_token(&tokenStr);
  if (token != COMMA && token != R_PAR) 
    errorMsg(ERR_SYNTAX, "Incorrect token in func call parameters");
  if (token == COMMA) {return fun_call_param(treePtr);}
  addFunCall(&funTree, funName, *treePtr, funParamCounter, funReturnCounter);   
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
int fun_params(varNode * treePtr)
{
  int result = 0;
  int multipleParams = 0;
  if (token == COMMA) multipleParams = 1;

  token = get_new_token(&tokenStr);
  if (token != ID && token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect params");
  //vyskocime z funkcie ak vsetko prebehlo spravne
  if (token == R_PAR && multipleParams == 0)
  {
    //TODO
    return result;
  } 
  else if (token == R_PAR && multipleParams == 1) errorMsg(ERR_SYNTAX, "Incorrect params");

  //pozrieme do stromu ci tam id premennej je
  bool isIDDeclared = isDeclared(*treePtr, tokenStr);
  string stringID; strInit(&stringID); strCopyString(&stringID, &tokenStr);
  if (strcmp(stringID.str, "_") == 0){errorMsg(ERR_SEMANTIC_DEFINITION, "Can not declare '_'");}
  //kontrola ak je id uz deklarovane hodime chybu 3
  if (isIDDeclared == true)
  {
    errorMsg(ERR_SEMANTIC_DEFINITION, "ID is already declared");
  }

  token = get_new_token(&tokenStr);
  if (token != KW_FLOAT64 && token != KW_INT && token != KW_STRING) errorMsg(ERR_SYNTAX, "Incorrect or missing param type");
  //kontrola spravnosti typu pri deklaraci premennej
  int tokenDataType = 0;
  if (token == KW_FLOAT64){tokenDataType = T_FLOAT;}
  else if(token == KW_INT){tokenDataType = T_INT;}
  else if(token == KW_STRING){tokenDataType = T_STRING;}
  else {errorMsg(ERR_SEMANTIC_DATATYPE, "ASSIGN statement - assign can not be boolean");}
  //pridanie parametrov do stromu
  funParamCounter++; //TODO
  addParam(&funTree, funName, tokenDataType, funParamCounter); //TODO
  //deklaracia prebehla a ID a typ premennej ulozime do stromu
  BSTInsert(treePtr, stringID, tokenDataType, globalScope);
  token = get_new_token(&tokenStr);
  if (token != COMMA && token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect params");
  if (token == COMMA) return fun_params(treePtr);
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

//fun_params for PRINT function
int print_params(varNode * treePtr)
{

  int result = 0;
  int multipleParams = 0;
  if (token == COMMA) multipleParams = 1;

  token = get_new_token(&tokenStr);
  if (token != ID && token != T_STRING && token != T_FLOAT && token != T_INT && token != R_PAR) errorMsg(ERR_SYNTAX, "PRINT function - Incorrect params");
  if (token == R_PAR && multipleParams == 0) return result;
  else if (token == R_PAR && multipleParams == 1) errorMsg(ERR_SYNTAX, "PRINT function - Incorrect params");

  //pozrieme do stromu ci tam id premennej je
  bool isIDDeclared = isDeclared(*treePtr, tokenStr);
  if (isIDDeclared != true  && token == ID)
  {
    errorMsg(ERR_SEMANTIC_DEFINITION, "ID is not declared");
  }
  token = get_new_token(&tokenStr);
  if (token != COMMA && token != R_PAR) errorMsg(ERR_SYNTAX, "PRINT function - Incorrect params");
  if (token == COMMA) return print_params(treePtr);
  return result;
}
