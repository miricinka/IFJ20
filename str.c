/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  str.c
 * 
 * DESCRIPTION:
 *  Lexical analysis
 * 
 * AUTHORS:
 *  Kolaříková Mirka    <xkolar76@stud.fit.vutbr.cz>
 *  Žovinec Martin      <xzovin00@stud.fit.vutbr.cz>
 *  Taken from reference compiler by VUT FIT in Brno 
*/

#include "str.h"

#define STR_LEN_INC 8
// konstanta STR_LEN_INC udava, na kolik bytu provedeme pocatecni alokaci pameti
// pokud nacitame retezec znak po znaku, pamet se postupne bude alkokovat na
// nasobky tohoto cisla 

#define STR_ERROR   1
#define STR_SUCCESS 0

int strInit(string *s)
//creates new string
{
   if ((s->str = (char*) malloc(sizeof(char) * STR_LEN_INC)) == NULL) //*size of char
      return STR_ERROR;
   s->str[0] = '\0';
   s->length = 0;
   s->allocSize = STR_LEN_INC;
   return STR_SUCCESS;
}

void strFree(string *s)
//frees string from memory
{
   free(s->str);
}

void strClear(string *s)
// clears contents of string
{
   s->str[0] = '\0';
   s->length = 0;
}

int strAddChar(string *s1, char c)
// adds one char at the end of string
{   
   if ((*s1).length + 1 >= s1->allocSize)
   {
      // memory not sufficient, reallocation is requiered
      if ((s1->str = (char*) realloc(s1->str, s1->length + STR_LEN_INC)) == NULL)
         return STR_ERROR;
      s1->allocSize = s1->length + STR_LEN_INC;
   }
   s1->str[s1->length] = c;
   s1->length++;
   s1->str[s1->length] = '\0';
   return STR_SUCCESS;
}

int strAddChars(string *s1, char *c) {
    int i = 0;
    while (c[i] != '\0') {
        if (strAddChar(s1, c[i]) == STR_ERROR) {
            return STR_ERROR;
        }
        i++;
    }
    return STR_SUCCESS;
}

int strCopyString(string *s1, string *s2)
// copies string s2 to string s1
{
 
   int newLength = s2->length;
   if (newLength >= s1->allocSize)
   {
      // memory not sufficient, reallocation is requiered
      if ((s1->str = (char*) realloc(s1->str, newLength + 1)) == NULL)
         return STR_ERROR;
      s1->allocSize = newLength + 1;
   }
   strcpy(s1->str, s2->str);
   s1->length = newLength;
 
   return STR_SUCCESS;

}

int strCmpString(string *s1, string *s2)
// compares two string and returns result
{
   return strcmp(s1->str, s2->str);
}

int strCmpConstStr(string *s1, char* s2)
// compares our string with constant string
{
   return strcmp(s1->str, s2);
}

char *strGetStr(string *s)
// returns text of string
{
   return s->str;
}

int strGetLength(string *s)
// returns lenght of string
{
   return s->length;
}
