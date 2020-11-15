#include "symtable.h"
#include <string.h>
#include "str.h"

#define TEST_VAR_TREE  0
#define TEST_FUN_LIST  1
#define TEST_FUN_TREE  0

/* Variable tree testing functions */

bool test_BSTInit(varNode *TempTree)			{
	BSTInit(TempTree);
    printVarTree(*TempTree);
	return true;
}

bool test_BSTSearch (varNode TempTree, string Key)	{
	varNode FOUND;
	FOUND=BSTSearch(TempTree, Key);

	if(!FOUND)
		printf("Polozka nebyla nalezena !\n");
	else {
		printf("Polozka byla nalezena !\n");
	}
	return true;			

}

bool test_isDeclared (varNode TempTree, string Key)	{
	bool FOUND;
	FOUND=isDeclared(TempTree, Key);

	if(!FOUND)
		printf("Promena %s NENI deklarovana.\n", Key.str);
	else {
		printf("Promena %s JE deklarovana.\n", Key.str);
	}
	return true;			
}

bool test_getType (varNode TempTree, string Key)	{
	int FOUND;
	FOUND=getType(TempTree, Key);

	if(FOUND == 666)
		printf("Promena %s NENI deklarovana.\n", Key.str);
	else {
		printf("Promena %s ma typ %d\n", Key.str, FOUND);
	}
	return true;			
}

bool test_BSTInsert (varNode *TempTree, string Key, int Type)		{
	BSTInsert(TempTree, Key, Type);	
	printVarTree(*TempTree);
	return true;
}

int test_BSTDispose(varNode* TempTree)		{
	BSTDispose(TempTree);
	printVarTree(*TempTree);
	return true;

}

/* Function list testing functions */

void testFunListInit(funList TEMPLIST){
	funListInit(&TEMPLIST);
	printFunList(TEMPLIST);
}

void testFunListAdd(funList *TEMPLIST, int val, int order){
	funListAdd(TEMPLIST, val, order);
	printFunList(*TEMPLIST);
}
/* .......................... sekce volani jednotlivych testu .............................*/ 

// K must be a string

int main(){

if (TEST_VAR_TREE){

		varNode TempTree;			/* deklarace testovaci promenne */ 

		int Type_of_Insert = 0;

		string K; strInit(&K);


		printf("Binarni vyhledavaci strom pro promene\n");
		printf("=========================\n");

		printf("[TEST01]\n");
		printf("Test inicializace....\n");
		test_BSTInit(&TempTree);

		printf("[TEST02]\n");
		printf("Zkusime zrusit strom\n");
		printf("~~~~~~~~~~~~~~~~~~~~\n");
		test_BSTDispose(&TempTree);

		printf("[TEST03]\n");
		printf("Pokusime se vyhledat polozku s klicem 'A' -- nenalezne se\n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

		strAddChars(&K,"H");

		test_BSTSearch(TempTree,K);

		printf("[TEST04]\n");
		printf("Vlozime prvni prvek (H,1,T_INT)\n");

		Type_of_Insert = T_INT;

		test_BSTInsert(&TempTree,K, Type_of_Insert);

		printf("[TEST05]\n");
		printf("Pokusime se vyhledat polozku s klicem H\n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		test_BSTSearch(TempTree,K);

		printf("[TEST06]\n");
		printf("Vlozime prvek (H,8) - pouze zmena hodnoty\n");   
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		Type_of_Insert = T_FLOAT;
		test_BSTInsert(&TempTree,K, Type_of_Insert);
		Type_of_Insert = T_INT;

		printf("[TEST07]\n");
		printf("Vlozime dalsi prvky a vytvorime tak symetricky binarni strom \n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");



		strClear(&K);
		strAddChars(&K,"A");
		BSTInsert(&TempTree,K, Type_of_Insert);

		strClear(&K);
		strAddChars(&K,"U");
		BSTInsert(&TempTree,K, Type_of_Insert);

		strClear(&K);
		strAddChars(&K,"PP");
		BSTInsert(&TempTree,K, Type_of_Insert);

		Type_of_Insert = T_STRING;

		strClear(&K);
		strAddChars(&K,"abraham");
		BSTInsert(&TempTree,K, Type_of_Insert);


		Type_of_Insert = T_INT;

		strClear(&K);
		strAddChars(&K,"1");
		BSTInsert(&TempTree,K, Type_of_Insert);

		strClear(&K);
		strAddChars(&K,"B");
		BSTInsert(&TempTree,K, Type_of_Insert);

		strClear(&K);
		strAddChars(&K,"az");
		BSTInsert(&TempTree,K, Type_of_Insert);

		printVarTree(TempTree);

		printf("[TEST08]\n");
		printf("Pokusime se vyhledat polozky s klici 'az', 'PP'\n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

		strClear(&K);
		strAddChars(&K,"az");
		test_BSTSearch(TempTree,K);

		strClear(&K);
		strAddChars(&K,"PP");
		test_BSTSearch(TempTree,K);

		printf("[TEST09]\n");
		printf("Pokusime se vyhledat polozky s klici '1', 'B'\n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

		strClear(&K);
		strAddChars(&K,"1");
		test_BSTSearch(TempTree,K);

		strClear(&K);
		strAddChars(&K,"B");
		test_BSTSearch(TempTree,K);


		printf("[TEST10]\n");
		printf("Je promena abraham deklarovana? \n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");

		strClear(&K);
		strAddChars(&K,"abraham");
		test_isDeclared(TempTree,K);


		printf("[TEST11]\n");
		printf("Je promena pes deklarovana?\n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		strClear(&K);
		strAddChars(&K,"pes");
		test_isDeclared(TempTree,K);


		printf("[TEST12]\n");
		printf("Zjistime typ abraham\n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		strClear(&K);
		strAddChars(&K,"abraham");
		test_getType(TempTree,K);

		printf("[TEST13]\n");
		printf("Zjistime typ B\n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");
		strClear(&K);
		strAddChars(&K,"B");
		test_getType(TempTree,K);

		printf("[TEST20]\n");
		printf("Nakonec zrusime cely strom\n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		test_BSTDispose(&TempTree);

		strFree(&K);
		printf("------------------------------ konec -------------------------------------\n");
	}

	if (TEST_FUN_LIST){
		funList TEMPLIST;

		printf("Jednosmerne vazany seznam\n");
		printf("=========================\n");

		printf("[TEST01]\n");
		printf("Test inicializace....\n");
		testFunListInit(TEMPLIST);

		printf("\n[TEST02]\n");
    	printf("Zavoláme 2x funListAdd\n");
    	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");	
		testFunListAdd(&TEMPLIST, T_INT, 1);
		testFunListAdd(&TEMPLIST, T_FLOAT , 2);
		testFunListAdd(&TEMPLIST, T_STRING, 3);
		printf("------------------------------ konec -------------------------------------\n");
	}
	








	if (TEST_FUN_TREE){
		printf("Binarni vyhledavaci strom pro funkce\n");
		printf("=========================\n");

		printf("------------------------------ konec -------------------------------------\n");
	}

	return(0);
}
