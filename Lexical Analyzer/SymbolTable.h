/************************************************************************
* Class: SymbolTable
*
* Constructors:	
*	SymbolTable() - Default constructor
*		
* Methods:		
*	void Insert(Token newtok)
*		Inserts an item into the list of tokens.
*	void Delete(string name)
*		Deletes the token with a name matching the parameter passed in. 
*	void PrintAll()
*		Prints all token names in the list.
*	bool Contains(string name)
*		Returns whether or not a token with a name matches the name passed in 
*		exists in the list.
*************************************************************************/
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include <string>
using std::string;
#include <list>
using std::list;
#include <iostream>
using std::cout;
using std::endl;

//This token structure is what is stored in the list. It is easily extensible in both the ability to add 
//items in the structure and to change datatypes. 
struct Token
{
	string name; //Name of variable / symbol. int x would be x, } would just be }
	string ttype; //int, float, void
	string tuse; //identifier, keyword, symbol
	string tvalue; //value of constants
	string vartype; //function or data
	bool declared;
	int scope;
};


class SymbolTable
{

public:
	SymbolTable() {;}
	~SymbolTable() {;}
	void Insert(Token newtok);
	void Delete(string name);
	void PrintAll();
	void Clear() { symboltable.clear();}
	list<Token> GetList() {return symboltable;}
	bool Contains(string name);
	Token* GetToken(string name);
	
private:
	list<Token> symboltable;
};
#endif // !SYMBOLTABLE
