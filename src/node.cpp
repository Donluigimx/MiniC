#include <iostream>
#include "node.hpp"

void Node::print(std::ofstream &of) {
	if(of.is_open()) {
		std::cout << "FATAL ERROR"  << std::endl;
		exit(0);
	} else {
		exit(1);
	}
}

void Expression::print(std::ofstream &of) {
	if(of.is_open()) {
		std::cout << "FATAL ERROR"  << std::endl;
		exit(0);
	} else {
		exit(1);
	}
}

void Parameter::print(std::ofstream &of) {
	std::string sym;
	if (this->type == Token::INT)
		sym = "Int";
	else
		sym = "error";
	if(of.is_open()) {
		of << "<Parameter value=\"" << sym << "\">\n";
		of << this->symbol << "\n";
		of << "</Parameter>\n";
	}
}

void Mul::print(std::ofstream &of) {
	if(of.is_open()) {
		of << "<Mul value=\"" << this->symbol << "\">\n";
		if(this->l != nullptr)
			this->l->print(of);
		if(this->r != nullptr)
			this->r->print(of);
		of << "</Mul>\n";
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}

void Add::print(std::ofstream &of) {
	if(of.is_open()) {
		of << "<Add value=\"" << this->symbol << "\">\n";
		if(this->l != nullptr)
			this->l->print(of);
		if(this->r != nullptr)
			this->r->print(of);
		of << "</Add>\n";
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}

void Comp::print(std::ofstream &of) {
	std::string aux = this->symbol;
	if(aux[0] == '>') {
		aux.erase(0,1);
		aux = "&gt;"+aux;
	} else if (aux[0] == '<') {
		aux.erase(0,1);
		aux = "&lt;"+aux;
	}
	if(of.is_open()) {
		of << "<Comp value=\"" << aux << "\">\n";
		if(this->l != nullptr)
			this->l->print(of);
		if(this->r != nullptr)
			this->r->print(of);
		of << "</Comp>\n";
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}

void Assign::print(std::ofstream &of) {
	if(of.is_open()) {
		of << "<Assign value=\"" << this->symbol << "\">\n";
		if(this->l != nullptr)
			this->l->print(of);
		if(this->r != nullptr)
			this->r->print(of);
		of << "</Assign>\n";
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}

void Id::print(std::ofstream &of) {
	if(of.is_open()) {
		of << "<Id>\n";
		if(this->l != nullptr)
			this->l->print(of);
		if(this->r != nullptr)
			this->r->print(of);
		of << this->symbol;
		of << "</Id>\n";
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}

void Value::print(std::ofstream &of) {
	if(of.is_open()) {
		of << "<Value>\n";
		if(this->l != nullptr)
			this->l->print(of);
		if(this->r != nullptr)
			this->r->print(of);
		of << this->symbol;
		of << "</Value>\n";
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}

void FuncCall::print(std::ofstream &of) {
	if(of.is_open()) {
		of << "<FuncCall value=\"" << this->symbol << "\">\n";
		if(this->l != nullptr)
			this->l->print(of);
		if(this->r != nullptr)
			this->r->print(of);
		for (auto it: this->values) {
			if (it != nullptr) {
				of << "<Parameter>\n";
				it->print(of);
				of << "</Parameter>\n";
			}
		}
		of << "</FuncCall>\n";
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}

void DefVar::print(std::ofstream &of) {
	std::string sym;
	if(this->type == Token::INT)
		sym = "int";
	else
		sym = "error";
	if(of.is_open()) {
		of << "<DefVar type=\"" << sym << "\">\n";
		for (auto it: this->values) {
			of << "<" << it.first << ">";
			if (it.second != nullptr) {
				it.second->print(of);
			}
			of << "</" << it.first << ">";
		}
		of << "</DefVar>\n";
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}

void Compound::print(std::ofstream &of) {
	if(of.is_open()) {
		of << "<Compound>\n";
		for (auto it: this->stmt) {
			if (it != nullptr)
				it->print(of);
		}
		of << "</Compound>\n";
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}

void DefFunc::print(std::ofstream &of) {
	std::string sym;
	if(this->type == Token::INT)
		sym = "int";
	if(this->type == Token::VOID)
		sym = "void";
	else
		sym = "void";
	if(of.is_open()) {
		of << "<DefFunc type=\"" << sym << "\" name=\"" << this->symbol << "\">\n";
		for (auto it: this->parameters) {
			if (it != nullptr)
				it->print(of);
		}
		if (this->compound != nullptr) {
			this->compound->print(of);
		}
		of << "</DefFunc>\n";
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}

void If::print(std::ofstream &of) {
	if(of.is_open()) {
		of << "<If>\n";
		if (this->exp != nullptr) {
			this->exp->print(of);
		}
		if (this->statement != nullptr) {
			this->statement->print(of);
		}
		of << "</If>\n";
		for (auto it: this->els) {
			if (it != nullptr) {
				it->print(of);
			}
		}
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}

void Else::print(std::ofstream &of) {
	if(of.is_open()) {
		of << "<Else>\n";
		if (this->statement != nullptr) {
			this->statement->print(of);
		}
		for (auto it: this->els) {
			if (it != nullptr) {
				it->print(of);
			}
		}
		of << "</Else>\n";
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}

void Iterator::print(std::ofstream &of) {
	if(of.is_open()) {
		of << "<Iterator type=\"" << this->symbol << "\">\n";
		for (auto it: this->lexpr) {
			if (it != nullptr)
				it->print(of);
		}
		if (this->statement != nullptr) {
			this->statement->print(of);
		}
		of << "</Iterator>\n";
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}

void Jump::print(std::ofstream &of) {
	if(of.is_open()) {
		of << "<Jump type=\"" << this->symbol << "\">\n";
		if (this->exp != nullptr) {
			this->exp->print(of);
		}
		of << "</Jump>\n";
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}

void Program::print(std::ofstream &of) {
	if(of.is_open()) {
		of << "<Program type=\"" << this->symbol << "\">\n";
		for (auto it: this->nodes) {
			if (it != nullptr)
				it->print(of);
		}
		of << "</Program>\n";
	} else {
		std::cout << "FATAL ERROR";
		of.close();
		exit(1);
	}
}
