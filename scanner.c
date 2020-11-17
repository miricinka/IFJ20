/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  scanner.c
 * 
 * DESCRIPTION:
 *  Lexical analysis
 * 
 * AUTHORS:
 *  Kolaříková Mirka    <xkolar76@stud.fit.vutbr.cz>
 *  Žovinec Martin      <xzovin00@stud.fit.vutbr.cz>
*/


#include "scanner.h"

int state = START;

int get_new_token(string *tokenStr) {

    char next_char;
    strClear(tokenStr);

    bool dot = false;       //temp var to ensure decimal point is in number just once
    bool exp = false;       //temp var to ensure exponent is in number just once
    bool sign = false;      //temp var to ensure sign is set correctly

    while (1){

        next_char = getc(stdin);

        switch (state){

            case START:

                // Ignore all white chars
                if (isspace(next_char)){
                    state = START;
                    if (next_char != '\n' && next_char != EOF)
                        break;
                }

                // end states

                switch (next_char){

                    case '+':
                        return ADD;
                    case '-':
                        return SUB;
                    case '*':
                        return MUL;
                    case '(':
                        return L_PAR;
                    case ')':
                        return R_PAR;
                    case ',':
                        return COMMA;
                    case ';':
                        return SEMICOL;
                    case '{':
                        return L_BR;
                    case '}':
                        return R_BR;
                    case '\n':
                        return EOL;
                    case EOF:
                        return ENDFILE;

                // Possible non-finite states 

                    case '/': //TODO
                        state = S_DIV;
                        break;
                    case '=':
                        state = S_ASSIGN;
                        break;
                    case '<':
                        state = S_L;
                        break;
                    case '>':
                        state = S_G;
                        break;
                    case '!':
                        state = S_NEG;
                        break;
                    case ':':
                        state = S_VAR_DEF;
                        break;
                    case '"': //TODO
                        state = S_STRING;
                        break;
                    case '_':
                        if(strAddChar(tokenStr, next_char)){
                            errorMsg(ERR_INTERNAL, "str.c allocation error");}
                        state = S_ID;
                        break;

                    default:
                        if(isalpha(next_char)){
                            if(strAddChar(tokenStr, next_char)){
                                errorMsg(ERR_INTERNAL, "str.c allocation error");}
                            state = S_KW;
                        }else if(isdigit(next_char)){
                            if(strAddChar(tokenStr, next_char)){
                                errorMsg(ERR_INTERNAL, "str.c allocation error");}
                            //check char after zero - numbers cannot start with 0
                            if(next_char == '0'){
                            	next_char = getc(stdin);
                            	if(isdigit(next_char)){
                            		errorMsg(ERR_LEXICAL, "number cannot start with 0");}
                            	ungetc(next_char, stdin);
                            	state = S_INT;
                                break;
                            }
                            state = S_INT;
                        }
                        else{
                            errorMsg(ERR_LEXICAL, "unknown lexem");
                        }
                        break;
                }
                break;
            
            case S_ASSIGN: // ==
                state = START;
                if(next_char == '=')
                    return EQ;
                ungetc(next_char, stdin);
                return ASSIGN;
                break;

            case S_L: // <=
                state = START;
                if(next_char == '=')
                    return LEQ;
                ungetc(next_char, stdin);
                return LESS;
                break;

            case S_G: // >=
                state = START;
                if(next_char == '=')
                    return GEQ;
                ungetc(next_char, stdin);
                return GREAT;
                break;

            case S_NEG: //!=
                state = START;
                if(next_char == '=')
                    return NEQ;
                errorMsg(ERR_LEXICAL, "\"!\" is invalid lexem");
                break;

            case S_VAR_DEF: // :=
                state = START;
                if(next_char == '=')
                    return VAR_DEF;
                errorMsg(ERR_LEXICAL, "\":\" is invalid lexem");
                break;

            case S_DIV:
                if(next_char == '/'){ //
                    state = L_COM;
                }else if(next_char == '*'){ // /*
                    state = S_BLOCK;
                }else{ // /
                    state = START;
                    ungetc(next_char, stdin); 
                    return DIV;
                }
                break;

            case L_COM:
                if(next_char == '\n'){ //end of comment
                    state = START;
                    ungetc(next_char, stdin);
                }else if(next_char == EOF){
                    state = START;
                    ungetc(next_char, stdin); //EOF in line commentar
                }else{} //ignor comment
                break;

            case S_ID:
                if(isalnum(next_char) || next_char == '_'){
                   if(strAddChar(tokenStr, next_char)){
                        errorMsg(ERR_INTERNAL, "str.c allocation error");} 
                }else{
                    state = START;
                    ungetc(next_char, stdin);
                    return ID;
                }
                break;
            case S_KW:
                if(isalpha(next_char) || isdigit(next_char)){
                    if(strAddChar(tokenStr, next_char)){
                        errorMsg(ERR_INTERNAL, "str.c allocation error");}
                }else if(next_char == '_'){ //no _ in KW
                    if(strAddChar(tokenStr, next_char)){
                        errorMsg(ERR_INTERNAL, "str.c allocation error");}
                    state = S_ID;
                    //ungetc(next_char, stdin);
                }else{
                    state = START;
                    ungetc(next_char, stdin);
                    /* key-words */
                    if(strcmp(tokenStr->str, "func") == 0){
                        strClear(tokenStr);return KW_FUNC;}
                    else if(strcmp(tokenStr->str, "return") == 0){
                        strClear(tokenStr);return KW_RETURN;}
                    else if(strcmp(tokenStr->str, "float64") == 0){
                        strClear(tokenStr);return KW_FLOAT64;} 
                    else if(strcmp(tokenStr->str, "int") == 0){
                        strClear(tokenStr);return KW_INT;}
                    else if(strcmp(tokenStr->str, "string") == 0){
                        strClear(tokenStr);return KW_STRING;} 
                    else if(strcmp(tokenStr->str, "for") == 0){
                        strClear(tokenStr);return KW_FOR;}
                    else if(strcmp(tokenStr->str, "if") == 0){
                        strClear(tokenStr);return KW_IF;}
                    else if(strcmp(tokenStr->str, "else") == 0){
                        strClear(tokenStr);return KW_ELSE;}
                    else if(strcmp(tokenStr->str, "package") == 0){
                        strClear(tokenStr);return KW_PACKAGE;}
                    /* main function */
                    else if(strcmp(tokenStr->str, "main") == 0){
                        strClear(tokenStr);return MAIN;} 
                    /* build-in functions */
                    else if(strcmp(tokenStr->str, "inputs") == 0){
                        strClear(tokenStr);return F_INPUTS;}
                    else if(strcmp(tokenStr->str, "inputi") == 0){
                        strClear(tokenStr);return F_INPUTI;}
                    else if(strcmp(tokenStr->str, "inputf") == 0){
                        strClear(tokenStr);return F_INPUTF;}
                    else if(strcmp(tokenStr->str, "print") == 0){
                        strClear(tokenStr);return F_PRINT;}
                    else if(strcmp(tokenStr->str, "int2float") == 0){
                        strClear(tokenStr);return F_INT2FLOAT;}
                    else if(strcmp(tokenStr->str, "float2int") == 0){
                        strClear(tokenStr);return F_FLOAT2INT;}
                    else if(strcmp(tokenStr->str, "len") == 0){
                        strClear(tokenStr);return F_LEN;}
                    else if(strcmp(tokenStr->str, "substr") == 0){
                        strClear(tokenStr);return F_SUBSTR;}
                    else if(strcmp(tokenStr->str, "ord") == 0){
                        strClear(tokenStr);return F_ORD;}
                    else if(strcmp(tokenStr->str, "chr") == 0){
                        strClear(tokenStr);return F_CHR;}
                    /* ID */
                    else{return ID;}
                }
                break;
            case S_INT:
                if(next_char == '.'){
                    dot = true; //NUM.something
                    if(strAddChar(tokenStr, next_char)){
                        errorMsg(ERR_INTERNAL, "str.c allocation error");}
                    state = S_FLOAT;
                }else if(next_char == 'e' || next_char == 'E'){
                    exp = true; //NUMesomething
                    if(strAddChar(tokenStr, next_char)){
                        errorMsg(ERR_INTERNAL, "str.c allocation error");}
                    state = S_FLOAT;
                }else if(isdigit(next_char)){
                    if(strAddChar(tokenStr, next_char)){
                        errorMsg(ERR_INTERNAL, "str.c allocation error");}
                }else{
                    state = START;
                    ungetc(next_char, stdin);
                    return T_INT;
                }
                break;
            case S_FLOAT:
                if(dot == true && exp == false){
                    if(isdigit(next_char)){ //only digit allowed after decimal point
                        if(strAddChar(tokenStr, next_char)){
                            errorMsg(ERR_INTERNAL, "str.c allocation error");}
                    }else{
                        errorMsg(ERR_LEXICAL, "no decimal after decimal point");
                    }
                    state = S_FLOAT2;
                }else if(exp == true){
                    if(isdigit(next_char)){
                        if(strAddChar(tokenStr, next_char)){
                            errorMsg(ERR_INTERNAL, "str.c allocation error");}
                        state = S_FLOAT_E;
                    }else if((next_char == '+' || next_char == '-')&& sign==false){
                        sign = true; //set sign
                        if(strAddChar(tokenStr, next_char)){
                            errorMsg(ERR_INTERNAL, "str.c allocation error");}
                        next_char = getc(stdin);
                        if(isdigit(next_char)){
                            if(strAddChar(tokenStr, next_char)){
                                errorMsg(ERR_INTERNAL, "str.c allocation error");}
                            state = S_FLOAT_E;
                        }
                    }else{errorMsg(ERR_LEXICAL, "incorrect exponent");}
                }
                break;
            case S_FLOAT2:
                if(isdigit(next_char)){
                    if(strAddChar(tokenStr, next_char)){
                            errorMsg(ERR_INTERNAL, "str.c allocation error");}
                    //
                }else if(next_char == 'e' || next_char == 'E'){
                    exp = true;
                    if(strAddChar(tokenStr, next_char)){
                            errorMsg(ERR_INTERNAL, "str.c allocation error");}
                    state = S_FLOAT;
                }else{
                    state = START;
                    ungetc(next_char, stdin);
                    return T_FLOAT;
                }
                break;
            case S_FLOAT_E:
                if(isdigit(next_char)){
                   if(strAddChar(tokenStr, next_char)){
                        errorMsg(ERR_INTERNAL, "str.c allocation error");} 
                }else if(next_char == '.'){
                    errorMsg(ERR_LEXICAL, "decimal point after exponent");
                }else{
                    state = START;
                    ungetc(next_char, stdin);
                    return T_FLOAT;
                }
                break;
            case S_STRING:
                if(next_char == '"'){
                    state = START;
                    return T_STRING;
                }else if(next_char == '\n' || next_char == EOF){
                    errorMsg(ERR_LEXICAL, "EOF or EOL before end of string");
                }else if(next_char <= 31){
                    errorMsg(ERR_LEXICAL, "chars <= must be written in escape sequence");
                }else if(next_char == '\\'){
                    state = S_ESC;   //hexadecimal sequence
                }else{
                    if(strAddChar(tokenStr, next_char)){
                        errorMsg(ERR_INTERNAL, "str.c allocation error");}
                }
                break;
            case S_BLOCK:
                if(next_char == '*'){
                    next_char = getc(stdin);
                    if(next_char == '/'){
                        state = START;
                        break;
                    }else{
                    //   
                    }
                }else if(next_char == EOF){
                    errorMsg(ERR_LEXICAL, "unfinished block comment");
                }
                break;
            case S_ESC:
                if(next_char == '\n' || next_char == EOF){
                    errorMsg(ERR_LEXICAL, "EOL or EOF before end of string"); //EOF or EOL before end of string
                }else if(next_char == '"'){
                    if(strAddChar(tokenStr, '"')){
                        errorMsg(ERR_INTERNAL, "str.c allocation error");}
                        state = S_STRING;
                }else if(next_char == 'n'){
                    if(strAddChar(tokenStr, '\n')){
                        errorMsg(ERR_INTERNAL, "str.c allocation error");}
                        state = S_STRING;
                }else if(next_char == 't'){
                    if(strAddChar(tokenStr, '\t')){
                        errorMsg(ERR_INTERNAL, "str.c allocation error");}
                        state = S_STRING;
                }else if(next_char == '\\'){
                    if(strAddChar(tokenStr, '\\')){
                        errorMsg(ERR_INTERNAL, "str.c allocation error");}
                        state = S_STRING;
                }else if(next_char == 'x'){
                    state = S_HEX;
                }else{
                    errorMsg(ERR_LEXICAL, "incorrect escape sequence");
                }
                break;

            case S_HEX:
                //todo
                if(isxdigit(next_char)){
                    int hex1 = hex_to_int(next_char);
                    next_char = getc(stdin);
                    if(isxdigit(next_char)){
                        int hex2 = hex_to_int(next_char);
                        int hexadecimal = hex1 * 16 + hex2;
                        if(hexadecimal <= 0){
                            errorMsg(ERR_LEXICAL, "incorrect hexadecimal escape sequence");
                        }
                        if(strAddChar(tokenStr, hexadecimal)){
                            errorMsg(ERR_INTERNAL, "str.c allocation error");}
                            state = S_STRING;
                    }else{
                        errorMsg(ERR_LEXICAL, "incorrect hexadecimal escape sequence");
                    }

                }else{
                    errorMsg(ERR_LEXICAL, "incorrect hexadecimal escape sequence");
                }
                break;

            default:
                break;
        }
    }
}

int hex_to_int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    else if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    return -1;
}