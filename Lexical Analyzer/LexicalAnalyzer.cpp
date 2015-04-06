#include "LexicalAnalyzer.h"
#include <iostream>
#include <fstream>
#include <regex>
using std::regex;
using std::regex_match;
using std::ifstream;
using std::ofstream;

/**************************************************************
*   Entry:  None
*
*    Exit:  Returns nothing.
*
* Purpose: Fills the keyword lookup table and creates the symbol table.
*
***************************************************************/
LexicalAnalyzer::LexicalAnalyzer()
{
	for(int i = 0; i < 100; i++)
		m_buffer[i] = '\0';
	
	m_keywordtable.emplace_back("void");
	m_keywordtable.emplace_back("int");
	m_keywordtable.emplace_back("float");
	m_keywordtable.emplace_back("if");
	m_keywordtable.emplace_back("else");
	m_keywordtable.emplace_back("while");
	m_keywordtable.emplace_back("return");
	m_keywordtable.emplace_back("cout");
	m_keywordtable.emplace_back("cin");
	m_keywordtable.emplace_back("const");


	m_symboltable = new SymbolTable;
	
}

/**************************************************************
*   Entry:  None
*
*    Exit:  Returns nothing.
*
* Purpose: deletes the sybmol table.
*
***************************************************************/
LexicalAnalyzer::~LexicalAnalyzer()
{
	delete m_symboltable;
}

/**************************************************************
*   Entry:  Called from a LA object
*
*    Exit:  Returns nothing.
*
* Purpose: Reads the file that is created by the preprocessor and calls analyzeline on each line.
*
***************************************************************/
void LexicalAnalyzer::ReadFile()
{
	m_symboltable->Clear();
	ifstream readpointer;
	readpointer.open("ProccessedFile.txt");

	while(!readpointer.eof())
	{
		readpointer.getline(m_buffer, 100);
		if(m_buffer[0] != '\0')
			AnalyzeLine();
		for(int i = 0; i < 100; i++)
			m_buffer[i] = '\0';
		
	}

}

