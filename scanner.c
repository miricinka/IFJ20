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
    //tokenStr = NULL;


    while (1){

        next_char = getc(stdin);

        switch (state){

            case START:

                // Ignore all white chars
                if (isspace(next_char)){
                    state = START;
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
                    case '\'': //TODO
                        state = S_STRING;
                        break;
                    case '_':
                        if(strAddChar(tokenStr, next_char)){
                            exit(ERR_INTERNAL);}
                        state = S_ID;

                    default:
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


            default:
                break;
        }
    }
}