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
	std::ofstream of("output", std::ios_base::out);
	syntactic.tree->analysis();
	if (Node::isOk)
		syntactic.tree->code(of);
	for (auto it: Node::symtable) {
		std::cout << it.first.first << "\t|";
		std::cout << it.first.second << "\t|";
		std::cout << it.second.dataType << "\t|";
		std::cout << it.second.type << "\t|";
		std::cout << it.second.isDef << std::endl;
	}

	of.close();
	return 0;
}
