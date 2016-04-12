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
	return 0;
}