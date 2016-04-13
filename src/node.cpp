#include <iostream>
#include "node.hpp"

void Node::print(std::ofstream &of) {
	if(of.is_open()) {
		of << "Some error has ocurred\n";
		of.close();
	}
	exit(1);
}

void NodeNT::print(std::ofstream &of) {
	if(of.is_open()) {
		of << "<" << symbol << ">\n";
		for(auto leaf: tree) {
			leaf->print(of);
		}
		of << "</" << symbol << ">\n";
	} else {
		std::cout << "Stream isn't open\n";
		exit(1);
	}
}

void NodeT::print(std::ofstream &of) {
	if(of.is_open()) {
		if (symbol == "<") 
			of << "&lt;\n";
		else if (symbol == ">")
			of << "&gt;\n";
		else
			of << symbol << "\n";
	} else {
		std::cout << "Stream isn't open\n";
		exit(1);
	}
}