/**************************************************************
*   Entry:  Called from ReadFile()
*
*    Exit:  Returns nothing.
*
* Purpose: Analyzes the line in the buffer into different tokens.
*
***************************************************************/
void LexicalAnalyzer::AnalyzeLine()
{
	string ident;
	string number;
	bool valid = true;
	bool nexttoken = false; 
	bool onedecimal = false;
	bool mainbreak = false;
	char * tokptr;
	tokptr = strtok(m_buffer, " ");
	
	while(mainbreak == false)
	{
		if(!isalnum(*(tokptr+0))) //not a number or a digit (must be symbol or operator)
		{
			if(*(tokptr+0) == '{' || *(tokptr+0) == '}' || *(tokptr+0) == '(' || *(tokptr+0) == ')' || *(tokptr+0) == ';' || *(tokptr+0) == ',') //symbol
			{
				string symbol(1,*(tokptr+0));
				Token newsymbol = {symbol, "0", "symbol", "0"};
				m_symboltable->Insert(newsymbol);
				tokptr++;
			}
			else if(*(tokptr+0) == '+' || *(tokptr+0) == '-' || *(tokptr+0) == '*' || *(tokptr+0) == '<' || *(tokptr+0) == '>' || *(tokptr+0) == '/' || *(tokptr+0) == '=' )
			{

				if(*(tokptr+1) ==  '<' || *(tokptr+1) == '>' || *(tokptr+1) == '=')//token lookahead, either << or >>
				{
					string op(2, *(tokptr+0)); 
					Token operatortoken = {op, "0", "operator", "0"};
					m_symboltable->Insert(operatortoken);
					tokptr += 2;

				}
				else
				{
					string op(1,*(tokptr+0));
					Token operatortoken = {op, "0", "operator", "0"};
					m_symboltable->Insert(operatortoken);
					tokptr++;
				}
			}
			else if(*(tokptr+0) == '"')
			{
				string tempx;
				tempx += (*tokptr+0);
				tokptr++;
				while(*tokptr != '"')
				{
					tempx += (*tokptr+0);
					tokptr++;
				}
				tempx += (*tokptr+0);
				tokptr++;
				Token newsymbolx = {tempx, "literal", "literal", tempx};
				m_symboltable->Insert(newsymbolx);
				tempx.clear();
			}
			else
				tokptr++;
		}
		else if(isalpha(*(tokptr+0)) || *(tokptr+0) == '_')//identifier/keyword section
		{
			while(valid == true)
			{
				if(isalnum(*(tokptr+0)) ||  *(tokptr+0) == '_')
				{
					ident += *(tokptr+0);
					tokptr++;
				}
				if(!isalnum(*(tokptr+0)) &&  *(tokptr+0) != '_' )
					valid = false;
				if(*(tokptr+0) == '\0' || *(tokptr+0) == '{' || *(tokptr+0) == '(' || *(tokptr+0) == ';' ||*(tokptr+0) == ' ' ||*(tokptr+0) == ',' ||*(tokptr+0) == ')')
					nexttoken = true;
				if((*(tokptr+0) == '\0'))
					tokptr++;
			}
			valid = true; //reset to true
			if(nexttoken == true)
			{
				//nexttoken = false; //reset to false

				bool keyword = LookupKeyWord((char*)ident.c_str());
				if(keyword == true)					//keyword
				{
					Token identtoken = {ident, "0", "keyword", "0"};
					m_symboltable->Insert(identtoken);
					//ident.clear();
				}
				else								//identifier
				
				{
					Token identtoken = {ident, "0", "identifier", "0"};
					m_symboltable->Insert(identtoken);
					//ident.clear();
				}
			}
			//reset my variables to default states.
			nexttoken = false;
			valid = true;
			ident.clear();
		}
		else if(isdigit(*(tokptr+0)) || *(tokptr+0) == '.')//digit (int for floating point)
		{
			while(valid == true && nexttoken == false)
			{
				if(*(tokptr+0) == '.')
				{
					if(onedecimal == true) //invalidates multiple decimals.
						valid = false;
					onedecimal = true;	
				}
				if(isdigit(*(tokptr+0)) )//&& onedecimal == false)//int
				{
					number += *(tokptr+0);
					tokptr++;
					//if(*(tokptr+0) == '.')
					//	onedecimal = true;
					if(*(tokptr+0) == ')' || *(tokptr+0) == ';'  || *(tokptr+0) == '\0' || *(tokptr+0) == ' ')
					{
						nexttoken = true;
					}
				}
				else if(*(tokptr+0) == '.')
				{
					number += *(tokptr+0);
					tokptr++;
					
					//if(isdigit(*(tokptr+0)))
					//{
					//	number += *(tokptr+0);
					//	tokptr++;
					//}
					//if(*(tokptr+0) == ')' || *(tokptr+0) == ';'  || *(tokptr+0) == '\0' || *(tokptr+0) == ' ')
					//{
					//	nexttoken = true;
					//}
				}
			}
				if(onedecimal == true && valid == true)
				{
					Token floattok = {number, "float", "number", "0"};
					m_symboltable->Insert(floattok);
					//number.clear();
				}
				else if(valid == true)
				{
					Token inttok = {number, "integer", "number", "0"};
					m_symboltable->Insert(inttok);
					//number.clear();
				}
				else
					tokptr++;
				number.clear();
				nexttoken = false;
				valid = true;

			
		}
		if(*(tokptr+0) == ' ' || *(tokptr+0) == '\t' )
			tokptr++;
		if(*(tokptr+0) == '\0')
			mainbreak = true;
	}
}

