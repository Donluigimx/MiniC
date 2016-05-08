#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>
#include "lexic.hpp"

#define INDEF "input.txt"
#define OUTDEF "output.txt"

Lexic::Lexic(char* inFile) {
	Initialyze();
	Analyze(inFile);
	this->pos = -1;
	Next();
}

void Lexic::Analyze(char* fname) {
	std::map<std::string,int> stbl;
	int lenght;
	char *buffer;
	std::ifstream inFile;
	//std::ofstream outFile;
	int actualState;
	int transitionValue;
	int inputVal;
	int symval;
	std::string identi;

	if (fname != NULL) {
		inFile.open(fname);
	} else {
		inFile.open("input.txt");
	}

	if (inFile.good()) {
		inFile.seekg(0,std::ios::end);
		lenght = inFile.tellg();
		inFile.seekg(0,std::ios::beg);
		buffer = new char[lenght];
		inFile.read(buffer,lenght);
		inFile.close();
	} else {
		std::cout << "Error trying to read the file" << std::endl;
		exit(0);
	}

	actualState = 0;
	identi = "";
	for (int i = 0; i < lenght; i++) {
		inputVal = getValue(buffer[i]);
		transitionValue = stateOut[actualState][inputVal];
		actualState = stateMachine[actualState][inputVal];
		if (transitionValue >= 0) {
			symval = transitionValue;
			if (transitionValue == Token::IDENTIFIER) {
				symval = symbolTable(stbl,identi.c_str());
				if(symval > 0)
					transitionValue = symval;
				else
					symval = stbl.find(identi.c_str())->second;
			} else if ( transitionValue == Token::NUMBER)
				symval = atoi(identi.c_str());
			else if (transitionValue == Token::ERROR)
				Error();
			if(transitionValue != Token::COMMENT && transitionValue != Token::COMMENT_O)
				token.push_back(std::pair<int,int>(transitionValue,symval));
			//outFile << transitionValue << ' ';
			identi = "";
		}
		if (buffer[i] != ' ' && buffer[i] != '\n' && buffer[i] != '\t' && buffer[i] != '/')
			identi += buffer[i];
	}
	transitionValue = stateOut[actualState][i18];
	if (transitionValue >= 0) {
		symval = transitionValue;
		if (transitionValue == 1) {
			symval = symbolTable(stbl,identi.c_str());
			if(symval > 0)
				transitionValue = symval;
			else
				symval = stbl.find(identi.c_str())->second;
		}
		token.push_back(std::pair<int,int>(transitionValue,symval));
		//outFile << transitionValue << ' ';
		identi = "";
	}

	token.push_back(std::pair<int, int>(Token::END_OF_FILE,Token::END_OF_FILE));
	//cout << "Tabla de simbolos:" << endl << endl;
	//for (map< string, int >::iterator i = stbl.begin(); i != stbl.end(); ++i)
	//{
	//	cout << i->first << "=>" << i->second << endl;
	//}

	//cout << endl << "Tokens" << endl << endl;
	//for (vector< pair<int, int > >::iterator i = token.begin(); i != token.end(); ++i)
	//{
	//	pair<int, int> it = *i;
	//	cout << names[it.first] << "=>" << it.second << endl;
	//}
	//outFile.close();
}

int Lexic::getValue(char c) {
	if ( c == ' ')
		return i0;
	else if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' )
		return i1;
	else if ( c >= '0' && c <= '9' )
		return i2;

	switch (c){
		case '+':
			return i3;
		case '-':
			return i4;
		case '/':
			return i5;
		case '*':
			return i6;
		case '%':
			return i7;
		case '<':
			return i8;
		case '>':
			return i9;
		case '=':
			return i10;
		case '!':
			return i11;
		case '(':
			return i12;
		case ')':
			return i13;
		case '{':
			return i14;
		case '}':
			return i15;
		case ';':
			return i16;
		case ',':
			return i17;
		case '\n':
			return i18;
		case '\t':
			return i19;
		default:
			return IN;
	}
}

int Lexic::symbolTable(std::map<std::string,int>& stbl, const char* value) {
	std::map< std::string, int >::iterator it;
	it = reserved.find(value);
	if (it != reserved.end())
		return it->second;
	it = stbl.find(value);
	if (it == stbl.end()) {
		symtbl.insert(std::pair<int,std::string>(stbl.size(),value));
		stbl.insert(std::pair<std::string,int>(value,stbl.size()));
	}
	return -1;
}

void Lexic::Initialyze() {
	reserved.insert( std::pair<std::string,int>("if",26));
	reserved.insert( std::pair<std::string,int>("int",27));
	reserved.insert( std::pair<std::string,int>("void",28));
	reserved.insert( std::pair<std::string,int>("break",29));
	reserved.insert( std::pair<std::string,int>("continue",30));
	reserved.insert( std::pair<std::string,int>("else",31));
	reserved.insert( std::pair<std::string,int>("for",32));
	reserved.insert( std::pair<std::string,int>("while",33));
	reserved.insert( std::pair<std::string,int>("do",34));
	reserved.insert( std::pair<std::string,int>("return",35));

	tokenSymbol.push_back("");
	tokenSymbol.push_back("Identificador");
	tokenSymbol.push_back("Numero");
	tokenSymbol.push_back("*");
	tokenSymbol.push_back("+");
	tokenSymbol.push_back("-");
	tokenSymbol.push_back("/");
	tokenSymbol.push_back("%");
	tokenSymbol.push_back(">");
	tokenSymbol.push_back(">=");
	tokenSymbol.push_back("<");
	tokenSymbol.push_back("<=");
	tokenSymbol.push_back("=");
	tokenSymbol.push_back("==");
	tokenSymbol.push_back("");
	tokenSymbol.push_back("!=");
	tokenSymbol.push_back("(");
	tokenSymbol.push_back(")");
	tokenSymbol.push_back("{");
	tokenSymbol.push_back("}");
	tokenSymbol.push_back(";");
	tokenSymbol.push_back("//");
	tokenSymbol.push_back("");
	tokenSymbol.push_back("/**/");
	tokenSymbol.push_back(",");
	tokenSymbol.push_back("Error");
	tokenSymbol.push_back("if");
	tokenSymbol.push_back("int");
	tokenSymbol.push_back("void");
	tokenSymbol.push_back("break");
	tokenSymbol.push_back("continue");
	tokenSymbol.push_back("else");
	tokenSymbol.push_back("for");
	tokenSymbol.push_back("while");
	tokenSymbol.push_back("do");
	tokenSymbol.push_back("return");
	tokenSymbol.push_back("$");
}

void Lexic::Next() {
	this->pos++;

	auto aux = this->token[this->pos];

	switch (aux.first) {
		case Token::IDENTIFIER:
			this->Symbol = this->symtbl[aux.second];
			break;

		case Token::NUMBER:
			this->Symbol = std::to_string(aux.second);
			break;

		default:
			this->Symbol = this->tokenSymbol[aux.second];
	}

	this->Token = aux.first;
	//std::cout << Symbol << " " << Token << std::endl;
}

void Lexic::Error() {
	std::cout << "ERROR IN LEXIC PHASE" << std::endl;
	exit(0);
}
