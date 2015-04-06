/***********************************************************
* Author:				Tyler Denney
* Date Created:			4/18/14
* Lab Number:			CST 320 Lab 1b
* Filename:				Driver.cpp
*
* Overview:
*	This program will simply test the functionality of my LexicalAnalyzer class. It will preprocess a file called Test1.txt
*	and write a file clled ProccessedFile.txt. It will then analyze the tokens in ProccessedFile.txt and print all tokens.
*
*
*/

#include "LexicalAnalyzer.h"
#include "Parser.h";



int main()
{
	bool parsed = false;
	LexicalAnalyzer LA;
	LA.PreProccess();
	LA.ReadFile();
//	LA.PrintTokens();
	Parser PA(LA.GetSymbolTable().GetList());
	parsed = PA.Program();
	if(parsed == true)
		cout << "~~~~~~~Parsed SUCCESSFUL~~~~~~~~~" << endl;
	else
		cout << "~~~~~~~~~File DID NOT parse~~~~~~~~~" << endl;
	cout << "NUMBER OF SYMBOLS: " << PA.GetSymbolCount() << endl;
	cout << "NUMBER OF IFS: " << PA.GetIfStatements() << endl;
	cout << "NUMBER OF WHILES: " << PA.GetWhileStatements() << endl;
	cout << "NUMBER OF INPUTS: " << PA.GetInputStatements() << endl;
	cout << "NUMBER OF OUTPUTS: " << PA.GetOutputStatements() << endl;
	cout << "NUMBER OF FUNCTIONS: " << PA.GetFunctions() << endl;
	cout << "AVERAGE LENGTH OF IDENTIFIERS: " << PA.GetAvgIdLength() << endl;

	//cout << "MAXIMUM NUMBER OF SCOPES: " << PA.();
	//cout << "AVERAGE NUMBER OF STATEMENTS FOR COMPOUND STATEMENTS: " << PA.();

	

	return 0; 
}