/**************************************************************
*   Entry:  Called from AnalyzeLine
*
*    Exit:  Returns bool if word is a keyword.
*
* Purpose: Searches the keyword table to see if it matches the string passed in.
*
***************************************************************/
bool LexicalAnalyzer::LookupKeyWord(char* str)
{
	bool kwexists = false;
	
	for(int i = 0; i < 10 && kwexists == false; ++i)
	{
		if(m_keywordtable[i] == str)
			kwexists = true;
	}
	return kwexists;
}

/**************************************************************
*   Entry:  Called from a LA object
*
*    Exit:  Returns nothing.
*
* Purpose: Writes a new file without comments and preprocessor directives.
*
***************************************************************/
void LexicalAnalyzer::PreProccess()
{
	ifstream data_file;
	char buffer[100];
	char* tokptr;
	bool commentline = false;
	bool notdefined = true;
	Token* mytoken;
	data_file.open("Test1.txt"); //HERE IS WHERE YOU CHOOSE EITHER "Test1.txt" or Test2.txt".
	ofstream writepointer("ProccessedFile.txt");

	 while(!data_file.eof()) 
	 {
	 	data_file.getline(buffer, 100);
		notdefined = true; //reset to true after every line.
		writepointer << endl;
		tokptr = strtok(buffer, " ");
		commentline = false;
		while( tokptr != nullptr && commentline == false)
		{	
			if(*tokptr == '/' && *(tokptr+1) == '/')
			{
					commentline = true;
					writepointer << endl;
			}
			else if(strcmp(tokptr, "#ifndef") == 0)
			{
				tokptr = strtok(NULL, " "); //check
				bool def = m_symboltable->Contains(tokptr);
				if(def != true)
				{
					notdefined = true;
					tokptr = strtok(NULL, " "); //check

				}
				else
				{
					notdefined = false;
					while(strcmp(tokptr,"#endif") && strcmp(tokptr,"#else"))
					{
						tokptr = strtok(NULL, " ");
						if(tokptr == nullptr)
						{
	 						data_file.getline(buffer, 100);
							tokptr = strtok(buffer, " ");
						}

					}
					tokptr = strtok(NULL, " "); //check
				}
			}
			else if(strcmp(tokptr,"#define") == 0 && notdefined == true)
			{
				string definetokenvalue;
				tokptr = strtok(NULL, " "); //check
				if(!m_symboltable->Contains(tokptr))
				{
					string definetokenname = tokptr;
					tokptr = strtok(NULL, " ");
					if(tokptr != nullptr)
						definetokenvalue = tokptr;
					else
						definetokenvalue = "0";
					Token definetoken = {definetokenname, "constant", "preproccesor directive", definetokenvalue};
					m_symboltable->Insert(definetoken);
					tokptr = strtok(NULL, " ");
				}
				else
				{
					tokptr = strtok(NULL, " "); //skip the name
					if(tokptr != nullptr)
						tokptr = strtok(NULL, " "); //skip the value if it exists.
				}


			}
			else
			{
				if(*tokptr == '\t')
					tokptr++;
				else if(m_symboltable->Contains(tokptr) == true)
				{
					mytoken = m_symboltable->GetToken(tokptr);
					writepointer << mytoken->tvalue << " " << *(tokptr+strlen(tokptr+1)) << " ";
					delete mytoken;
					tokptr = strtok(NULL, " ");//gets the next token	

				}
				else
				{
					if(strcmp(tokptr, "#endif") == 0) //dont write the endif
					tokptr = strtok(NULL, " ");//gets the next token
					else
					{
						writepointer << tokptr << " ";
						tokptr = strtok(NULL, " ");//gets the next token	
					}

				}

			}
			//tokptr = strtok(NULL, " ");//gets the next token	
		}
	 }
}

/**************************************************************
*   Entry:  Called from a LA object
*
*    Exit:  Returns nothing.
*
* Purpose: Prints the name of all tokens in list.
*
***************************************************************/
void LexicalAnalyzer::PrintTokens()
{
	m_symboltable->PrintAll();
}


