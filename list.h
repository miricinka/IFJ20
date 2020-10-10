/***************************
 * PROJECT: 
 *  IFJ20 - Compiler for imperative programming language IFJ20
 * 
 * UNIVERSITY: 
 *  Faculty of Information Technology, Brno University of Technology
 * 
 * FILE:
 *  list.h
 * 
 * DESCRIPTION:
 *  List functions
 * 
 * AUTHORS:
 *  Hrúz Tomáš    <xhruzt00@stud.fit.vutbr.cz>
 *  
*/

// typy jednotlivych instrukci
#define ID          0

/* Key words */

#define I_KW_FUNC     1
#define I_KW_RETURN   2
#define I_KW_FLOAT64  3
#define I_KW_INT      4
#define I_KW_STRING   5
#define I_KW_FOR      6
#define I_KW_IF       7
#define I_KW_ELSE     8
#define I_KW_PACKAGE  9

/* Operators*/

#define I_ADD         10  //  +
#define I_SUB         11  //  -
#define I_DIV         12  //  /
#define I_MUL         13  //  *

#define I_EQ          20  //  ==
#define I_NEQ         21  //  !=
#define I_LESS        22  //  <
#define I_GREAT       23  //  >
#define I_LEQ         24  //  <=
#define I_GEQ         25  //  >=

/* Types */

#define I_T_INT        30
#define I_T_FLOAT      31
#define I_T_STRING     32 

/* Built in functions */

#define I_F_PRINT     33
#define I_F_LEN       34
#define I_F_SUBSTR    35
#define I_F_ORD       36
#define I_F_CHR       37
#define I_F_INT2FLOAT 38
#define I_F_FLOAT2INT 39


/* Special Characters */

#define I_L_PAR       41  //  (
#define I_R_PAR       42  //  )
#define I_L_BR        43  //  {
#define I_R_BR        44  //  }
#define I_ASSIGN      45  //  =
#define I_COMMA       46  //  ,
#define I_SEMICOL     47  //  ;
#define I_VAR_DEF     48  //  :=

#define I_EOL         50
#define I_ENDFILE     51

/* Non-finate states */

#define I_START       60
#define I_S_ASSIGN    61
#define I_S_DIV       62
#define I_S_L         63
#define I_S_G         64
#define I_S_NEG       65
#define I_S_ID        66
#define I_S_KW        67
#define I_S_INT       68
#define I_S_FLOAT     69
#define I_S_FLOAT2    70
#define I_S_FLOAT_E   71
#define I_S_STRING    72
#define I_L_COM       73
#define I_S_VAR_DEF   74
#define I_S_BLOCK     75
#define I_S_ESC       76
#define I_S_HEX       77

typedef struct
{
  int instType;  // typ instrukce
  void *addr1; // adresa 1
  void *addr2; // adresa 2
  void *addr3; // adresa 3
} tInstr;

typedef struct listItem
{
  tInstr Instruction;
  struct listItem *nextItem;
} tListItem;
    
typedef struct
{
  struct listItem *first;  // ukazatel na prvni prvek
  struct listItem *last;   // ukazatel na posledni prvek
  struct listItem *active; // ukazatel na aktivni prvek
} tListOfInstr;

void listInit(tListOfInstr *L);
void listFree(tListOfInstr *L);
void listInsertLast(tListOfInstr *L, tInstr I);
void listFirst(tListOfInstr *L);
void listNext(tListOfInstr *L);
void listGoto(tListOfInstr *L, void *gotoInstr);
void *listGetPointerLast(tListOfInstr *L);
tInstr *listGetData(tListOfInstr *L);
