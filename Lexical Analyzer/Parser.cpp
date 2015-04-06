#include "Parser.h"

Parser::Parser(list<Token> symbols)
{
	
	m_ifs = 0;
	m_whiles = 0;
	m_outputs = 0;
	m_inputs = 0;
	m_functions = 0;
	m_symbols = symbols;
	m_current = m_symbols.begin();
	
}
Parser::~Parser()
{

}


bool Parser::Program()
{
	return Definition();
}

void Parser::SetDeclared()
{
	string name = m_current->name;
	string type;
	list<Token>::iterator temp = m_current;
	++temp;
	if(temp->name == "(")
	{
	//	m_current->vartype = "Function";
		--temp;
		--temp;
		type = temp->name;
	//	m_current->ttype = type;
		m_rules.push_back("Function declaration");
		m_functions++;
		list<Token>::iterator it = m_symbols.begin();
		for(;it != m_symbols.end(); ++it)
		{
			if(it->name == name)
			{
				it->declared = true;
				it->ttype = type;
				it->vartype = "Function";
			}
		}
	}
	else
	{
		m_rules.push_back("Data declaration");
		list<Token>::iterator it = m_symbols.begin();
		for(;it != m_symbols.end(); ++it)
		{
			if(it->name == name)
			{
				it->declared = true;
				it->vartype = "Data";
			}

		}
	}
}

bool Parser::Definition()
{
	if(m_current == m_symbols.end())
	{
		cout << "End of Tokens" << endl;
		return true;
	}
	if(Data_Definition())
	{
		if(Definition())
		{
			cout << "Definition -> Definition" << endl;
			return true;
		}
		else
		{
			cout << "Definition -> Declaration failed" << endl;
			return false;
		}
	}
	else
	{
		cout << "Definition -> Data_Definition failed" << endl;
		return false;
	}
}
bool Parser::Data_Definition()
{
	list<Token>::iterator temp, i;
	temp = m_current;
	if(m_current == m_symbols.end())
	{
		cout << "Unexpected end of file in Data_Definition" << endl;
		return false;
	}
	if(Type())
	{
		if(m_current->tuse == "identifier")
		{
			SetDeclared();
			++m_current;
			if(m_current == m_symbols.end())
			{				
				cout << "Unexpected end of file in Data_Definition" << endl;
				return false;
			}
			if(Another_Def())
			{
				//m_rules.push_back("Data Declaration");
				cout << "Data_Definition -> Another_Def" << endl;
				return true;
			}
			else
			{
				cout << "Data_definition -> Another_Def failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Data_Definition failed on Identifier" << endl;
			return false;
		}
	}
	else
	{
		cout << "Data_definition -> Type failed." << endl;
		return false;
	}
}
bool Parser::Type()
{
	list<Token>::iterator temp, i;
	temp = m_current;
	if(m_current->name == "int" || m_current->name == "float" || m_current->name == "void")
	{
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Type" << endl;
			return false;
		}
		cout << "Type -> " + temp->name << endl;
		return true;
	}
	else
	{
		cout << "Failed Type, expecting int, flaot, void" << endl;
		return false;
	}

}
bool Parser::Another_Def()
{
	if(m_current->name == ";")
	{
		++m_current;
		cout << "Another_Def -> ;" << endl;
		return true;
	}
	if(m_current->name == ",")
	{
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Another_Def" << endl;
			return false;
		}
		if(m_current->tuse == "identifier")
		{
			SetDeclared();
			++m_current;
			if(m_current == m_symbols.end())
			{
				cout << "Unexpected end of file in Another Def" << endl;
				return false;
			}
			if(Identifier_List())
			{
				if(m_current->name == ";")
				{
					++m_current;
					cout << "Another_Def -> ;" << endl;
					return true;
				}
				else
				{
					cout << "Another_Def -> ; failed" << endl;
					return false;
				}
			}
			else
			{
				cout << "Another_Def -> Identifier_List failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Another_Def -> identifier failed" << endl;
			return false;
		}
	}
	if(m_current->name == "(")
	{
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Another_Def" << endl;
			return false;
		}
		if(Parameters())
		{
			if(m_current->name == ")")
			{
				++m_current;
				if(m_current == m_symbols.end())
				{
					cout << "Unexpected end of file in Another_Def" << endl;
					return false;
				}
				if(m_current->name == "{")
				{
					++m_current;
					if(m_current == m_symbols.end())
					{
						cout << "Unexpected end of file in Another_Def" << endl;
						return false;
					}
					if(Statement())
					{
						if(m_current->name == "}")
						{
							++m_current;
							cout << "Another_Def -> }" << endl;
							return true;
						}
						else
						{
							cout << "Another_Def -> } failed" << endl;
							return false;
						}
					}
					else
					{
						cout << "Another_Def -> Statement failed" << endl;
						return false;
					}

				}
				else
				{
					cout << "Another_Def -> { failed" << endl;
					return false;
				}
			}
			else
			{
				cout << "Another_Def -> ) failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Another_Def -> Parameters failed" << endl;
			return false;
		}
	}
	else
	{
		cout << "Another_Def -> ( failed" << endl;
		return false;
	}
	
}

