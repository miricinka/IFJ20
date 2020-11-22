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
int levelOfScope = 1; //variabel for scope, it is incremented
//generation
tListOfInstr *list; //variable for list of instructions


/**
 * @brief Adds new instruction to list of instructions
 * 
 * @param instType type of generated instruction
 */
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


/**
 * @brief Start of parsing, initialization of structures
 * 
 * @param instrList list of instructions for generation
 */
int parse(tListOfInstr *instrList)
{
        //initialization of string far names of functions
        strInit(&funName);
        //initialization of string for current token
        strInit(&tokenStr);
        //initialization of function tree
        funInit(&funTree);
        //after result is zero after this function it bubbles to main as sucesfull analysis of code
        //this applies to whole parser
        int result = 0;
        list = instrList;
        //loads next token
        token = get_new_token(&tokenStr);
        //if token is end of file => error Empty File
        if (token == ENDFILE) errorMsg(ERR_SYNTAX, "Empty file");
        //starting analysis with first non-term
        else result = program();

        //free of strings
        strFree(&tokenStr);
        strFree(&funName);
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
        string tempMain;

        token = get_new_token(&tokenStr);       
        switch (token)
        {
        case KW_FUNC: //starts <fun_def> non-term analysis
                result = fun_def();
                if (result != 0) return result;
                return fun_def_list();
        case EOL: //recursively chceck for optional EOLs
                return fun_def_list();
        case ENDFILE: //end of program
                //printFunTree(funTree);               
                strInit(&tempMain);
                strAddChars(&tempMain, "main");
                //funSearch finds function in tree of functions, if main is not there it is error 3
                if (!funSearch(&funTree, tempMain)) errorMsg(ERR_SEMANTIC_DEFINITION, "Program is missing \"main\" function");
                //check in tree if called functions were declared
                if (isFunCallDec(funTree) != 0) errorMsg(ERR_SEMANTIC_DEFINITION, "Function was not defined");
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
                addFunDec(&funTree, tempMain, funParamCounter, funReturnCounter);
                strFree(&tempMain);

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

                //create variable tree for main functions
                varNode treePtr;
                BSTInit(&treePtr);
                //handle stat_list rule
                result = stat_list(&treePtr);
                if (result != 0) return result;

                //right bracket token, this is end of main function
                if (token != R_BR) errorMsg(ERR_SYNTAX, "Wrong main func signature - missing '}' ");
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

                //left param token
                token = get_new_token(&tokenStr);
                if (token != L_PAR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '(' ");

                //create tree for variables in function
                varNode treePtr;
                BSTInit(&treePtr);
                //counter for number of parameters in function
                funParamCounter = 0;
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

                        //right param token, it was already loaded in fun returns
                        if (token != R_PAR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing ')' ");
                        //parameters and returns were added to functions in fun_params and fun_returns rules
                        token = get_new_token(&tokenStr);
                }
                //number of parameters and returns added to function in tree
                addFunDec(&funTree, funName, funParamCounter, funReturnCounter);
                
                //left bracket token
                if (token != L_BR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '{' ");

                //handle stat_list rule
                result = stat_list(&treePtr);
                if (result != 0) return result;

                //right bracket token, already loaded in stat_list
                if (token != R_BR) errorMsg(ERR_SYNTAX, "Wrong func signature - missing '}' ");

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
                        if (token == F_INPUTS 
                        || token == F_INPUTI 
                        || token == F_INPUTF 
                        || token == F_INT2FLOAT 
                        || token == F_FLOAT2INT 
                        || token == F_LEN 
                        || token == F_SUBSTR 
                        || token == F_ORD 
                        || token == F_CHR)
                        {
                                //TODO
                        }
                        //token for prec parser
                        if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) errorMsg(ERR_SYNTAX, "Incorrect statement - bad token after =");

                        //call precedence parser 
                        precResult = prec_parse(treePtr, token, tokenStr);
                        token = precResult.end_token;
                        //EOL token loeaded in precedence parser
                        if (token != EOL) errorMsg(ERR_SYNTAX, "Incorrect statement assign - missing EOL");


                        //check if type on left sides is equal to type on right side of assign
                        int variableType = getType(*treePtr, stringID);
                        //if left side is '_' we do not need to check type
                        if (strcmp(stringID.str, "_") != 0)
                        {
                                if (variableType != precResult.end_datatype){ errorMsg(ERR_SEMANTIC_COMPATIBILITY, "Incorrect statement assign - wrong type assigment"); }
                        }
                }
                else if (token == COMMA)//more IDs on left side
                {
                        //TODO
                        return ass_stat(treePtr);
                }
                
                else if (token == L_PAR) //<fun> ID ( <fun_call_param> )
                {
                        //add function to tree
                        strClear(&funName);                 
                        strCopyString(&funName, &stringID); 
                        funReturnCounter = 0;               
                        funParamCounter = 0;               
                        addFunToTree(&funTree, stringID);
                        //handle parameters of called function
                        return fun_call_param(treePtr);
                }
        }
        else if (token == KW_RETURN) //<stat> return <ass_exps>
        {
                return ass_exps(treePtr);
        }

        /**
         * Built-in functions
         */


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
 *  TODO
 * 
 * @param treePtr tree for variables
 */
int ass_stat(varNode *treePtr)
{
        int result = 0;

        //ID token
        token = get_new_token(&tokenStr);
        if (token != ID) errorMsg(ERR_SYNTAX, "ASSIGN statement - must be ID");

        //comma or assign token
        token = get_new_token(&tokenStr);
        if (token != COMMA && token != ASSIGN) errorMsg(ERR_SYNTAX, "ASSIGN statement - token must be '=' or ',' for another ID");

        //recursively call ass_stat for more IDs
        if (token == COMMA) return ass_stat(treePtr);

        //handle ass_exps
        return ass_exps(treePtr);
        //successfuly handled
        return result;
}


/**
 * @brief <ass_exps>	<exp> <ass_exps>
 * <ass_exps>	, <exp>	<ass_exps>
 * <ass_exps>	<ass_ids>
 * <ass_exps>	<fun>
 * TODO
 * 
 * @param treePtr tree for variables
 */
int ass_exps(varNode *treePtr)
{
        int result = 0;
        prec_end_struct precResult;

        //token for precedence parser
        token = get_new_token(&tokenStr);
        if (token != T_INT && token != T_STRING && token != T_FLOAT && token != ID && token != L_PAR) errorMsg(ERR_SYNTAX, "Incorrect token after RETURN - must be ID, FLOAT, INT or STRING");

        //precedence parser called
        precResult = prec_parse(treePtr, token, tokenStr);
        token = precResult.end_token;

        //check if it is function
        /*if (token == L_PAR)
        {
                result = fun_call_param(); 
                if (result != 0) return result;
                if (token != R_PAR) errorMsg(ERR_SYNTAX, "FUNCTION CALL statement - ')' missing");
                token = get_new_token(&tokenStr);
        }*/

        //
        if (precResult.end_datatype == TYPE_BOOL)
                errorMsg(ERR_SEMANTIC_COMPATIBILITY, "RETURN statement - return type can't be BOOL");
        //token = get_new_token(&tokenStr); //toto pojde prec precedencka vrati SEMICOL token
        if (token != COMMA && token != EOL)
                errorMsg(ERR_SYNTAX, "RETURN statement - ',' or EOL missing");
        if (token == COMMA)
                return ass_exps(treePtr);
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

        //function without parameters
        if (token == R_PAR) return result; 
        //if it is ID check if it is declared
        if (token == ID)
        {
                bool isIDDeclared = isDeclared(*treePtr, tokenStr);
                if (isIDDeclared == false) errorMsg(ERR_SEMANTIC_DEFINITION, "ID not declared");
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
        //TODO Comment
        addFunCall(&funTree, funName, *treePtr, funParamCounter, funReturnCounter);
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
        if (token == R_PAR && multipleParams == 0)
        {
                //TODO
                return result;
        }
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

        //check if variable is declared
        bool isIDDeclared = isDeclared(*treePtr, tokenStr);
        if (isIDDeclared != true && token == ID) {errorMsg(ERR_SEMANTIC_DEFINITION, "ID is not declared");}

        //if comma multiple parameters are present recursively call print_params
        token = get_new_token(&tokenStr);
        if (token != COMMA && token != R_PAR) errorMsg(ERR_SYNTAX, "PRINT function - Incorrect params");
        if (token == COMMA) return print_params(treePtr);
        //print parameters handled correctly
        return result;
}