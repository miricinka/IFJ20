# Symtable for functions

Symtable for functions is implemented as a binary search tree

### Structures of function symtable
Binary tree node  contains:
* string Function ID
* bool isDeclared
* bool isCalled
* list funParams
* list funReturns

List structure contains:
* listElement First
* int elementCount

List element contains:
* int type
* int order

### Using symtable in parser
When a function is declared/called it will be added to symtable without parameters or return codes, which will be added separetely. 
The parser will work with symtable using following functions:

1. Planting the binary tree
* funBSTInit(funNode  symtablePointer);
	* inicializes the pointer to function symtable and its lists
* addFunCall(funNode  symtablePointer, string functionID);
	* adds function to the symtable and sets bool (funcNode->isCalled) to true
* addFunDec(funNode  symtablePointer, string functionID);
	* adds function to the symtable and sets bool (funcNode->isDeclared) to true

* funListDelete(funNode  symtablePointer);
	* frees the whole structure for a happy valgrind
2. Caring for the tree
* isFunCallDec(funNode  symtablePointer);
	* looks into every function and compares its isDeclared and isCalled bool value - function can be declared and not called, but can't be called and not declared
* addParam(string functionID, int parameterType, int parameterOrder)
	* adds parameter type to functions parameter type list, if there is already a parameter on the same position, then compares types
* addReturn(string functionID, int parameterType, int parameterOrder)
	* adds return type to functions return type list, if there is already a parameter on the same position, then compares types