bool Parser::Identifier_List()
{
	if(m_current->name == ",")
	{
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Identifier_List" << endl;
			return false;
		}
		if(m_current->tuse == "identifier")
		{
			++m_current;
			if(m_current == m_symbols.end())
			{
				cout << "Unexpected end of file in Identifier_List" << endl;
				return false;
			}
			if(Identifier_List())
			{
				cout << "Identifier_List -> Identifier_List" << endl;
				return true;
			}
			else
			{
				cout << "Identifier_List -> Identifier_List failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Identifier_List -> identifier failed" << endl;
			return false;
		}
	}
	else
	{
		if(m_current->name == ";")
		{
			cout << "Identifier_List -> lambda" << endl;
			return true;
		}
		else
		{
			cout << "Identifier_List -> lambda failed" << endl;
			return false;
		}
	}
}

bool Parser::Parameters()
{
	if(Type())
	{
		if(m_current->tuse == "identifier")
		{
			SetDeclared();
			++m_current;
			if(m_current == m_symbols.end())
			{
				cout << "Unexpected end of file in Parameters" << endl;
				return false;
			}
			if(Parameters2())
			{
				cout << "Parameters -> Parameters2" << endl;
				return true;
			}
			else
			{
				cout << "Paramters -> Paramters2 failed" << endl;
			return false;
			}
		}
		else
		{
			cout << "Paramters -> identifier failed" << endl;
			return false;
		}
	}
	else
	{
		if(m_current->name == ")")
		{
			cout << "Parameters -> Lambda" << endl;
			return true;
		}
		else
		{
			cout << "Parameters -> Type failed" << endl;
			return false;
		}
	}
}

bool Parser::Parameters2()
{
	if(m_current->name == ",")
	{
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Parameters2" << endl;
			return false;
		}
		if(Type())
		{
			if(m_current->tuse == "identifier")
			{
				SetDeclared();
				++m_current;
				if(m_current == m_symbols.end())
				{
					cout << "Unexpected end of file in Parameters2" << endl;
					return false;
				}
				if(Parameters2())
				{
					cout << "Parameters2 -> Parameters2" << endl;
					return true;
				}
				else
				{
					cout << "Paramters2 -> Parameters2 failed" << endl;
					return false;
				}
			}
			else
			{
				cout << "Paramters2 -> identifier failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Parameters2 -> Type failed" << endl;
			return false;
		}
	}
	else
	{
		if(m_current->name == ")")
		{
			cout << "Parameters2 -> Lambda" << endl;
			return true;
		}
		else
		{
			cout << "Parameters2 -> , failed" << endl;
			return false;
		}
	}
}


