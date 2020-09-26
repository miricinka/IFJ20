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
    tokenStr = NULL;


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

                    case '/':
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
                    case '\'':
                        state = S_STRING;
                        break;

                    default:
                        break;
                }
                break;
            
            case S_ASSIGN:
                state = START;
                if(next_char == '=')
                    return EQ;
                ungetc(next_char, stdin);
                return ASSIGN;
                break;

            default:
                break;
        }
    }
}
