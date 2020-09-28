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
                            exit(ERR_INTERNAL);}
                        state = S_ID;
                        break;

                    default:
                        if(isalpha(next_char)){
                            if(strAddChar(tokenStr, next_char)){
                                exit(ERR_INTERNAL);}
                            state = S_KW;
                        }else if(isdigit(next_char)){
                            if(strAddChar(tokenStr, next_char)){
                                exit(ERR_INTERNAL);}
                            state = S_INT;
                        }
                        else{
                            //printf("IM HERE, new char '%c'\n", next_char);
                            exit(ERR_LEXICAL); //unknown lexem
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
                exit(ERR_LEXICAL); //invalid lexem start

            case S_VAR_DEF: // :=
                state = START;
                if(next_char == '=')
                    return VAR_DEF;
                exit(ERR_LEXICAL); //invalid lexem start

            case S_DIV:
                if(next_char == '/'){ //
                    state = L_COM;
                    break;
                }
                state = START;
                ungetc(next_char, stdin); // /
                return DIV;
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
                        exit(ERR_INTERNAL);} 
                }else{
                    state = START;
                    ungetc(next_char, stdin);
                    return ID;
                }
                break;
            case S_KW:
                if(isalpha(next_char) || isdigit(next_char)){
                    if(strAddChar(tokenStr, next_char)){
                        exit(ERR_INTERNAL);}
                }else if(next_char == '_'){ //no _ in KW
                    if(strAddChar(tokenStr, next_char)){
                        exit(ERR_INTERNAL);}
                    state = S_ID;
                    //ungetc(next_char, stdin);
                }else{
                    state = START;
                    ungetc(next_char, stdin);
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
                    else{return ID;}
                }
                break;
            case S_INT:
                if(next_char == '.'){
                    dot = true; //NUM.something
                    if(strAddChar(tokenStr, next_char)){
                        exit(ERR_INTERNAL);}
                    state = S_FLOAT;
                }else if(next_char == 'e' || next_char == 'E'){
                    exp = true; //NUMesomething
                    if(strAddChar(tokenStr, next_char)){
                        exit(ERR_INTERNAL);}
                    state = S_FLOAT;
                }else if(isdigit(next_char)){
                    if(strAddChar(tokenStr, next_char)){
                        exit(ERR_INTERNAL);}
                    //
                }else if(isalpha(next_char)){
                    exit(ERR_LEXICAL); //character is not allowed in number
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
                            exit(ERR_INTERNAL);}
                    }else{
                        exit(ERR_LEXICAL); //no decimal after decimal point
                    }
                    state = S_FLOAT2;
                }else if(exp == true){
                    if(isdigit(next_char)){
                        if(strAddChar(tokenStr, next_char)){
                            exit(ERR_INTERNAL);}
                        state = S_FLOAT_E;
                    }else if((next_char == '+' || next_char == '-')&& sign==false){
                        sign = true; //set sign
                        if(strAddChar(tokenStr, next_char)){
                            exit(ERR_INTERNAL);}
                        next_char = getc(stdin);
                        if(isdigit(next_char)){
                            if(strAddChar(tokenStr, next_char)){
                                exit(ERR_INTERNAL);}
                            state = S_FLOAT_E;
                        }
                    }else{exit(ERR_LEXICAL);} //bad exponent
                }
                break;
            case S_FLOAT2:
                if(isdigit(next_char)){
                    if(strAddChar(tokenStr, next_char)){
                            exit(ERR_INTERNAL);}
                    //
                }else if(next_char == 'e' || next_char == 'E'){
                    exp = true;
                    if(strAddChar(tokenStr, next_char)){
                            exit(ERR_INTERNAL);}
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
                        exit(ERR_INTERNAL);} 
                }else if(next_char == '.'){
                    exit(ERR_LEXICAL); //decimal point after exponent
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
                    exit(ERR_LEXICAL); //EOF EOL before end of string
                }else if(next_char <= 31){
                    exit(ERR_LEXICAL); //chars <31 must use escape sequence
                }else{
                    if(strAddChar(tokenStr, next_char)){
                        exit(ERR_INTERNAL);}
                }

            default:
                break;
        }
    }
}
