/************************************************************************
* Class: Parser
*
* Constructors:	
*	LexicalAnalyzer(list<Token>) - constructor, sets m_current and m_symbols
*		
* Methods:		
*	All of these methods refer to the rules that corrospond to the names in the Grammar attached.
*	All return a boolean value if the rule is successful or not.
*	bool Definition() 
	bool Data_Definition()
	bool Type()
	bool Another_Def()
	bool Identifier_List()
	bool Statement()
	bool Expression()
	bool Primary()
	bool Argument_List()
	bool Argument_List_2()
	bool Parameters()
	bool Parameters2()
	bool Expression2()
*************************************************************************/

#ifndef PARSER_H
#define PARSER_H
#include "SymbolTable.h"
class Parser
{
public:
	Parser(list<Token> symbols);
	~Parser();
	
	//Here are the final project functions
	int GetSymbolCount() { return m_symbols.size();}
	int GetAvgIdLength();
	int GetScopeLevels();
	int GetIfStatements() { return m_ifs;}
	int GetWhileStatements() { return m_whiles;}
	int GetInputStatements() { return m_inputs;}
	int GetOutputStatements() { return m_outputs;}
	int GetFunctions() { return m_functions;}

	bool Program();
private:
	list<Token> m_symbols;
	list<Token>::iterator m_current;
	list<string> m_rules;
	bool Definition();
	bool Data_Definition();
	bool Type();
	bool Another_Def();
	bool Identifier_List();
	bool Statement();
	bool Expression();
	bool Primary();
	bool Argument_List();
	bool Argument_List_2();
	bool Parameters();
	bool Parameters2();
	bool Expression2();
	bool Expression3();
	void SetDeclared();


	int m_ifs;
	int m_whiles;
	int m_outputs;
	int m_inputs;
	int m_functions;
	int m_currentscope;

	


};
#endif