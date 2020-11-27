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
 *  Tomáš Hrúz  <xhruzt00@stud.fit.vutbr.cz> 
*/
#include "parser.h"
//tokens
string tokenStr; //variable for string of token
int token = 0;   //variable for tokens
//trees
funNode funTree;      //variable for tree of functions
int funParamCounter;  //variable for number of parameters
int funReturnCounter; //variable for number of returns
string funName;       //variable for name of function in tree
string funDeclaredName; //variable for name of currently defined function
int levelOfScope = 1; //variabel for scope, it is incremented
//generation
tListOfInstr *list; //variable for list of instructions
//return checker
int returnCalled = 0; //checks if return is called in function with returns
bool mainCheck = false; //checks if main function is in program


/**
 * @brief Start of parsing, initialization of structures
 * 
 * @param instrList list of instructions for generation
 */
int parse(tListOfInstr *instrList)
{
        //initialization of string for names of functions
        strInit(&funName);
        //initialization of string for already declared function
        strInit(&funDeclaredName);
        //initialization of string for current token
        strInit(&tokenStr);
        //initialization of function tree
        funInit(&funTree);
        //after result is zero after this function it bubbles to main as sucesfull analysis of code
        //this applies to whole parser
        int result = 0;

        //initialize list of instructions
        list = instrList;
        listInit(list);

        //loads next token
        token = get_new_token(&tokenStr);
        //if token is end of file => error Empty File
        if (token == ENDFILE) errorMsg(ERR_SYNTAX, "Empty file");
        //starting analysis with first non-term
        else result = program();

        //free of strings
        strFree(&tokenStr);
        strFree(&funName);
        strFree(&funDeclaredName);
        //bubble zero(success) to upper rule of LL grammar
        return result;
}


/**
 * @brief <prog> <opt_eol> <prolog> <fun_def_list> <EOF>.
 * First rule of LL grammar it will continuously dive deeper in LL grammar and than bubble back to main
 */ 
int program()
{
        int result = 0;
        switch (token)
        {
        case KW_PACKAGE: //token must be package
                //handling prolog non-term, return is assigned to result because analysis continues
                //further in program if it is handling last non-term it goes straight to return which bubbles in main
                result = prolog();
                //if error occured in analysis program will bubble to main with right error code and exit
                if (result != 0) return result;
                //prolog analysis passed correctly and continues to fun_def_list analysis
                return fun_def_list();
                //bubble zero(success) to upper rule of LL grammar
                return result;

        case EOL: //before package there can be EOLs so recursivly check if package token already arrived
                token = get_new_token(&tokenStr);
                if (token != EOL && token != KW_PACKAGE) errorMsg(ERR_SYNTAX, "Wrong header");
                return program();
                break;
        }
        // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
        errorMsg(ERR_SYNTAX, "Wrong header");
        return ERR_SYNTAX;
}


/**
 * @brief package main EOL.
 * First line of program must be like this rule of LL-grammar
 */
int prolog()
{
        int result = 0;
        //main token
        token = get_new_token(&tokenStr);
        if (token != MAIN) errorMsg(ERR_SYNTAX, "Wrong header - missing main");
        //EOL token
        token = get_new_token(&tokenStr);
        if (token != EOL) errorMsg(ERR_SYNTAX, "Wrong header - missing EOL");
        //first line of program successful
        genFileHead();
        return result;
}


/**
 * @brief <fun_def_list> <opt_eol> <fun_def> <fun_def_list>.
 * Rule for list of function definitions
 */
int fun_def_list()
{
        int result = 0;
        //because in case it's apperantly wrong

        token = get_new_token(&tokenStr);       
        switch (token)
        {
        case KW_FUNC: //starts <fun_def> non-term analysis
                result = fun_def();
                if (result != 0) return result;
                //checks if function with returns has called return
                if (returnCalled == 1) errorMsg(ERR_SEMANTIC_PARAM, "Function is missing return");
                return fun_def_list();
        case EOL: //recursively chceck for optional EOLs
                return fun_def_list();
        case ENDFILE: //end of program
                //printFunTree(funTree);               
                if (mainCheck == false) errorMsg(ERR_SEMANTIC_DEFINITION, "Program is missing \"main\" function");
                //check in tree if called functions were declared
                isFunCallDec(funTree);

                printList(list);
                return result;
                break;
        }
        //other than tokens in switch causes syntax error
        errorMsg(ERR_SYNTAX, "Functions error - not a function");
        return ERR_SYNTAX;
}


/**
 * @brief <fun_def> func ID ( <fun_params> ) ( <fun_returns> ) { <stat_list> }					
 * Rule for function definition
 */
