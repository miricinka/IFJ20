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

int get_next_char() {

    char next_char;


    while (1){

        next_char = getc(stdin);

        switch (state){

            case START:

                // Ignore all white chars
                if (isspace(next_char)){
                    state = START;
                }

                // Finite states

                switch (next_char){

                    case '+':
                        return ADD;
                    case '-':
                        return SUB;
                    case '*':
                        return MUL;
                    case '(':
                        return L_BRACKET;
                    case ')':
                        return R_BRACKET;
                    case ',':
                        return COMMA;
                    case '\n':
                        return EOL;
                    case EOF:
                        return ENDFILE;

                // Possible non-finite states 

                    case '/':
                        state = S_DIV;
                        break;
                    case '=':
                        state = S_EQ;
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
            

            default:
                break;
        }
    }
}