bool Parser::Statement()
{
	
	if(m_current->name == "if")
	{
		m_rules.push_back("If statement");
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Statement" << endl;
			return false;
		}
		if(m_current->name == "(")
		{
			++m_current;
			if(m_current == m_symbols.end())
			{
				cout << "Unexpected end of file in Statement" << endl;
				return false;
			}
			if(m_current->tuse == "identifier")
			{
				if(m_current->declared == false)
				{
					cout << m_current->name + " not yet declared. PARSING FAILED" << endl;
					return false;
				}
				++m_current;
				if(m_current == m_symbols.end())
				{
					cout << "Unexpected end of file in Statement" << endl;
					return false;
				}
				if(Expression2())
				{

					if(m_current->name == ")")
					{
						++m_current;
						if(m_current == m_symbols.end())
						{
							cout << "Unexpected end of file in Statement" << endl;
							return false;
						}
						if(Statement())
						{
							cout << "Statement -> if ( identifier Expression2 ) Statement" << endl;
							//m_rules.push_back("If statement");
							m_ifs++;
							return true;
						}
						else
						{
							cout << "Statement -> if ( identifier Expression2 ) Statement failed" << endl;
							return false;
						}

					}
					else
					{
						cout << "Statement -> if ( Expression ) failed" << endl;
						return false;
					}
				}
				else
				{
					cout << "Statement -> if ( identifier Expression2 failed" << endl;
					return false;
				}

				
			}
			else
			{
				cout << "Statement -> if ( identifier failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Statement -> ( failed" << endl;
			return false;
		}

	}
	else if(m_current->name == "else")
	{
		m_rules.push_back("Else statement");
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Statement" << endl;
			return false;
		}
		if(Statement())
		{
			cout << "Statement -> else Statement" << endl;
			//m_rules.push_back("Else statement");
			return true;
		}
		else
		{
			cout << "Statement -> else Statement failed" << endl;
			return false;
		}

	}
	else if(m_current->name == "while")
	{
		m_rules.push_back("While Loop");
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Statement" << endl;
			return false;
		}
		if(m_current->name == "(")
		{
			++m_current;
			if(m_current == m_symbols.end())
			{
				cout << "Unexpected end of file in Statement" << endl;
				return false;
			}
			if(m_current->tuse == "identifier")
			{
				if(m_current->declared == false)
				{
					cout << m_current->name + " not yet declared. PARSING FAILED" << endl;
					return false;
				}
				++m_current;
				if(m_current == m_symbols.end())
				{
					cout << "Unexpected end of file in Statement" << endl;
					return false;
				}
				if(Expression2())
				{
					if(m_current->name == ")")
					{
						++m_current;
						if(m_current == m_symbols.end())
						{
							cout << "Unexpected end of file in Statement" << endl;
							return false;
						}
						if(Statement())
						{
							cout << "Statement -> while ( identifier Expression2 ) Statement" << endl;
							//m_rules.push_back("While Loop");
							m_whiles++;
							return true;
						}
						else
						{
							cout << "Statement -> while ( identifier Expression2 ) Statement failed" << endl;
							return false;
						}
					}
					else
					{
						cout << "Statement -> while ( identifier Expression2 ) failed" << endl;
						return false;
					}
				}
				else
				{
					cout << "Statement -> while ( identifier Expression2 failed" << endl;
					return false;
				}
			}
			else
			{
				cout << "Statement -> while ( identifier failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Statement -> while ( failed" << endl;
			return false;
		}

	}
	else if(m_current->name == "return")
	{
		m_rules.push_back("Return statement");
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Statement" << endl;
			return false;
		}
		if(Expression())
		{
			if(m_current->name == ";")
			{
				++m_current;
				if(m_current == m_symbols.end())
				{
					cout << "Unexpected end of file in Statement. Expected } after ;" << endl;
					return false;
				}
				cout << "Return -> Expression -> ;" << endl;
				///m_rules.push_back("Return statement");
				return true;
			}
			else
			{
				cout << "Statement -> ; failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Statement -> Expression failed" << endl;
			return false;
		}
	}
	else if(m_current->name == "{")
	{
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Statement" << endl;
			return false;
		}
		if(Statement())
		{
			if(m_current->name == "}")
			{
				++m_current;
				if(m_current == m_symbols.end())
				{
					cout << "Unexpected end of file in Statement" << endl;
					return false;
				}
				if(Statement())
				{
					cout << "Statement -> { Statement } Statement" << endl;
					return true;
				}
				else
				{
					cout << "Statement -> { Statement } Statement failed" << endl;
					return false;
				}

			}
			else
			{
				cout << "Statement -> { Statement } failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Statement -> { Statement failed" << endl;
			return false;
		}
	}
	else if(Expression())
	{
		if(m_current->name == ";")
		{
			++m_current;
			if(m_current == m_symbols.end())
			{
				cout << "Undexpected end of file in Statement" << endl;
				return false;
			}
			if(Statement())
			{
				cout << "Statement -> Statement" << endl;
				return true;
			}
			else
			{
				cout << "Statement -> Statement failed" << endl;
				return false;
			}

		}
		else
		{
			cout << "Statement -> ; failed" << endl;
			return false;
		}
	}
	else if(Data_Definition())
	{
		if(Statement())
		{
			cout << "Statement -> Statement" << endl;
			return true;
		}
		else
		{
			cout << "Statement -> Statement failed" << endl;
			return false;
		}
	}
	else
	{
		if(m_current->name == "}")
		{
			cout << "Statement -> Lambda" << endl;
			return true;
		}
		else
		{
			cout << "Statement -> lambda failed" << endl;
			return false;
		}
	}

}
bool Parser::Expression()
{
	if(m_current->tuse == "identifier")
	{
		if(m_current->declared == false)
				{
					if(m_current->vartype == "Data")
					{
					cout << m_current->name + " not yet declared. PARSING FAILED" << endl;
					return false;
					}
				}
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Expression" << endl;
			return false;
		}
		if(Expression2())
		{
			cout << "Expression -> Expression2" << endl;
			return true;
		}
		else
		{
			cout << "Expresion -> Expression2 failed" << endl;
			return false;
		}
	
	}
	else if(m_current->name == "cout")
	{
		m_rules.push_back("Output statement");
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Expression" << endl;
			return false;
		}
		if(m_current->name == "<<")
		{
			++m_current;
			if(m_current == m_symbols.end())
			{
				cout << "Unexpected end of file in Expression" << endl;
				return false;
			}
			if(Primary())
			{
				if(Expression2())
				{
					cout << "Expression -> Expression2" << endl;
					//m_rules.push_back("Output statement");
					m_outputs++;
					return true;
				}
				else
				{
					cout << "Expression -> Expression2 failed" << endl;
					return false;
				}
			}
			else
			{
				cout << "Expression -> Primary failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Expression -> << failed" << endl;
			return false;
		}
	}
	else if(m_current->name == "cin")
	{
		m_rules.push_back("Input statement");
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Expression" << endl;
			return false;
		}
		if(m_current->name == ">>")
		{
			++m_current;
			if(m_current == m_symbols.end())
			{
				cout << "Unexpected end of file in Expression" << endl;
				return false;
			}
			if(m_current->tuse == "identifier")
			{
				if(m_current->declared == false)
				{
					cout << m_current->name + " not yet declared. PARSING FAILED" << endl;
					return false;
				}
				++m_current;
				if(m_current == m_symbols.end())
				{
					cout << "Unexpected end of file in Expression" << endl;
					return false;
				}
				if(m_current->name == ";")
				{
					cout << "cin >> identifier;" << endl;
					//m_rules.push_back("Input statement");
					m_inputs++;
					return true;
				}
				else
				{
					cout << "Expression -> cin >> identifier ; failed" << endl;
					return false;
				}
			}
			else
			{
				cout << "Expression -> cin >> identifier failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Expression -> >> failed" << endl;
			return false;
		}
	}
	else if(Primary())
	{
		if(Expression2())
		{
			cout << "Expression -> Expression2" << endl;
			return true;
		}
		else
		{
			cout << "Expression -> Expression2 failed" << endl;
			return false;
		}
	}
	else
	{
		cout << "Expression failed. Expecting identifier, cout, cin, or primary" << endl;
		return false;
	}
}

