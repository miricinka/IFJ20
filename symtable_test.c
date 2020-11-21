#include "symtable.h"
#include "str.h"
#include "error.h"

#include <string.h>

/* Test control, choose which tests to run by assigning 1 to it*/

#define TEST_VAR_STACK 0
#define TEST_VAR_TREE  0
#define VAR_TREE_STACK 0

#define TEST_FUN_LIST  0
#define TEST_FUN_TREE  0
#define TEST_TREE_LIST 1


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

void test_BSTInsert (varNode *TempTree, char *Key, int Type, int scope)		{
	string name; strInit(&name);
	strAddChars(&name,Key);
	BSTInsert(TempTree, name, Type, scope);	
	strFree(&name);
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

void testFunListSearch(funList *TEMPLIST, int order){
	funListElement ListElement;
	ListElement = funListSearch (TEMPLIST, order);
	if(ListElement){
		printf("Prvek %d BYL nalezen a ma typ %d\n",order,ListElement->type);
	}else{
		printf("Prvek NEBYL nalezen\n");
	}

}


/* Function Binary tree testing functions */

void testFunctionTreeSearch(funNode testTree, string key){
	if(funSearch (&testTree,  key)){
		printf("Funkce BYLA nalezena\n");
	}else{
		printf("Funkce NEBYLA nalezena\n");
	}
}

void testStackEmpty(varNode varTree){
	if(varTree->varStack == NULL){
		printf("Stack JE prazdny!\n");
	} else {
		printf("Stack NENI prazdny!\n");
	}
}

/* .......................... sekce volani jednotlivych testu .............................*/ 

// K must be a string

int main(){
if (TEST_VAR_STACK){
	varNode varTree; BSTInit(&varTree);
	string name; strInit(&name);
	strAddChars(&name,"meh");
	BSTInsert(&varTree, name, T_INT, 0);
	
	printf("Binarni vyhledavaci strom pro promene\n");
	printf("=========================\n");
	
	printf("[TEST01]\n");
	printf("Push 1\n");
	printVarTree(varTree);

	printf("[TEST02]\n");
	printf("Push 2\n");
	stackPush(&varTree, T_STRING, 1);
	printVarTree(varTree);

	printf("[TEST03]\n");
	printf("Pop 1\n");
	stackPop(&varTree);

	printf("[TEST04]\n");
	printf("Prazdny stack?\n");
	testStackEmpty(varTree);

	printf("[TEST05]\n");
	printf("Pop 2\n");
	stackPop(&varTree);
	
	printf("[TEST06]\n");
	printf("Prazdny stack?\n");
	testStackEmpty(varTree);

	printf("[TEST07]\n");
	printf("Stack delete\n");
	stackPush(&varTree, T_STRING, 1);
	stackPush(&varTree, T_STRING, 1);
	stackPush(&varTree, T_STRING, 1);

	stackDelete(&varTree);

	testStackEmpty(varTree);

	strFree(&name);
	BSTDispose(&varTree);
}

if (TEST_VAR_TREE){

		varNode TempTree;			/* deklarace testovaci promenne */ 

		int Type_of_Insert = 0;
		int scope = 1;

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
		

		test_BSTInsert(&TempTree,"H", Type_of_Insert,scope);
		printVarTree(TempTree);

		printf("[TEST05]\n");
		printf("Pokusime se vyhledat polozku s klicem H\n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		test_BSTSearch(TempTree,K);

		printf("[TEST06]\n");
		printf("Vlozime prvek (H,8) - pouze zmena hodnoty\n");   
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		Type_of_Insert = T_FLOAT;
		scope++;
		test_BSTInsert(&TempTree,"H", Type_of_Insert,scope);
		Type_of_Insert = T_INT;
		printVarTree(TempTree);

		printf("[TEST07]\n");
		printf("Vlozime dalsi prvky a vytvorime tak symetricky binarni strom \n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");
		
		test_BSTInsert(&TempTree,"A", Type_of_Insert,scope);
		test_BSTInsert(&TempTree,"U", Type_of_Insert,scope);
		test_BSTInsert(&TempTree,"PP", Type_of_Insert,scope);

		Type_of_Insert = T_STRING;

		test_BSTInsert(&TempTree,"abraham", Type_of_Insert,scope);

		Type_of_Insert = T_INT;

		test_BSTInsert(&TempTree,"1", Type_of_Insert,scope);
		test_BSTInsert(&TempTree,"B", Type_of_Insert,scope);
		test_BSTInsert(&TempTree,"az", Type_of_Insert,scope);
		test_BSTInsert(&TempTree,"PPE", Type_of_Insert,scope);
		test_BSTInsert(&TempTree,"aa", Type_of_Insert,scope);

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
		strAddChars(&K,"A");
		test_getType(TempTree,K);

		printf("[TEST13]\n");
		printf("Zjistime typ B\n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n");
		
		strClear(&K);
		strAddChars(&K,"B");
		test_getType(TempTree,K);

		printf("[TEST14]\n");
		printf("Smažeme prvek U\n");
		printf("Strom pred mazanim:\n");
		printVarTree(TempTree);
		strClear(&K);
		strAddChars(&K,"U");
		BSTDelete(&TempTree,K);

		printf("Strom po mazani:\n");
		printVarTree(TempTree);

		printf("[TEST15]\n");
		printf("Smažeme prvek abraham\n");
		printf("Strom pred mazanim:\n");
		printVarTree(TempTree);
		strClear(&K);
		strAddChars(&K,"abraham");
		BSTDelete(&TempTree,K);

		printf("Strom po mazani:\n");
		printVarTree(TempTree);


		printf("[TEST20]\n");
		printf("Nakonec zrusime cely strom\n");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		test_BSTDispose(&TempTree);

		strFree(&K);
	}

	if (VAR_TREE_STACK){
		varNode varTree; BSTInit(&varTree);
		string name; strInit(&name);
		int scope = 1;
		int type = T_INT;


		printf("Strom promenych se zasobnikem\n");
		printf("=========================\n");


		printf("[TEST01]\n");
		printf("Vytvoreni stromu\n");

		test_BSTInsert(&varTree,"H", type,scope);
		test_BSTInsert(&varTree,"A", type,scope);
		test_BSTInsert(&varTree,"U", type,scope);
		test_BSTInsert(&varTree,"PP", type,scope);

		type = T_STRING;

		test_BSTInsert(&varTree,"abraham", type,scope);

		type = T_FLOAT;

		test_BSTInsert(&varTree,"1", type,scope);
		test_BSTInsert(&varTree,"B", type,scope);
		test_BSTInsert(&varTree,"az", type,scope);
		test_BSTInsert(&varTree,"PPE", type,scope);
		test_BSTInsert(&varTree,"aa", type,scope);

		printVarTree(varTree);


		printf("[TEST02]\n");
		printf("Vkladani \"az\" a \"aa\" jako string do vyssiho scope \n");
		scope++;
		type = T_STRING;
		test_BSTInsert(&varTree,"az", type,scope);
		test_BSTInsert(&varTree,"aa", type,scope);
		test_BSTInsert(&varTree,"aaa", type,scope);

		printVarTree(varTree);

		printf("[TEST03]\n");
		printf("Mazani Scope 2\n");
		scope--;
		BSTScopeDelete(&varTree,scope);

		printVarTree(varTree);

		printf("[TEST04]\n");
		printf("Pokus o redeklaraci ve stejném scope\n");
		test_BSTInsert(&varTree,"az", type,scope);

		printVarTree(varTree);

		/* Uvolneni parametru */

		strFree(&name);
		BSTDispose(&varTree);
	}
	
	if (TEST_FUN_LIST){
		funList TEMPLIST;

		printf("Jednosmerne vazany seznam\n");
		printf("=========================\n");

		printf("[TEST01]\n");
		printf("Test inicializace....\n");
		testFunListInit(TEMPLIST);

		printf("\n[TEST02]\n");
    	printf("Zavoláme 3x funListAdd\n");
    	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");	
		testFunListAdd(&TEMPLIST, T_INT, 1);
		testFunListAdd(&TEMPLIST, T_FLOAT , 2);
		testFunListAdd(&TEMPLIST, T_STRING, 3);

		printf("\n[TEST03]\n");
    	printf("Zkusíme najít všechny prvky\n");
    	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");	
		testFunListSearch(&TEMPLIST, 1);
		testFunListSearch(&TEMPLIST, 2);
		testFunListSearch(&TEMPLIST, 3);

		printf("\n[TEST03]\n");
    	printf("Zkusíme najít prvek co neexistuje\n");
    	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");	
		testFunListSearch(&TEMPLIST, 4);

		printf("\n[TEST04]\n");
		printf("Smažeme celý list\n");
    	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");	
		
		funListDelete(&TEMPLIST);
		printFunList(TEMPLIST);
	}

	if (TEST_FUN_TREE){
		funNode funTree;
		string name; strInit(&name);
		varNode varTree; BSTInit(&varTree);
		
		printf("Binarni vyhledavaci strom pro funkce\n");
		printf("=========================\n");

		printf("[TEST01]\n");
		printf("Test inicializace....\n");
		funInit(&funTree);
		printFunTree(funTree);

		printf("[TEST02]\n");
		printf("Test mazání stromu\n");
		funDisposeTree(&funTree);
		printFunTree(funTree);

		printf("[TEST03]\n");
		printf("Test přidání prvku do stromu\n");
		strAddChars(&name,"main");
		addFunToTree(&funTree,name);
		printFunTree(funTree);

		printf("[TEST04]\n");
		printf("Test přidání více prvků do stromu\n");
		strClear(&name);
		strAddChars(&name,"pain");
		addFunToTree(&funTree,name);

		strClear(&name);
		strAddChars(&name,"gain");
		addFunToTree(&funTree,name);

		strClear(&name);
		strAddChars(&name,"protein");
		addFunToTree(&funTree,name);

		strClear(&name);
		strAddChars(&name,"papej");
		addFunToTree(&funTree,name);
		
		strClear(&name);
		strAddChars(&name,"sane");
		addFunToTree(&funTree,name);

		strClear(&name);
		strAddChars(&name,"B");
		addFunToTree(&funTree,name);

		printFunTree(funTree);

		printf("[TEST05]\n");
		printf("Test hledání prvků ve stromu\n");
		
		strClear(&name);
		strAddChars(&name,"protein");
		testFunctionTreeSearch(funTree, name);

		printf("[TEST06]\n");
		printf("Test hledání prvků, které nejsou ve stromu\n");
		strClear(&name);
		strAddChars(&name,"kebab");
		testFunctionTreeSearch(funTree, name);

		printf("[TEST07]\n");
		printf("Mazání naplněného stromu\n");
		funDisposeTree(&funTree);
		printFunTree(funTree);

		printf("[TEST08]\n");
		printf("přidání volání funkce\n");
		addFunCall(&funTree, name, varTree, 0, 0);

		strClear(&name);
		strAddChars(&name,"TeePee");
		addFunToTree(&funTree, name);
		addFunCall(&funTree, name, varTree, 0, 0);

		printFunTree(funTree);

		printf("[TEST09]\n");
		printf("Kontrola volání funkce\n");
		isFunCallDec(funTree);

		printf("[TEST10]\n");
		printf("Deklarace funkce\n");

		strClear(&name);
		strAddChars(&name,"pizza");
		addFunToTree(&funTree, name);
		addFunDec(&funTree, name, 0, 0);

		strClear(&name);
		strAddChars(&name,"micka");
		addFunToTree(&funTree, name);
		addFunDec(&funTree, name, 0, 0);

		strClear(&name);
		strAddChars(&name,"PP");
		addFunToTree(&funTree, name);
		addFunDec(&funTree, name, 0, 0);

		strClear(&name);
		strAddChars(&name,"TeePee");
		addFunToTree(&funTree, name);
		addFunDec(&funTree, name, 0, 0);

		strClear(&name);
		strAddChars(&name,"kebab");
		addFunToTree(&funTree, name);
		addFunDec(&funTree, name, 0, 0);

		strClear(&name);
		strAddChars(&name,"pp");
		addFunToTree(&funTree, name);
		addFunDec(&funTree, name, 0, 0);

		printFunTree(funTree);

		printf("[TEST11]\n");
		printf("Kontrola volání funkce\n");
		if(isFunCallDec(funTree) == 0){
			printf("Funkce jsou volane i deklarovane!\n");
		}

		printf("[TEST12]\n");
		printf("Zmenime hledany prvek micka na prcka\n");
		funNode *tempNode;

		strClear(&name);
		strAddChars(&name,"micka");

		tempNode = funSearch (&funTree,name);

		strClear(&name);
		strAddChars(&name,"prcka");
		strCopyString(&((*tempNode)->name),&name);

		printFunTree(funTree);

		/* Uvolnění bordelu */

		BSTDispose(&varTree);
		funDisposeTree(tempNode);
		funDisposeTree(&funTree);
		strFree(&name);
	}
	if (TEST_TREE_LIST) {
		funNode funTree; funInit(&funTree);
		string name; strInit(&name);
		//funListElement foundElement;
		varNode varTree; BSTInit(&varTree);
		int parameterOrder = 0;
		int returnOrder = 0;

		printf("Binarni vyhledavaci strom pro funkce s listem\n");
		printf("=========================\n");

		printf("[TEST01]\n");
		printf("Vlozime deklaraci funkce s parametrem n.1 typu INT a parametrem n.2 typu STRING\n");
			strAddChars(&name,"main");

		addFunToTree(&funTree, name);

		addParam(&funTree, name, T_INT, ++parameterOrder);
		addParam(&funTree, name, T_STRING, ++parameterOrder);

		addFunDec(&funTree, name, parameterOrder, returnOrder);
		
			printFunTree(funTree);

		printf("[TEST02]\n");
		printf("Vlozime korektni volani funkce (nemel by byt error)\n");
		
		parameterOrder = 0;

		addFunToTree(&funTree, name);

		addParam(&funTree, name, T_INT, ++parameterOrder);
		addParam(&funTree, name, T_STRING, ++parameterOrder);

		addFunCall(&funTree, name, varTree, parameterOrder, returnOrder);

		printf("[TEST03]\n");
		printf("Vlozime volani funkce s spatnym parametrem n.1 typu STRING a spatnym poctem parametru\n");

		addParam(&funTree, name, T_STRING, ++parameterOrder);

		addFunCall(&funTree, name, varTree, parameterOrder, returnOrder);

		printf("[TEST04]\n");
		printf("Vlozime vice funkci a testujeme znovu nad vetsim stromem\n");

		strClear(&name);
		strAddChars(&name,"pain");
		addFunToTree(&funTree, name);

		strClear(&name);
		strAddChars(&name,"gain");
		addFunToTree(&funTree, name);

		strClear(&name);
		strAddChars(&name,"vein");
		addFunToTree(&funTree, name);

		strClear(&name);
		strAddChars(&name,"in_vain");
		addFunToTree(&funTree, name);

		strClear(&name);
		strAddChars(&name,"slain");
		addFunToTree(&funTree, name);

		/* Volani */

		parameterOrder = 0;

		strClear(&name);
		strAddChars(&name,"rain");
		addFunToTree(&funTree, name);

		addParam(&funTree, name, T_INT, ++parameterOrder);
		addParam(&funTree, name, T_STRING, ++parameterOrder);

		addFunDec(&funTree, name, parameterOrder, returnOrder);

		/****************/

		strClear(&name);
		strAddChars(&name,"kekein");
		addFunToTree(&funTree, name);

		strClear(&name);
		strAddChars(&name,"depejn");
		addFunToTree(&funTree, name);

			printFunTree(funTree);

		printf("[TEST05]\n");
		printf("Volani funkce rain s korektnimi parametry (nemel by byt error)\n");

		strClear(&name);
		strAddChars(&name,"rain");
		parameterOrder = 0;

		addFunToTree(&funTree, name);

		addParam(&funTree, name, T_INT, ++parameterOrder);
		addParam(&funTree, name, T_STRING, ++parameterOrder);

		addFunCall(&funTree, name, varTree, parameterOrder, returnOrder);

		printf("[TEST05]\n");
		printf("Volani funkce rain se spatnym parametrem a parametrem navic\n");

		parameterOrder = 0;

		addFunToTree(&funTree, name);

		addParam(&funTree, name, T_STRING, ++parameterOrder);
		addParam(&funTree, name, T_STRING, ++parameterOrder);
		addParam(&funTree, name, T_STRING, ++parameterOrder);

		addFunCall(&funTree, name, varTree, parameterOrder, returnOrder);

		/*Test end - be free my little string!*/
		strFree(&name);
		BSTDispose(&varTree);
		funDisposeTree(&funTree);
	}
	printf("------------------------------ konec -------------------------------------\n");
	return 0;
}
