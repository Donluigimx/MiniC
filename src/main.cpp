#include <iostream>
#include <fstream>
#include "lexic.hpp"
#include "syntactic.hpp"

int main(int argc, char const *argv[])
{
	const char* value = NULL;
	
	if(argc == 2) {
		value = argv[1];
	}

	Lexic lexic((char*)value);
	Syntactic syntactic(&lexic);
	std::ofstream of("output.xml", std::ios_base::out);
	syntactic.tree->print(of);
	return 0;
}