bool Parser::Expression2()
{
	string statementtype; //logical, arithmetic, assignment
	if(m_current->name == ">" || m_current->name == "<" ||  m_current->name == "+" ||  m_current->name == "-" ||  m_current->name == "*" ||  m_current->name == "/" ||  m_current->name == "=" ||  m_current->name == "==")
	{
		statementtype = m_current->name;
			if(statementtype == "<" ||  statementtype == ">" || statementtype == "==")
					m_rules.push_back("Logical expression");
				else if(statementtype == "+" ||  statementtype == "-" ||statementtype == "*" ||  statementtype == "/")
					m_rules.push_back("Arithmetic expression");
				else
					m_rules.push_back("Assignment statement");
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Expression2" << endl;
			return false;
		}
		if(Expression())
		{
			if(Expression2())
			{
				cout << "Expression2 -> Expression2" << endl;
			/*	if(statementtype == "<" ||  statementtype == ">")
					m_rules.push_back("Logical expression");
				else if(statementtype == "+" ||  statementtype == "-" ||statementtype == "*" ||  statementtype == "/")
					m_rules.push_back("Arithmetic expression");
				else
					m_rules.push_back("Assignment statement");*/
					return true;
			}
			else
			{
				cout << "Expression2 -> Expression 2 failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Expression2 -> Expression failed" << endl;
			return false;
		}
	}
	else if(m_current->name == "(")
	{
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Expression2" << endl;
			return false;
		}
		if(Argument_List())
		{
			if(m_current->name == ")")
			{
				++m_current;
				if(m_current == m_symbols.end())
				{
					cout << "Unexpected end of file in Expression2" << endl;
					return false;
				}
					cout << "Expression2 -> ( Argument_List );" << endl;
					return true;
			}	
			else
			{
				cout << "Expression2 -> ( Argument_List ) failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Expression2 -> ( Argument_List failed" << endl;
			return false;
		}
	}
	else if(m_current->name == "<<")
	{
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Expression2" << endl;
			return false;
		}
		if(Expression())
		{

			return true;
		}
		else
			return false;
	}
	else
	{
		if(m_current->name == ";" || m_current->name == ")")
		{
			cout << "Expression2 -> lambda" << endl;
			return true;
		}
		else
		{
			cout << "Expression2 -> lambda failed" << endl;
			return false;
		}
	}
}
bool Parser::Primary()
{
	if(m_current->tuse == "identifier")
	{
		if(m_current->declared == false)
		{
			cout << m_current->name + " not yet declared. PARSING FAILED" << endl;
			return false;
		}
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Primary" << endl;
			return false;
		}
		cout << "Primary -> identifier" << endl;
		return true;
	}
	else if(m_current->ttype == "float" || m_current->ttype == "integer" || m_current->ttype == "literal")
	{
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Primary" << endl;
			return false;
		}
		cout << "Primary -> num" << endl;
		return true;
	}
	else if(m_current->name == "(")
	{
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Primary" << endl;
			return false;
		}
		if(Expression())
		{
			if(m_current->name == ")")
			{
				++m_current;
				cout << "Primary -> )" << endl;
				return true;
			}
			else
			{
				cout << "Primary -> ) failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Primary -> Expression failed" << endl;
			return false;
		}
	}
	

}
bool Parser::Argument_List()
{
	m_rules.push_back("Function Call");
	if(m_current->tuse == "identifier")
	{
		if(m_current->declared == false)
		{
			cout << m_current->name + " not yet declared. PARSING FAILED" << endl;
			return false;
		}
		m_current++;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Primary" << endl;
			return false;
		}
		if(Expression3())
		{
			if(Argument_List_2())
			{
				cout << "Argument_List -> identifier Expression2 Argument_List2" << endl;
				//m_rules.push_back("Function Call");
				return true;
			}
			else
			{
				cout << "Argument_List -> identifier Expression2 Argument_List2 failed" << endl;
				return false;
			}

		}
		else
		{
			cout << "Argument_List -> identifier Expression2 failed" << endl;
			return false;
		}


	}
	else
	{
		if(m_current->name == ")")
		{
			cout << "Arugment_List -> Lambda" << endl;
			return true;
		}
		else
		{
			cout << "Argument_List -> Lambda failed" << endl;
			return false;
		}
	}

}
bool Parser::Argument_List_2()
{
	if(m_current->name == ",")
	{
		++m_current;
		if(m_current == m_symbols.end())
		{
			cout << "Unexpected end of file in Primary" << endl;
			return false;
		}
		if(m_current->tuse == "identifier")
		{
			if(m_current->declared == false)
			{
				cout << m_current->name + " not yet declared. PARSING FAILED" << endl;
				return false;
			}
			++m_current;
			if(m_current == m_symbols.end())
			{
				cout << "Unexpected end of file in Primary" << endl;
				return false;
			}
			if(Expression3())
			{
				if(Argument_List_2())
				{
					cout << "Arugment_List2 -> , identifier Expression2 Argument_List2" << endl;
					return true;
				}
				else
				{
					cout << "Arugment_List2 -> , identifier Expression2 Argument_List2 failed" << endl;
					return false;
				}
			}
			else
			{
				cout << "Arugment_List2 -> , identifier Expresison2 failed" << endl;
				return false;
			}
		}
		else
		{
			cout << "Arugment_List2 -> , identifier failed" << endl;
			return false;
		}
	}
	else
	{
		if(m_current->name == ")")
		{
			cout << "Arugment_List2 -> Lambda" << endl;
			return true;
		}
		else
		{
			cout << "Argument_List2 -> Lambda failed" << endl;
			return false;
		}
	}
	

}

bool Parser::Expression3()
{
	if(Expression2())
	{
		cout << "Expression3 -> Expression2" << endl;
		return true;
	}
	else
	{
		if(m_current->name == ",")
		{
			cout << "Expression3 -> ," << endl;
			return true;
		}
		else
		{
			cout << "Expression3 -> , failed" << endl;
		}
	}
}

int Parser::GetAvgIdLength()
{
	int length = 0;
	int num = 0;
	for(list<Token>::iterator it = m_symbols.begin(); it != m_symbols.end();++it)
	{
		if(it->tuse == "identifier")
		{
			length += it->name.size();
			num++;
		}
	}
	return length / num;
}