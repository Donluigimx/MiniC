#include <iostream>
#include "node.hpp"

std::string Node::context = "";
bool Node::isOk = true;
std::map<std::pair<std::string, std::string>, SymbolDef> Node::symtable;
std::map<std::string,int> regval;
std::vector<std::string> regpass;

int charType(char c) {
	if (c == 'i')
		return Token::INT;
	if (c == 'v')
		return Token::VOID;

	return -1;
}

void init() {
	regpass.push_back(" ");
	regpass.push_back("%edi");
	regpass.push_back("%esi");
	regpass.push_back("%edx");
	regpass.push_back("%ecx");
	regpass.push_back("%r8d");
	regpass.push_back("%r9d");
}
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
		sym = "int";
	else
		sym = "error";
	if(of.is_open()) {
		of << "<Parameter value=\"" << sym << "\">\n";
		of << this->symbol << "\n";
		of << "</Parameter>\n";
	}
}

void Parameter::analysis() {
	SymbolDef sym;
	auto it = Parameter::symtable.find(std::pair<std::string, std::string>(this->symbol,Parameter::context));
	if (it == Parameter::symtable.end()) {
		sym.dataType = this->type;
		sym.type = Token::PARAMETER;
		sym.isDef = true;
		Parameter::symtable.insert(std::pair<std::pair<std::string, std::string>, SymbolDef>(std::pair<std::string, std::string>(this->symbol,Parameter::context), sym));
	} else {
		std::cout << "Error in " << Parameter::context;
		std::cout << "," << this->symbol << "is redefined."<< std::endl;
		Parameter::isOk = false;
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

void Mul::analysis() {
	this->l->analysis();
	this->r->analysis();

	if (this->l->dataType == this->r->dataType) {
		if (this->l->dataType != 'e' && this->r->dataType != 'e') {
			this->dataType = this->l->dataType;
		} else {
			std::cout << "Error in " << Node::context;
			std::cout << ", Error Mul." << std::endl;
			Mul::isOk = false;
		}
	} else {
		std::cout << "Error in " << Node::context;
		std::cout << ", an expression inside does not have the same data type." << std::endl;
		Mul::isOk = false;
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

void Add::analysis() {
	this->l->analysis();
	this->r->analysis();

	if (this->l->dataType == this->r->dataType) {
		if (this->l->dataType != 'e' && this->r->dataType != 'e') {
			this->dataType = this->l->dataType;
		} else {
			std::cout << "Error in " << Node::context;
			std::cout << ", Error Add." << std::endl;
			Add::isOk = false;
		}
	} else {
		std::cout << "Error in " << Node::context;
		std::cout << ", an expression inside does not have the same data type." << std::endl;
		Add::isOk = false;
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

void Comp::analysis() {
	this->l->analysis();
	this->r->analysis();

	if (this->l->dataType == this->r->dataType) {
		if (this->l->dataType != 'e' && this->r->dataType != 'e') {
			this->dataType = this->l->dataType;
		} else {
			std::cout << "Error in " << Node::context;
			std::cout << ", Error Comparison." << std::endl;
			Comp::isOk = false;
		}
	} else {
		std::cout << "Error in " << Node::context;
		std::cout << ", an expression inside does not have the same data type." << std::endl;
		Comp::isOk = false;
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

void Assign::analysis() {
	this->l->analysis();
	this->r->analysis();

	if (this->l->dataType == this->r->dataType) {
		if (this->l->dataType != 'e' && this->r->dataType != 'e') {
			this->dataType = this->l->dataType;
		} else {
			std::cout << "Error in " << Node::context;
			std::cout << ", Error Assign." << std::endl;
			Assign::isOk = false;
		}
	} else {
		std::cout << "Error in " << Node::context;
		std::cout << ", an expression inside does not have the same data type." << std::endl;
		Assign::isOk = false;
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

void Id::analysis() {
	auto it = Id::symtable.find(std::pair<std::string, std::string>(this->symbol, Id::context));
	if (it != Id::symtable.end()) {
		if (it->second.dataType == Token::INT) {
			this->dataType = 'i';
		} else if (it->second.dataType == Token::VOID) {
			this->dataType = 'v';
		} else {
			this->dataType = 'e';
		}
	} else {
		std::cout << "Error in " << Node::context;
		std::cout << ", " << this->symbol << " is not defined in this context." << std::endl;
		Id::isOk = false;
		this->dataType = 'e';
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

void Value::analysis() {
	this->dataType = 'i';
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

void FuncCall::analysis() {
	auto it = FuncCall::symtable.find(std::pair<std::string, std::string>(this->symbol, ""));
	int i = 0;
	if (it != FuncCall::symtable.end()) {
		if (it->second.type == Token::FUNCTION) {
			if (it->second.dataType == Token::INT) {
				this->dataType = 'i';
			} else if (it->second.dataType == Token::VOID) {
				this->dataType = 'v';
			} else {
				this->dataType = 'e';
			}
			if (this->values.size() == it->second.parameters.size()) {
				for (auto iti: this->values) {
					iti->analysis();
					auto aux = FuncCall::symtable.find(std::pair<std::string, std::string>(it->second.parameters[i], this->symbol));
					if (aux->second.dataType == charType(iti->dataType)) {
						i++;
						continue;
					} else {
						std::cout << "Error in " << Node::context;
						std::cout << ", " << this->symbol << " function parameter data type does not match." << std::endl;
						FuncCall::isOk = false;
						this->dataType = 'e';
						break;
					}
				}
			} else {
				std::cout << "Error in " << Node::context;
				std::cout << ", " << this->symbol << " function parameters quantity does not match." << std::endl;
				FuncCall::isOk = false;
				this->dataType = 'e';
			}
		} else {
			std::cout << "Error in " << Node::context;
			std::cout << ", " << this->symbol << " is not a function." << std::endl;
			FuncCall::isOk = false;
			this->dataType = 'e';
		}
	} else {
		std::cout << "Error in " << Node::context;
		std::cout << ", " << this->symbol << " function is not defined in this context." << std::endl;
		FuncCall::isOk = false;
		this->dataType = 'e';
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

void DefVar::analysis() {
	SymbolDef sym;
	sym.type = Token::IDENTIFIER;
	sym.dataType = this->type;
	sym.isDef = true;
	for (auto i: this->values) {
		auto it = DefVar::symtable.find(std::pair<std::string, std::string>(i.first, DefVar::context));
		if (it == DefVar::symtable.end())
			DefVar::symtable.insert(std::pair<std::pair<std::string, std::string>, SymbolDef>(std::pair<std::string, std::string>(i.first,Parameter::context), sym));
		else {
			std::cout << "Error in " << Node::context;
			std::cout << ", " << i.first << " is already defined in this context." << std::endl;
			DefVar::isOk = false;
		}
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

void Compound::analysis() {
	for (auto i: this->stmt) {
		if (i != nullptr)
			i->analysis();
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

void DefFunc::analysis() {
	auto it = DefFunc::symtable.find(std::pair<std::string, std::string>(this->symbol, DefFunc::context));
	SymbolDef sym;

	sym.dataType = this->type;
	sym.type = Token::FUNCTION;

	DefFunc::context = this->symbol;
	if (it != DefFunc::symtable.end()) {
		if (!it->second.isDef) {
			if (this->parameters.size() == it->second.parameters.size()) {
				int i = 0;
				for (auto iti: this->parameters) {
					auto aux = DefFunc::symtable.find(std::pair<std::string, std::string>(it->second.parameters[i], this->symbol));
					if (aux->second.dataType == iti->type) {
						i++;
						continue;
					} else {
						std::cout << "Error in " << DefFunc::context;
						std::cout << ", " << " function parameter data type does not match." << std::endl;
						DefFunc::isOk = false;
						break;
					}
				}
				if (DefFunc::isOk && this->compound != nullptr) {
					this->compound->analysis();
					it->second.isDef = true;
				} else {
					std::cout << "Redefinition of " << DefFunc::context << "." << std::endl;
					FuncCall::isOk = false;
				}
			} else {
				std::cout << "Error in " << DefFunc::context;
				std::cout << ", " << " function parameters quantity does not match." << std::endl;
				FuncCall::isOk = false;
			}
		} else {
			std::cout << "Redefinition of " << DefFunc::context << "." << std::endl;
			FuncCall::isOk = false;
		}
	} else {
		for (auto iti: this->parameters) {
			iti->analysis();
			sym.parameters.push_back(iti->symbol);
		}
		if (this->compound != nullptr) {
			this->compound->analysis();
			sym.isDef = true;
		} else {
			sym.isDef = false;
		}

		FuncCall::symtable.insert(std::pair<std::pair<std::string, std::string>, SymbolDef>(std::pair<std::string, std::string>(this->symbol, ""), sym));
	}
	DefFunc::context = "";
}

void DefFunc::code(std::ofstream &of) {
	int c = 1;
	int rest = 0;
	if (this->compound == nullptr)
		return;

	for (auto it: this->parameters) {
		regval.insert(std::pair<std::string, int>(it->symbol,c));
		c++;
	}

	for (auto it: DefFunc::symtable) {
		if (it.first.second == this->symbol) {
			if (it.second.type != Token::PARAMETER) {
				regval.insert(std::pair<std::string, int>(it.first.first,c));
				c++;
			}
		}
	}

	of << ".global " << this->symbol << "\n";
	of << ".type " << this->symbol << " ,@function\n";
	of << this->symbol << ":\n";
	of << "pushq %rbp\n";
	of << "movq %rsp %rbp\n";
	rest = (regval.size()*4)+8;
	rest = rest + (rest%8);
	of << "subq $" << rest << " %rsp\n";

	for (auto it: this->parameters) {
		int i = regval.find(it->symbol)->second;
		of << "movl " << regpass[i] << " " << i*-4 << "(%rbp)\n";
	}

	of << ";\n;\n";
	of << "popq %rbp\nleave\nret\n\n";
	regval.clear();
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

void If::analysis() {
	if (this->exp != nullptr)
		this->exp->analysis();
	if (this->statement != nullptr)
		this->statement->analysis();

	for (auto it: this->els)
		it->analysis();
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

void Else::analysis() {
	if (this->statement != nullptr)
		this->statement->analysis();
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

void Iterator::analysis() {
	for (auto it: this->lexpr)
		if (it != nullptr)
			it->analysis();
	if (this->statement != nullptr)
		this->statement->analysis();
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

void Jump::analysis() {
	if (this->exp != nullptr)
		this->exp->analysis();
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

void Program::analysis() {
	for (auto it: this->nodes)
		it->analysis();

	auto main = Program::symtable.find(std::pair<std::string, std::string>("main", ""));
	if (main == Program::symtable.end()) {
		std::cout << "main is not defined." << std::endl;
		Program::isOk = false;
	}

	for (auto val: Program::symtable) {
		if (!val.second.isDef) {
			std::cout << "Error on definition of " << val.first.first << std::endl;
			Program::isOk = false;
		}
	}
}

void Program::code(std::ofstream &of) {
	of << ".text\n";
	init();
	for (auto it: this->nodes) {
		it->code(of);
	}
}
