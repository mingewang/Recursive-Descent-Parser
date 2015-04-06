/************************************************************************
* Class: LexicalAnalyzer
*
* Constructors:	
*	LexicalAnalyzer() - Default constructor
*		
* Methods:		
*	void PreProccess()
*		Removes comments and processed preprocessor directives.
*	void ReadFile()
*		Reads file line by line.
*	void AnalyzeLine()
*		Analyzes the tokens in the file and stores them in the symbol table.
*	void PrintTokens()
*		Prints all tokens with their name and use.
*	bool LookupKeyword(char*)
*		Searches the keyword table and returns if found or not.
*************************************************************************/

#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H
#include <vector>
using std::vector;
#include "SymbolTable.h"

class LexicalAnalyzer
{
public:
	LexicalAnalyzer();
	~LexicalAnalyzer();
	void PreProccess();
	void ReadFile();
	void PrintTokens();
	SymbolTable GetSymbolTable() {return *m_symboltable;}
private:
	void AnalyzeLine();
	bool LookupKeyWord(char* str);
	SymbolTable *m_symboltable;
	vector<string>m_keywordtable;
	char m_buffer[100];
};

#endif //	!LEXICALANALYZER