int fun_def()
{
        int result = 0;
        //main handles separately
        token = get_new_token(&tokenStr);
        if (token == MAIN) //<fun_def> func main ( ) { <stat_list> }
        {
                //add main function to tree and create main string
                string tempMain;
                strInit(&tempMain);
                strAddChars(&tempMain, "main");
                addFunToTree(&funTree, tempMain);
                //set counters to zero
                funParamCounter = 0;
                funReturnCounter = 0;
                addFunDec(&funTree, tempMain, funParamCounter/*, funReturnCounter*/);
                strFree(&tempMain);

                mainCheck = true;
                //left param token
                token = get_new_token(&tokenStr);
                if (token != L_PAR) errorMsg(ERR_SYNTAX, "Wrong main func signature - missing '(' ");
                //right param token if it is something else it is error 6 because main can not contain parameter
                token = get_new_token(&tokenStr);
                if (token != R_PAR) errorMsg(ERR_SEMANTIC_PARAM, "Main functions can not contain parameter");
                //left param for returns is optional, token could be left bracket
                token = get_new_token(&tokenStr);
                if (token != L_PAR && token != L_BR) errorMsg(ERR_SYNTAX, "Wrong main func signature - return params or '{' ");
                //optional right return params in main function
                if (token == L_PAR)
                {
                        token = get_new_token(&tokenStr);
                        if (token != R_PAR) errorMsg(ERR_SEMANTIC_PARAM, "Main functions can not contain return value");
                        //load token for left bracket
                        token = get_new_token(&tokenStr);
                }
                //left bracket token
                if (token != L_BR) errorMsg(ERR_SYNTAX, "Wrong main func signature - missing '{' ");

                genMainFunc();

                //create variable tree for main functions
                varNode treePtr;
                BSTInit(&treePtr);
                //handle stat_list rule
                result = stat_list(&treePtr);
                if (result != 0) return result;

                //right bracket token, this is end of main function
                if (token != R_BR) errorMsg(ERR_SYNTAX, "Wrong main func signature - missing '}' ");

                genMainEnd();
                //main function successful
                return result;
        }
        else if (token == ID) //<fun_def> func ID ( <fun_params> ) ( <fun_returns> ) { <stat_list> }
        {
                //save function name
                strClear(&funName);
                strCopyString(&funName, &tokenStr); 
                //add function to tree
                addFunToTree(&funTree, funName);

                //check for function redeclaration
                funDecCheck(&funTree, funName);

                //save function name for returns
                strClear(&funDeclaredName);
                strCopyString(&funDeclaredName, &tokenStr); 

                //left param token
                token = get_new_token(&tokenStr);
                if (token != L_PAR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '(' ");

                //generate function header
                genFuncHead(funName.str);

                //create tree for variables in function
                varNode treePtr;
                BSTInit(&treePtr);
                //counter for number of parameters in function
                funParamCounter = 0;
                funReturnCounter = 0;
                //handle fun_params rule
                result = fun_params(&treePtr);
                if (result != 0) return result;

                //right param token
                if (token != R_PAR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing ')' ");
                token = get_new_token(&tokenStr);
                //definition of function does not need return value params
                if (token != L_PAR && token != L_BR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '(' or '{' ");
                if (token == L_PAR)
                {
                        //counter for number of returns in function
                        funReturnCounter = 0;
                        //hande fun_returns rule
                        result = fun_returns();
                        if (result != 0) return result;
                        //check number of returns
                        funReturnCheck(&funTree,funName, funReturnCounter);
                        
                        //function must call return in body if returncounter is not zero
                        if (funReturnCounter != 0) {returnCalled = 1;}

                        //right param token, it was already loaded in fun returns
                        if (token != R_PAR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing ')' ");
                        //parameters and returns were added to functions in fun_params and fun_returns rules
                        token = get_new_token(&tokenStr);
                }
                //number of parameters and returns added to function in tree
                addFunDec(&funTree, funName, funParamCounter/*, funReturnCounter*/);
                
                //left bracket token
                if (token != L_BR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '{' ");               

                //handle stat_list rule
                result = stat_list(&treePtr);
                if (result != 0) return result;

                //right bracket token, already loaded in stat_list
                if (token != R_BR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '}' ");

                //generate end of definition of fucntion
                genFuncEnd(funName.str);

                //Dispose tree for variables because they are only local for function
                BSTDispose(&treePtr);
        }
        //if we try to redefine functions of language GO => semantic error 3
        else if (  token == F_INPUTS 
                || token == F_INPUTI 
                || token == F_INPUTF 
                || token == F_PRINT 
                || token == F_INT2FLOAT 
                || token == F_FLOAT2INT 
                || token == F_LEN 
                || token == F_SUBSTR 
                || token == F_ORD 
                || token == F_CHR)
        { errorMsg(ERR_SEMANTIC_DEFINITION, "Redefinition of function"); }
        else errorMsg(ERR_SYNTAX, "Function is missing ID");

        //successfuly handled definition of function
        return result;
}


/**
 * @brief <stat_list> <opt_eol> <stat> EOL <stat_list>. 
 * Rule for list of statements in function
 *
 * @param treePtr tree for variables 
 */
int stat_list(varNode *treePtr)
{
        int result = 0;

        //token must be statement
        token = get_new_token(&tokenStr);
        switch (token)
        {
        //if token is statement handle stat rule
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
        //if statement was handled successfuly recursively call stat_list to handle another statement
                result = stat(treePtr);
                if (result != 0) return result;

                return stat_list(treePtr);

        case EOL: //recursivly call stat_list because of optional EOLs
                return stat_list(treePtr);
        case R_BR: //end of function
                return result;
                break;
        }
        //if something else it is not statement
        errorMsg(ERR_SYNTAX, "Incorrect stat - neither id, if, for nor function");
        //successfuly handled all statements in function
        return result;
}


/**
 * @brief Statement rules
 *
 * @param treePtr tree for variables
 */
int stat(varNode *treePtr)
{
        int result = 0;
        prec_end_struct precResult;

        if (token == KW_IF) //<stat> if <exp> { <stat_list> } else { <stat_list> }
        {
                //increment level of scope
                levelOfScope++;

                //only tokens accepted in if condition
                token = get_new_token(&tokenStr);
                if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) errorMsg(ERR_SYNTAX, "Incorrect token after IF statement");

                //call precedence parser with tree of variables, current token, and string of token
                precResult = prec_parse(treePtr, token, tokenStr);
                //token loaded last first token after expression
                token = precResult.end_token;
                //if can have only bool value in condition
                if (precResult.end_datatype != TYPE_BOOL) errorMsg(ERR_SEMANTIC_COMPATIBILITY, "IF statement expression must be boolean");
                //left bracket was loaded in precedence parser
                if (token != L_BR) errorMsg(ERR_SYNTAX, "IF statement - missing {");

                //handle stat_list rule
                result = stat_list(treePtr);
                if (result != 0) return result;

                //end of scope
                levelOfScope--;
                BSTScopeDelete(treePtr, levelOfScope);

                //right bracket token after all statements in if
                if (token != R_BR) errorMsg(ERR_SYNTAX, "IF statement - missing }");
                //else token
                token = get_new_token(&tokenStr);
                if (token != KW_ELSE)errorMsg(ERR_SYNTAX, "IF statement - missing 'ELSE'");

                //scope for else
                levelOfScope++;
                //left bracket token
                token = get_new_token(&tokenStr);
                if (token != L_BR) errorMsg(ERR_SYNTAX, "IF statement - missing { in ELSE");
                token = get_new_token(&tokenStr);
                //EOL token
                if (token != EOL) errorMsg(ERR_SYNTAX, "IF statement - no EOL after ELSE");

                //handle stat_list in else
                result = stat_list(treePtr);
                if (result != 0) return result;
                //right bracket loaded in stat_list
                if (token != R_BR) errorMsg(ERR_SYNTAX, "IF statement - missing }");

                //end of else scope
                levelOfScope--;
                BSTScopeDelete(treePtr, levelOfScope);
                //end of If Else statement
                token = get_new_token(&tokenStr);
                if (token != EOL) errorMsg(ERR_SYNTAX, "IF statement - missing EOL");
        }
        else if (token == KW_FOR) //<stat> for <var_def> ; <exp> ; <ass_stat> <opt_eol> { <stat_list> }
        {
                //scope in for header
                levelOfScope++;

                //semicol if first part of for header is missing or id which will go to precedence parser if not missing
                token = get_new_token(&tokenStr);
                if (token != SEMICOL && token != ID) errorMsg(ERR_SYNTAX, "Incorrect token after FOR kw");

                if (token == ID) //first part of header is present
                {
                        //save id of variable to stringID
                        string stringID;
                        strInit(&stringID);
                        strCopyString(&stringID, &tokenStr);

                        //token for variable definition
                        token = get_new_token(&tokenStr);
                        if (token != VAR_DEF) errorMsg(ERR_SYNTAX, "FOR statement - must be var def");
                        //token for precedence parser
                        token = get_new_token(&tokenStr);
                        if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) errorMsg(ERR_SYNTAX, "FOR statement - incorrect var def");

                        //call precedence parser
                        precResult = prec_parse(treePtr, token, tokenStr);
                        token = precResult.end_token;
                        //this is not conditional expression
                        if (precResult.end_datatype == TYPE_BOOL) errorMsg(ERR_SEMANTIC_COMPATIBILITY, "FOR statement - var def cannot be boolean");
                        //token loaded in precedence parser
                        if (token != SEMICOL) errorMsg(ERR_SYNTAX, "FOR statement - semicolon missing");

                        //add variable to tree
                        BSTInsert(treePtr, stringID, precResult.end_datatype, levelOfScope);
                        strFree(&stringID);
                }
                //second part of for header is condition and it is not optional
                //token for precedence parser
                token = get_new_token(&tokenStr);
                if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) errorMsg(ERR_SYNTAX, "FOR statement - incorrect expression");

                //call precedence parser
                precResult = prec_parse(treePtr, token, tokenStr);
                token = precResult.end_token;
                //conditional expression, must be bool
                if (precResult.end_datatype != TYPE_BOOL) errorMsg(ERR_SEMANTIC_COMPATIBILITY, "FOR statement - expression must be boolean");

                //token loaded in precedence parser
                if (token != SEMICOL) errorMsg(ERR_SYNTAX, "FOR statement - semicolon missing");

                //token for third part of for header or left bracket to star for body
                token = get_new_token(&tokenStr);
                if (token != ID && token != L_BR) errorMsg(ERR_SYNTAX, "FOR statement - '{' missing");

                if (token == ID) //third part of for header
                {
                        //assign token
                        token = get_new_token(&tokenStr);
                        if (token != ASSIGN) errorMsg(ERR_SYNTAX, "FOR statement - must be assign statement");
                        //token for precedence parser
                        token = get_new_token(&tokenStr);
                        if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) errorMsg(ERR_SYNTAX, "FOR statement - incorrect expression");

                        //call precedence parser
                        precResult = prec_parse(treePtr, token, tokenStr);
                        token = precResult.end_token;
                        //expression is not conditional so it can not be bool
                        if (precResult.end_datatype == TYPE_BOOL) errorMsg(ERR_SEMANTIC_COMPATIBILITY, "FOR statement - assign can not be boolean");

                        //left bracket token loaded in precedence parser
                        if (token != L_BR) errorMsg(ERR_SYNTAX, "FOR statement - '{' missing");
                }
                //scope in for body
                levelOfScope++;

                //EOL token
                token = get_new_token(&tokenStr);
                if (token != EOL) errorMsg(ERR_SYNTAX, "FOR statement - EOL missing");

                //handle stat_list
                result = stat_list(treePtr);
                if (result != 0) return result;

                //right bracket loaded in prec parser
                if (token != R_BR) errorMsg(ERR_SYNTAX, "FOR statement - '}' missing");
                //EOL token
                token = get_new_token(&tokenStr);
                if (token != EOL) errorMsg(ERR_SYNTAX, "FOR statement - EOL missing");

                //end of both scopes: for header, for body
                levelOfScope = levelOfScope - 2;
                BSTScopeDelete(treePtr, levelOfScope);
        }
        // these stats could be present if token is ID
        //<stat>	<var_def>
        //<stat>	<ass_stat>
        //<stat>	<fun>
        else if (token == ID)
        {
                //check in tree if variable is declared and store in for further use
                //save string of id for further use
                bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                string stringID;
                strInit(&stringID);
                strCopyString(&stringID, &tokenStr);

                //var_def token for definition of variable
                //comma token for more ids
                //assign token for assign value to already decfined variable
                //left param token for function
                token = get_new_token(&tokenStr);
                if (token != VAR_DEF && token != COMMA && token != ASSIGN && token != L_PAR) errorMsg(ERR_SYNTAX, "Incorrect statement - bad token after ID");


                if (token == VAR_DEF) //<var_def> ID := <exp>
                {
                        //if id on left side of definition was "_" it cannot be defined
                        if (strcmp(stringID.str, "_") == 0) { errorMsg(ERR_SEMANTIC_DEFINITION, "Can not declare '_'"); }

                        genDefvar(stringID.str);

                        //token for precedence parser
                        token = get_new_token(&tokenStr);
                        if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) errorMsg(ERR_SYNTAX, "Incorrect statement - bad token after :=");
                        
                        //call precedence parser
                        precResult = prec_parse(treePtr, token, tokenStr);
                        token = precResult.end_token;
                        //expression is no conditional so it can not be bool
                        if (precResult.end_datatype == TYPE_BOOL) errorMsg(ERR_SEMANTIC_DATATYPE, "ASSIGN statement - assign can not be boolean");

                        //definition was successful and variable is inserted to tree
                        BSTInsert(treePtr, stringID, precResult.end_datatype, levelOfScope);

                        //EOL token 
                        if (token != EOL) errorMsg(ERR_SYNTAX, "Incorrect statement declaration - missing EOL");
                        genPops(stringID.str);
                }
                
                else if (token == ASSIGN) // <ass_stat> <ass_ids> = <ass_exps> ONLY FOR ONE VARIABLE!!!
                {
                        //if token on left side of assigning was '_' we do not need to check if it was declared
                        if (strcmp(stringID.str, "_") != 0)
                        {
                                //check if variable is in tree
                                if (isIDDeclared == false) { errorMsg(ERR_SEMANTIC_DEFINITION, "ID is not declared"); }
                                
                        }


                        //right side of assign can be function
                        token = get_new_token(&tokenStr);

                        //print can not be in assign 
                        if (token == F_PRINT){errorMsg(ERR_SEMANTIC_PARAM, "Print can not be used in assign");}

                        //build in functions
                        if (token == F_INT2FLOAT 
                        || token == F_FLOAT2INT 
                        || token == F_LEN )
                        {
                                if (token == F_INT2FLOAT) //int2float function
                                {

                                        int IDType;     
                                        //check type of ID to which valuse is assigned to
                                        if (strcmp(stringID.str, "_") != 0)
                                        {
                                                IDType = getType(*treePtr, stringID);
                                        }
                                        else
                                        {
                                                IDType = EMPTY;
                                        }                                

                                        //left param token
                                        token = get_new_token(&tokenStr);
                                        if (token != L_PAR) errorMsg(ERR_SYNTAX, "INT2FLOAT statement - missing '(' ");
                                        //token for precedence parser
                                        token = get_new_token(&tokenStr);
                                        if (token != ID && token != T_INT) errorMsg(ERR_SEMANTIC_PARAM, "INT2FLOAT statement - token must be ID or integer");

                                        //ID must check declaration and type
                                        if (token == ID)
                                        {
                                                string stringI2F;
                                                //check if ID is declared
                                                bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                                                if (isIDDeclared == false) errorMsg(ERR_SEMANTIC_DEFINITION, "ID not declared");

                                                strInit(&stringI2F);
                                                strCopyString(&stringI2F, &tokenStr);

                                                //check if type of input is integer
                                                int variableType = getType(*treePtr, stringI2F);
                                                if (variableType != T_INT){ errorMsg(ERR_SEMANTIC_PARAM, "INT2FLOAT statement - ID must be integer"); }

                                                //generate int2float instruction with ID as input
                                                genInt2Fl(stringID.str,ID,tokenStr.str);

                                                strFree(&stringI2F);
                                        }
                                        else
                                        {
                                                //generate int2float instruction with integer as input
                                                genInt2Fl(stringID.str,T_INT,tokenStr.str);
                                        }
 
                                        //Right param token
                                        token = get_new_token(&tokenStr);
                                        if (token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect INT2FLOAT statement -missing ')'");

                                        //EOL token
                                        token = get_new_token(&tokenStr);
                                        if (token != EOL) errorMsg(ERR_SYNTAX, "INT2FLOAT statement - missing EOL ");

                                        //check if return types are equal
                                        if(IDType != T_FLOAT && IDType != EMPTY) errorMsg(ERR_SEMANTIC_PARAM, "INT2FLOAT statement - Wrong type of value to which output is assigned to");

                                        return result;
                                }
                                else if (token == F_FLOAT2INT) // float2int function
                                {

                                        int IDType;
                                        //check type of ID to which valuse is assigned to
                                        if (strcmp(stringID.str, "_") != 0)
                                        {
                                                IDType = getType(*treePtr, stringID);
                                        }
                                        else
                                        {
                                                IDType = EMPTY;
                                        }                                                         

                                        //left param token
                                        token = get_new_token(&tokenStr);
                                        if (token != L_PAR) errorMsg(ERR_SYNTAX, "INT2FLOAT statement - missing '(' ");
                                        //token for precedence parser
                                        token = get_new_token(&tokenStr);
                                        if (token != ID && token != T_FLOAT) errorMsg(ERR_SEMANTIC_PARAM, "INT2FLOAT statement - token must be ID or float");
                                        
                                        //ID must check declaration and type
                                        if (token == ID)
                                        {
                                                string stringF2I;   
                                                //check if ID is declared
                                                bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                                                if (isIDDeclared == false) errorMsg(ERR_SEMANTIC_DEFINITION, "ID not declared");

                                                strInit(&stringF2I);
                                                strCopyString(&stringF2I, &tokenStr);

                                                //check if type of input is float
                                                int variableType = getType(*treePtr, stringF2I);
                                                if (variableType != T_FLOAT){ errorMsg(ERR_SEMANTIC_PARAM, "INT2FLOAT statement - ID must be float"); }

                                                //generate float2int instruction with ID as input
                                                genFl2Int(stringID.str,ID,tokenStr.str);

                                                strFree(&stringF2I);
                                        }
                                        else
                                        {
                                                //generate float2int instruction with float as input
                                                genFl2Int(stringID.str,T_FLOAT,tokenStr.str);
                                        }
                                        
        
                                        //Right param token
                                        token = get_new_token(&tokenStr);
                                        if (token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect INT2FLOAT statement -missing ')'");

                                        //EOL token
                                        token = get_new_token(&tokenStr);
                                        if (token != EOL) errorMsg(ERR_SYNTAX, "INT2FLOAT statement - missing EOL ");

                                        //check if return types are equal
                                        if(IDType != T_INT && IDType != EMPTY) errorMsg(ERR_SEMANTIC_PARAM, "FLOAT2INT statement - Wrong type of value to which output is assigned to");

                                        return result;
                                }
                                else if (token == F_LEN) // len function
                                {
                                        string stringLEN; 

                                        int IDType;
                                        //check type of ID to which valuse is assigned to
                                        if (strcmp(stringID.str, "_") != 0)
                                        {
                                                IDType = getType(*treePtr, stringID);
                                        }
                                        else
                                        {
                                                IDType = EMPTY;
                                        }                                                              

                                        //left param token
                                        token = get_new_token(&tokenStr);
                                        if (token != L_PAR) errorMsg(ERR_SYNTAX, "LEN statement - missing '(' ");
                                        //token for precedence parser
                                        token = get_new_token(&tokenStr);
                                        if (token != ID && token != T_STRING) errorMsg(ERR_SYNTAX, "LEN statement - token must be ID or string");
                                        
                                        if (token == ID)
                                        {
                                                //check if ID is declared
                                                bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                                                if (isIDDeclared == false) errorMsg(ERR_SEMANTIC_DEFINITION, "ID not declared");

                                                strInit(&stringLEN);
                                                strCopyString(&stringLEN, &tokenStr);
                                                //check if type of input is float
                                                int variableType = getType(*treePtr, stringLEN);
                                                if (variableType != T_STRING){ errorMsg(ERR_SEMANTIC_COMPATIBILITY, "LEN statement - ID must be string"); }
                                                
                                                //generate len instruction with ID as input
                                                genStrlen(stringID.str,ID,tokenStr.str);

                                                strFree(&stringLEN);
                                        }
                                        else
                                        {       
                                                //generate len instruction with string as input
                                                genStrlen(stringID.str,T_STRING,tokenStr.str);
                                        }
                                        
 
                                        //Right param token
                                        token = get_new_token(&tokenStr);
                                        if (token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect LEN statement -missing ')'");

                                        //EOL token
                                        token = get_new_token(&tokenStr);
                                        if (token != EOL) errorMsg(ERR_SYNTAX, "LEN statement - missing EOL ");

                                        //check if return types are equal
                                        if(IDType != T_INT && IDType != EMPTY) errorMsg(ERR_SEMANTIC_PARAM, "LEN statement - Wrong type of value to which output is assigned to");
                                     
                                        return result;
                                }
                        }
                        //token for prec parser
                        if (token == F_INPUTI || token == F_INPUTS || token == F_INPUTF || token == F_CHR || token == F_ORD || token == F_SUBSTR) {errorMsg(ERR_SEMANTIC_PARAM, "ASSIGN statement - wrong number of return types for built in function");}
                        else if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) errorMsg(ERR_SYNTAX, "ASSIGN statement - bad token after =");


                        //ulozit typ navratovej hodnoty premennej
                        //if it is ID check if it is declared
                        if (token == ID)
                        {
                                bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                                if (isIDDeclared == false)
                                {
                                       
                                        //add function to tree
                                        strClear(&funName);                 
                                        strCopyString(&funName, &tokenStr); 

                                        //generate call user function instruction
                                        genCall(funName.str);

                                        //funReturnCounter = 0;               
                                        funParamCounter = 0;  

                                        //token must be left param
                                        token = get_new_token(&tokenStr);

                                        int variableType;
                                        //check type of variable to which value will be assigned                                     
                                        //check type of ID to which valuse is assigned to
                                        if (strcmp(stringID.str, "_") != 0)
                                        {
                                                variableType = getType(*treePtr, stringID);
                                        }
                                        else
                                        {
                                                variableType = EMPTY;
                                        }                                                
                                        //check if function was used and process return type
                                        addFunToTree(&funTree, funName);
                                        addReturn(&funTree, funName, variableType, 1);


                                        //handle parameters of called function
                                        result = fun_call_param(treePtr);
                                        if (result != 0) return result;
                                        
                                        //check number of returns
                                        funReturnCheck(&funTree,funName,1);

                                        //token must be comma or EOL
                                        token = get_new_token(&tokenStr);

                                        //in this case where function is on right side of assign there cant be another id or function or vaule 
                                        if (token != EOL) errorMsg(ERR_SYNTAX, "RETURN statement - EOL missing");
                                        return result;
                                }
                        }

                        //call precedence parser 
                        precResult = prec_parse(treePtr, token, tokenStr);
                        token = precResult.end_token;
                        //EOL token loeaded in precedence parser
                        if (token != EOL) errorMsg(ERR_SYNTAX, "Incorrect statement assign - missing EOL");

                        int variableType;
                        //check if type on left sides is equal to type on right side of assign
                        if (strcmp(stringID.str, "_") != 0)
                        {
                                variableType = getType(*treePtr, stringID);
                        }
                        else
                        {
                                variableType = EMPTY;
                        }                         
                        //if left side is '_' we do not need to check type
                        if (strcmp(stringID.str, "_") != 0)
                        {
                                if (variableType != precResult.end_datatype){ errorMsg(ERR_SEMANTIC_COMPATIBILITY, "Incorrect statement assign - wrong type assigment"); }
                        }
                }
                else if (token == COMMA)//more IDs on left side
                {
                        //list for variables
                        funList *assignVariablesList;
                        assignVariablesList = malloc(sizeof(struct funList));
                        assignVariablesList->First = NULL;
                        assignVariablesList->elementCount = 0;

                        int assignVarCounter = 1;
                        if (strcmp(stringID.str, "_") != 0)
                        {
                                //look up type of first variable
                                int variableType = getType(*treePtr, stringID);
                                //add first variable to list
                                funListAdd(assignVariablesList,variableType,assignVarCounter);
                        }
                        else
                        {
                                //add empty variable to list
                                funListAdd(assignVariablesList,EMPTY,assignVarCounter); 
                        }

                        return ass_stat(treePtr,assignVariablesList,assignVarCounter,stringID);
                }
                
                else if (token == L_PAR) //<fun> ID ( <fun_call_param> )
                {
                        //add function to tree
                        strClear(&funName);                 
                        strCopyString(&funName, &stringID); 

                        //generate call user function instruction
                        genCall(funName.str);

                        //funReturnCounter = 0;               
                        funParamCounter = 0;               
                        addFunToTree(&funTree, stringID);

                        //function with return cant be called alone on line
                        funReturnCheck(&funTree,funName,0);

                        //handle parameters of called function
                        return fun_call_param(treePtr);
                }
        }
        else if (token == KW_RETURN) //<stat> return <return_values>
        {
                funReturnCounter = 0;
                // return was call so it is set to zero
                returnCalled = 0;
                return return_values(treePtr);
        }

        /**
         * Built-in functions
         */


        else if (token == F_INPUTF)
        {
                errorMsg(ERR_SEMANTIC_OTHER, "Statement inputf can not be lonely on line, it will be sad");
        }
        else if (token == F_INPUTI)
        {
                errorMsg(ERR_SEMANTIC_OTHER, "Statement inputi can not be lonely on line, it will be sad");
        }
        else if (token == F_INPUTS)
        {
                errorMsg(ERR_SEMANTIC_OTHER, "Statement inputs can not be lonely on line, it will be sad");
        }
        else if (token == F_FLOAT2INT)
        {
                errorMsg(ERR_SEMANTIC_PARAM, "Statement float2int can not be lonely on line, it will be sad");
        }
        else if (token == F_INT2FLOAT)
        {
                errorMsg(ERR_SEMANTIC_PARAM, "Statement int2float can not be lonely on line, it will be sad");
        }
        else if (token == F_LEN)
        {
                errorMsg(ERR_SEMANTIC_PARAM, "Statement len can not be lonely on line, it will be sad");
        }
        else if (token == F_ORD)
        {
                errorMsg(ERR_SEMANTIC_PARAM, "Statement ord can not be lonely on line, it will be sad");
        }
        else if (token == F_PRINT) // print function with
        {
                //left param token
                token = get_new_token(&tokenStr);
                if (token != L_PAR) errorMsg(ERR_SYNTAX, "PRINT statement - '(' missing");

                //handled in print_params
                result = print_params(treePtr);
                if (result != 0) return result;

                //EOL token
                token = get_new_token(&tokenStr);
                if (token != EOL) errorMsg(ERR_SYNTAX, "PRINT statement - EOL missing");
        }
        else if (token == F_SUBSTR)
        {
                errorMsg(ERR_SEMANTIC_PARAM, "Statement substr can not be lonely on line, it will be sad");
        }
        else if (token == F_CHR)
        {
                errorMsg(ERR_SEMANTIC_PARAM, "Statement chr can not be lonely on line, it will be sad");
        }
        //IDs handled successfuly
        return result;
}


/**
 * @brief <ass_stat> <ass_ids> = <ass_exps>.
 *  Left sides of assign statement
 * 
 * @param treePtr tree for variables
 */
int ass_stat(varNode *treePtr,funList *assignVariablesList,int assignVarCounter,string readFunctionID)
{
        int result = 0;

        //ID token
        token = get_new_token(&tokenStr);
        if (token != ID) errorMsg(ERR_SYNTAX, "ASSIGN statement - must be ID");

        //empty ID
        string emptyID;
        strInit(&emptyID);
        strCopyString(&emptyID, &tokenStr);

        //if variable is basic ID we check if it is declared,get type and add it to list
        //else if variable is empty we add EMPTY type variable to list
        assignVarCounter++;
        if (strcmp(emptyID.str, "_") != 0)
        {
                //is ID declared?
                bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                if (isIDDeclared == false) errorMsg(ERR_SEMANTIC_DEFINITION, "ID not declared");

                //look up type of variable
                int variableType = getType(*treePtr, tokenStr);
        
                //add variable to list
                funListAdd(assignVariablesList,variableType,assignVarCounter);
        }
        else
        {
                //add empty variable to list
                funListAdd(assignVariablesList,EMPTY,assignVarCounter);
        }       
        strFree(&emptyID);     


        //comma or assign token
        token = get_new_token(&tokenStr);
        if (token != COMMA && token != ASSIGN) errorMsg(ERR_SYNTAX, "ASSIGN statement - token must be '=' or ',' for another ID");

        //recursively call ass_stat for more IDs
        if (token == COMMA) return ass_stat(treePtr,assignVariablesList,assignVarCounter,readFunctionID);

        //list for right side of assignment
        funList *assignAssignList;
        assignAssignList = malloc(sizeof(struct funList));
        assignAssignList->First = NULL;
        assignAssignList->elementCount = 0;

        int assignAssignmentCounter = 0;

        //handle ass_exps
        return ass_exps(treePtr,assignVariablesList,assignVarCounter,assignAssignList,assignAssignmentCounter,readFunctionID);

        //clear list of variables
        funListDelete(assignVariablesList);
        //successfuly handled
        return result;
}


/**
 * @brief <ass_exps>	<exp> <ass_exps>
 * <ass_exps>	, <exp>	<ass_exps>
 * <ass_exps>	<ass_ids>
 * <ass_exps>	<fun>
 * 
 * @param treePtr tree for variables
 */
int ass_exps(varNode *treePtr,funList *assignVariablesList,int assignVarCounter,funList *assignAssignList,int assignAssignmentCounter, string readFunctionID)
{
        int result = 0;
        prec_end_struct precResult;

        
        token = get_new_token(&tokenStr);

        
        /**
        * token for built-in functions
        * 
        */
       //print can not be in assign 
        if (token == F_PRINT){errorMsg(ERR_SEMANTIC_PARAM, "Print can not be used in assign");}
        if (token == F_INPUTI)
        {
                //generate read integer instruction
                genRead(F_INPUTI,readFunctionID.str);

                //left param token
                token = get_new_token(&tokenStr);
                if (token != L_PAR) errorMsg(ERR_SYNTAX, "INPUTI statement - missing '(' ");
                
                //Right param token
                token = get_new_token(&tokenStr);
                if (token != R_PAR) errorMsg(ERR_SYNTAX, "INPUTI statement -missing ')'");

                //EOL token
                token = get_new_token(&tokenStr);
                if (token != EOL) errorMsg(ERR_SYNTAX, "INPUTI statement - missing EOL ");
                
                //check if return types are equal
                assignAssignmentCounter++;
                funListAdd(assignAssignList,T_INT,assignAssignmentCounter);
                assignAssignmentCounter++;
                funListAdd(assignAssignList,T_INT,assignAssignmentCounter);
                compareLists(assignVariablesList,assignAssignList);

                return result;
        }
        else if (token == F_INPUTF)
        {
                //generate read float instruction
                genRead(F_INPUTF,readFunctionID.str);

                //left param token
                token = get_new_token(&tokenStr);
                if (token != L_PAR) errorMsg(ERR_SYNTAX, "INPUTF statement - missing '(' ");
                
                //Right param token
                token = get_new_token(&tokenStr);
                if (token != R_PAR) errorMsg(ERR_SYNTAX, "INPUTF statement -missing ')'");

                //EOL token
                token = get_new_token(&tokenStr);
                if (token != EOL) errorMsg(ERR_SYNTAX, "INPUTF statement - missing EOL ");

                //check if return types are equal
                assignAssignmentCounter++;
                funListAdd(assignAssignList,T_FLOAT,assignAssignmentCounter);
                assignAssignmentCounter++;
                funListAdd(assignAssignList,T_INT,assignAssignmentCounter);
                compareLists(assignVariablesList,assignAssignList);

                return result;
        }
        else if (token == F_INPUTS)
        {
                //generate read string instruction
                genRead(F_INPUTS,readFunctionID.str);

                //left param token
                token = get_new_token(&tokenStr);
                if (token != L_PAR) errorMsg(ERR_SYNTAX, "INPUTS statement - missing '(' ");
                
                //Right param token
                token = get_new_token(&tokenStr);
                if (token != R_PAR) errorMsg(ERR_SYNTAX, "INPUTS statement -missing ')'");

                //EOL token
                token = get_new_token(&tokenStr);
                if (token != EOL) errorMsg(ERR_SYNTAX, "INPUTS statement - missing EOL ");

                //check if return types are equal
                assignAssignmentCounter++;
                funListAdd(assignAssignList,T_STRING,assignAssignmentCounter);
                assignAssignmentCounter++;
                funListAdd(assignAssignList,T_INT,assignAssignmentCounter);
                compareLists(assignVariablesList,assignAssignList);

                return result;               
        }
        else if (token == F_SUBSTR)
        {
                string stringSUBSTR;                                     

                //left param token
                token = get_new_token(&tokenStr);
                if (token != L_PAR) errorMsg(ERR_SYNTAX, "SUBSTR statement - missing '(' ");
                //token for precedence parser
                token = get_new_token(&tokenStr);
                if (token != ID && token != T_STRING) errorMsg(ERR_SEMANTIC_PARAM, "SUBSTR statement - token must be ID or string");
                
                if (token == ID)
                {
                        //check if ID is declared
                        bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                        if (isIDDeclared == false) errorMsg(ERR_SEMANTIC_DEFINITION, "ID not declared");

                        strInit(&stringSUBSTR);
                        strCopyString(&stringSUBSTR, &tokenStr);
                        //check if type of input is float
                        int variableType = getType(*treePtr, stringSUBSTR);
                        if (variableType != T_STRING){ errorMsg(ERR_SEMANTIC_COMPATIBILITY, "SUBSTR statement - ID must be string"); }
                        strFree(&stringSUBSTR);
                }

                //comma token
                token = get_new_token(&tokenStr);
                if (token != COMMA) errorMsg(ERR_SYNTAX, "SUBSTR statement - comma missing between parameters");

                //token for precedence parser
                token = get_new_token(&tokenStr);
                if (token != ID && token != T_INT) errorMsg(ERR_SEMANTIC_PARAM, "SUBSTR statement - token must be ID or integer");
                
                if (token == ID)
                {
                        //check if ID is declared
                        bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                        if (isIDDeclared == false) errorMsg(ERR_SEMANTIC_DEFINITION, "SUBSTR not declared");

                        strInit(&stringSUBSTR);
                        strCopyString(&stringSUBSTR, &tokenStr);
                        //check if type of input is float
                        int variableType = getType(*treePtr, stringSUBSTR);
                        if (variableType != T_INT){ errorMsg(ERR_SEMANTIC_PARAM, "SUBSTR statement - ID must be integer"); }
                        strFree(&stringSUBSTR);
                }

                //comma token
                token = get_new_token(&tokenStr);
                if (token != COMMA) errorMsg(ERR_SYNTAX, "SUBSTR statement - comma missing between parameters");

                //token for precedence parser
                token = get_new_token(&tokenStr);
                if (token != ID && token != T_INT) errorMsg(ERR_SEMANTIC_PARAM, "SUBSTR statement - token must be ID or integer");
                
                if (token == ID)
                {
                        //check if ID is declared
                        bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                        if (isIDDeclared == false) errorMsg(ERR_SEMANTIC_DEFINITION, "SUBSTR not declared");

                        strInit(&stringSUBSTR);
                        strCopyString(&stringSUBSTR, &tokenStr);
                        //check if type of input is float
                        int variableType = getType(*treePtr, stringSUBSTR);
                        if (variableType != T_INT){ errorMsg(ERR_SEMANTIC_PARAM, "SUBSTR statement - ID must be integer"); }
                        strFree(&stringSUBSTR);
                }

                //Right param token
                token = get_new_token(&tokenStr);
                if (token != R_PAR) errorMsg(ERR_SYNTAX, "SUBSTR statement -missing ')'");

                //EOL token
                token = get_new_token(&tokenStr);
                if (token != EOL) errorMsg(ERR_SYNTAX, "SUBSTR statement - missing EOL ");

                //check if return types are equal
                assignAssignmentCounter++;
                funListAdd(assignAssignList,T_STRING,assignAssignmentCounter);
                assignAssignmentCounter++;
                funListAdd(assignAssignList,T_INT,assignAssignmentCounter);
                compareLists(assignVariablesList,assignAssignList);

                return result;
        }
        else if (token == F_CHR)
        {
                string stringCHR;                                     

                //left param token
                token = get_new_token(&tokenStr);
                if (token != L_PAR) errorMsg(ERR_SYNTAX, "CHR statement - missing '(' ");
                //token for precedence parser
                token = get_new_token(&tokenStr);
                if (token != ID && token != T_INT) errorMsg(ERR_SEMANTIC_PARAM, "CHR statement - token must be ID or integer");
                
                if (token == ID)
                {
                        //check if ID is declared
                        bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                        if (isIDDeclared == false) errorMsg(ERR_SEMANTIC_DEFINITION, "ID not declared");

                        strInit(&stringCHR);
                        strCopyString(&stringCHR, &tokenStr);
                        //check if type of input is float
                        int variableType = getType(*treePtr, stringCHR);
                        if (variableType != T_INT){ errorMsg(ERR_SEMANTIC_PARAM, "CHR statement - ID must be integer"); }
                        strFree(&stringCHR);
                }

                //Right param token
                token = get_new_token(&tokenStr);
                if (token != R_PAR) errorMsg(ERR_SYNTAX, "CHR statement -missing ')'");

                //EOL token
                token = get_new_token(&tokenStr);
                if (token != EOL) errorMsg(ERR_SYNTAX, "CHR statement - missing EOL ");

                //check if return types are equal
                assignAssignmentCounter++;
                funListAdd(assignAssignList,T_STRING,assignAssignmentCounter);
                assignAssignmentCounter++;
                funListAdd(assignAssignList,T_INT,assignAssignmentCounter);
                compareLists(assignVariablesList,assignAssignList);

                return result; 
        }
        else if (token == F_ORD)
        {
                string stringORD;                                     

                //left param token
                token = get_new_token(&tokenStr);
                if (token != L_PAR) errorMsg(ERR_SYNTAX, "ORD statement - missing '(' ");
                //token for precedence parser
                token = get_new_token(&tokenStr);
                if (token != ID && token != T_STRING) errorMsg(ERR_SEMANTIC_PARAM, "ORD statement - token must be ID or string");
                
                if (token == ID)
                {
                        //check if ID is declared
                        bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                        if (isIDDeclared == false) errorMsg(ERR_SEMANTIC_DEFINITION, "ID not declared");

                        strInit(&stringORD);
                        strCopyString(&stringORD, &tokenStr);
                        //check if type of input is float
                        int variableType = getType(*treePtr, stringORD);
                        if (variableType != T_STRING){ errorMsg(ERR_SEMANTIC_PARAM, "CHR statement - ID must be string"); }
                        strFree(&stringORD);
                }

                //comma token
                token = get_new_token(&tokenStr);
                if (token != COMMA) errorMsg(ERR_SYNTAX, "ORD statement - comma missing between parameters");

                //token for precedence parser
                token = get_new_token(&tokenStr);
                if (token != ID && token != T_INT) errorMsg(ERR_SEMANTIC_PARAM, "ORD statement - token must be ID or integer");
                
                if (token == ID)
                {
                        //check if ID is declared
                        bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                        if (isIDDeclared == false) errorMsg(ERR_SEMANTIC_DEFINITION, "ID not declared");

                        strInit(&stringORD);
                        strCopyString(&stringORD, &tokenStr);
                        //check if type of input is float
                        int variableType = getType(*treePtr, stringORD);
                        if (variableType != T_INT){ errorMsg(ERR_SEMANTIC_PARAM, "ORD statement - ID must be integer"); }
                        strFree(&stringORD);
                }

                //Right param token
                token = get_new_token(&tokenStr);
                if (token != R_PAR) errorMsg(ERR_SYNTAX, "ORD statement -missing ')'");

                //EOL token
                token = get_new_token(&tokenStr);
                if (token != EOL) errorMsg(ERR_SYNTAX, "ORD statement - missing EOL ");

                //check if return types are equal
                assignAssignmentCounter++;
                funListAdd(assignAssignList,T_INT,assignAssignmentCounter);
                assignAssignmentCounter++;
                funListAdd(assignAssignList,T_INT,assignAssignmentCounter);
                compareLists(assignVariablesList,assignAssignList);

                return result;    
        }




        //token for precedence parser or user function
        if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) errorMsg(ERR_SYNTAX, "Incorrect token after RETURN - must be ID, FLOAT, INT or STRING");

        //if it is ID check if it is declared
        if (token == ID)
        {
                bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                //if ID is not declared it means it must be function
                if (isIDDeclared == false)
                {
                        //add function to tree
                        strClear(&funName);                 
                        strCopyString(&funName, &tokenStr); 

                        //generate call user function instruction
                        genCall(funName.str);

                        funReturnCounter = 0;               
                        funParamCounter = 0;                       

                        //token must be left param
                        token = get_new_token(&tokenStr);


                        addFunToTree(&funTree, funName);
                        //check return count and type
                        funListCompareReturn (assignVariablesList, &funTree, funName, assignVarCounter);
                        
                        //handle parameters of called function
                        result = fun_call_param(treePtr);
                        if (result != 0) return result;

                        //token must be comma or EOL
                        token = get_new_token(&tokenStr);

                        //in this case where function is on right side of assign there cant be another id or function or vaule 
                        if (token != EOL) errorMsg(ERR_SYNTAX, "RETURN statement - EOL missing");
                        return result;
                }
                //precedence parser called
                precResult = prec_parse(treePtr, token, tokenStr);
                token = precResult.end_token;

                //value from precedence parser can not be bool type
                if (precResult.end_datatype == TYPE_BOOL) errorMsg(ERR_SEMANTIC_COMPATIBILITY, "RETURN statement - return type can't be BOOL");
                //add right side of assign to list
                assignAssignmentCounter++;
                funListAdd(assignAssignList,precResult.end_datatype,assignAssignmentCounter);
        }
        else if (token != L_PAR)
        {
                assignAssignmentCounter++;
                funListAdd(assignAssignList,token,assignAssignmentCounter);
                token = get_new_token(&tokenStr);
        }


        //funListAdd(assignAssignList,EMPTY,assignAssignmentCounter);

        //funListAdd(assignAssignList,T_TYPE,assignAssignmentCounter);

        //if comma recursively call ass_exps
        if (token != COMMA && token != EOL) errorMsg(ERR_SYNTAX, "RETURN statement - ',' or EOL missing");
        if (token == COMMA) return ass_exps(treePtr,assignVariablesList,assignVarCounter,assignAssignList,assignAssignmentCounter,readFunctionID);

        //check if return types are equal
        compareLists(assignVariablesList,assignAssignList);
        return result;
}


/**
 * @brief <fun_call_param> ID <fun_call_param>.
 * Parameters of called function
 * @param treePtr
 */
int fun_call_param(varNode *treePtr)
{
        int result = 0;

        //token for parameter in called function
        token = get_new_token(&tokenStr);
        if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect token in func call parameters");

        //if it is ID check if it is declared
        if (token == ID)
        {
                bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                if (isIDDeclared == false) errorMsg(ERR_SEMANTIC_DEFINITION, "ID not declared");
        }

        //function without parameters
        if (token == R_PAR) 
        {
           addFunCall(&funTree, funName, *treePtr, funParamCounter/*, funReturnCounter*/);    
           return result;  
        }

        //parameter counter
        funParamCounter++; 
        //add params but if token is ID we must get type of ID first
        if (token == ID) addParam(&funTree, funName, getType(*treePtr, tokenStr), funParamCounter);
        else addParam(&funTree, funName, token, funParamCounter);

        //comma for another parameter or right param for end of paremeters
        token = get_new_token(&tokenStr);
        if (token != COMMA && token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect token in func call parameters");

        //recursively call fun_call_param for another parameters
        if (token == COMMA)
        {
                return fun_call_param(treePtr);
        }
        addFunCall(&funTree, funName, *treePtr, funParamCounter/*, funReturnCounter*/);
        return result;
}


/**
 * @brief <fun_params> <par> <par_next>.
 * parameters in function
 * 
 * @param treePtr tree for variables
 */

int fun_params(varNode *treePtr)
{
        int result = 0;
        //variable for checking if multiple parameters are present
        int multipleParams = 0;
        
        //if token is comma multiple parameters are present
        if (token == COMMA) multipleParams = 1;

        //token id or Right par for end of parameters
        token = get_new_token(&tokenStr);
        if (token != ID && token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect params");

        //jump out of function
        if (token == R_PAR && multipleParams == 0)  { return result; }
        else if (token == R_PAR && multipleParams == 1) errorMsg(ERR_SYNTAX, "Incorrect params");

        //check if variable is declared
        bool isIDDeclared = isDeclared(*treePtr, tokenStr);
        string stringID;
        strInit(&stringID);
        strCopyString(&stringID, &tokenStr);
        //cannot declare variable '_'
        if (strcmp(stringID.str, "_") == 0) { errorMsg(ERR_SEMANTIC_DEFINITION, "Can not declare '_'"); }

        //if ID is declared it is error
        if (isIDDeclared == true) { errorMsg(ERR_SEMANTIC_DEFINITION, "ID is already declared"); }

        //generate definition of variable in function header
        genDefvar(tokenStr.str);

        //token must be keyword for type
        token = get_new_token(&tokenStr);
        if (token != KW_FLOAT64 && token != KW_INT && token != KW_STRING) errorMsg(ERR_SYNTAX, "Incorrect or missing param type");

        //save type of token to variable
        int tokenDataType = 0;
        if (token == KW_FLOAT64) tokenDataType = T_FLOAT;
        else if (token == KW_INT) tokenDataType = T_INT;
        else if (token == KW_STRING) tokenDataType = T_STRING;
        else errorMsg(ERR_SEMANTIC_DATATYPE, "ASSIGN statement - assign can not be boolean");

        //counter for parameters increment
        funParamCounter++;         
        // add parameter to function                                  
        addParam(&funTree, funName, tokenDataType, funParamCounter); 
        //add ID and type of variable to tree
        BSTInsert(treePtr, stringID, tokenDataType, levelOfScope);

        //recursively call fun_params if it is comma or end parameterrs
        token = get_new_token(&tokenStr);
        if (token != COMMA && token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect params");
        if (token == COMMA) return fun_params(treePtr);
        //sucesfully handled
        return result;
}

/**
 * @brief <fun_returns>	<ret>	<ret_next>.
 * Returns in definition of function
 */

int fun_returns()
{
        int result = 0;
        //variable for multiple returns
        int multipleParams = 0;
        //multiple returns are present
        if (token == COMMA) multipleParams = 1;

        //token must be float int string or right param for function without returns
        token = get_new_token(&tokenStr);
        if (token != KW_FLOAT64 && token != KW_INT && token != KW_STRING && token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect return params");

        //end of function
        if (token == R_PAR && multipleParams == 0) return result;
        //handling of more parameters
        else if (token == R_PAR && multipleParams == 1) errorMsg(ERR_SYNTAX, "Incorrect return params");

        //save type of token to variable
        int tokenDataType = 0;
        if (token == KW_FLOAT64) tokenDataType = T_FLOAT;
        else if (token == KW_INT) tokenDataType = T_INT;
        else if (token == KW_STRING) tokenDataType = T_STRING;
        else errorMsg(ERR_SEMANTIC_DATATYPE, "ASSIGN statement - assign can not be boolean");

        //counter for returns increment
        funReturnCounter++;         
        // add return to function                                  
        addReturn(&funTree, funName, tokenDataType, funReturnCounter); 

        //end of parameters or multiple parameters
        token = get_new_token(&tokenStr);
        if (token != COMMA && token != R_PAR) errorMsg(ERR_SYNTAX, "Incorrect return params");
        if (token == COMMA) return fun_returns();

        return result;
}


/**
 * @brief Parameters in print function
 * 
 * @param treePtr tree for variables
 */
int print_params(varNode *treePtr)
{
        int result = 0;

        //variable for multiple parameters
        int multipleParams = 0;
        //multiple parameters are present
        if (token == COMMA) multipleParams = 1;

        //token must be type or Right param for function without parameters
        token = get_new_token(&tokenStr);
        if (token != ID && token != T_STRING && token != T_FLOAT && token != T_INT && token != R_PAR) errorMsg(ERR_SYNTAX, "PRINT function - Incorrect params");

        //end of function or wrong token inside
        if (token == R_PAR && multipleParams == 0) return result;
        else if (token == R_PAR && multipleParams == 1)  errorMsg(ERR_SYNTAX, "PRINT function - Incorrect params");
        
        if (token == ID)
        {
                //check if variable is declared
                bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                if (isIDDeclared != true && token == ID) {errorMsg(ERR_SEMANTIC_DEFINITION, "ID is not declared");}
        }
        genWrite(token, tokenStr.str);
        //if comma multiple parameters are present recursively call print_params
        token = get_new_token(&tokenStr);
        if (token != COMMA && token != R_PAR) errorMsg(ERR_SYNTAX, "PRINT function - Incorrect params");
        if (token == COMMA) return print_params(treePtr);
        //print parameters handled correctly
        return result;
}


/**
 * @brief Values in return function, this is <ass_exps> but with return checks
 * 
 * @param treePtr tree for variables
 */
int return_values(varNode *treePtr)
{
        int result = 0;
        
        prec_end_struct precResult;
        //variable for multiple parameters
        int multipleParams = 0;
        //multiple parameters are present
        if (token == COMMA) multipleParams = 1;

        //token viable for return function
        token = get_new_token(&tokenStr);
        if (token != ID && token != T_STRING && token != T_FLOAT && token != T_INT && token != EOL) errorMsg(ERR_SYNTAX, "Return function - Incorrect params");

        //end of function or wrong token inside, checking types of returns
        if (token == EOL && multipleParams == 0) 
        { 
                funReturnCheck(&funTree,funDeclaredName,funReturnCounter);
                return result; 
        }
        else if (token == EOL && multipleParams == 1)  errorMsg(ERR_SYNTAX, "Return function - Incorrect params");

        //check if variable is declared
        bool isIDDeclared = isDeclared(*treePtr, tokenStr);
        if (isIDDeclared != true && token == ID) {errorMsg(ERR_SEMANTIC_DEFINITION, "ID is not declared");}

        //call precedence parser with tree of variables, current token, and string of token
        precResult = prec_parse(treePtr, token, tokenStr);
        //token loaded last first token after expression
        token = precResult.end_token;

        //check return types
        funReturnCounter++;
        addReturn(&funTree,funDeclaredName,precResult.end_datatype,funReturnCounter);
        
        //token was already loaded in precedence parser
        //recursively calling values if comma or end handling if EOL
        if (token != COMMA && token != EOL) errorMsg(ERR_SYNTAX, "Return function - Incorrect params");
        if (token == COMMA) return return_values(treePtr);

        //sanity check
        funReturnCheck(&funTree,funDeclaredName,funReturnCounter);
        return result;
}