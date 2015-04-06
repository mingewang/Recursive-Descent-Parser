#include "SymbolTable.h"

/*SymbolTable::SymbolTable()
{

}

SymbolTable::~SymbolTable()
{

} */

/**************************************************************
*   Entry:  Called from a SymbolTable object
*
*    Exit:  Returns nothing.
*
* Purpose: Adds an item into the list.
*
***************************************************************/
void SymbolTable::Insert(Token newtok)
{
	//if(!Contains(newtok.name))			//Can you insert the same name on a token more than once?
		symboltable.push_back(newtok);
}
	
/**************************************************************
*   Entry:  Called from a SymbolTable object
*
*    Exit:  Returns nothing.
*
* Purpose: Deletes an item from the list.
*
***************************************************************/
void SymbolTable::Delete(string name)
{
	for(list<Token>::iterator it = symboltable.begin(); it != symboltable.end();)
	{
		if(it->name == name)
			it = symboltable.erase(it);
		else
			++it;		
	}
}

/**************************************************************
*   Entry:  Called from a SymbolTable object
*
*    Exit:  Returns bool - Does the list contain the token or not.
*
* Purpose: Searches through the list to find a token if it exists.
*
***************************************************************/
bool SymbolTable::Contains(string name)
{
	bool returnval =  false;

	char* simplename = new char[name.length()]; //all but last character of name passed in
	for(int i = 0; i < name.length(); ++i)
		simplename[i] = '\0';

	for(list<Token>::iterator it = symboltable.begin(); it != symboltable.end();++it)
	{
		if(it->name == name)
			returnval = true;
		else
		{
			strncpy(simplename, name.c_str(), name.length() - 1);
			if(simplename == it->name && (name[name.length() - 1] == ';' || name[name.length() - 1] == '{' || name[name.length() - 1] == '}' || name[name.length() - 1] == '(' || name[name.length() - 1] == ')' ))
			returnval = true;
		}
		
	}
	delete simplename;
	return returnval;
}

/**************************************************************
*   Entry:  Called from a SymbolTable object
*
*    Exit:  Returns nothing.
*
* Purpose: Prints the name of all tokens in list.
*
***************************************************************/
void SymbolTable::PrintAll()
{
	for(list<Token>::iterator it = symboltable.begin(); it != symboltable.end(); ++it)
	{
		cout << it->name << "			" << it->tuse << endl;
	}
}

/**************************************************************
*   Entry:  Called from a SymbolTable object
*
*    Exit:  Returns a Token object that matches the name passed in.
*
* Purpose: Returns a Token that user is searching for.
*
***************************************************************/
Token* SymbolTable::GetToken(string name)
{
		Token* returnval = new Token();

	char* simplename = new char[name.length()]; //all but last character of name passed in
	for(int i = 0; i < name.length(); ++i)
		simplename[i] = '\0';

	for(list<Token>::iterator it = symboltable.begin(); it != symboltable.end();++it)
	{
		if(it->name == name)
			returnval = new Token(*it);
		else
		{
			strncpy(simplename, name.c_str(), name.length() - 1);
			if(simplename == it->name && (name[name.length() - 1] == ';' || name[name.length() - 1] == '{' || name[name.length() - 1] == '}' || name[name.length() - 1] == '(' || name[name.length() - 1] == ')' ))
			returnval = new Token(*it);
		}
		
	}
	delete simplename;
	return returnval;
}
