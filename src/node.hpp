#ifndef NODO
#define NODO

#include <iostream>
#include <vector>
#include <fstream>
#include "token.hpp"

//std::map<std::pair<std::string, std::string>, objeto> map;
class Node {
private:

public:
	std::string symbol;
	int type;

	Node(std::string str, int t): symbol(str), type(t) { };
	Node() { };
	virtual ~Node() { };
	virtual void print(std::ofstream &of);
	//virtual void push(Node* nd) { };
};

class Parameter: public Node {
private:

public:
	Parameter (std::string s, int t): Node(s,t) { };
	virtual ~Parameter () { };
	void print(std::ofstream &of);
};

class Expression: public Node {
private:

public:
	Expression *r, *l;
	Expression(std::string s, int t): Node(s,t) { this-> r = this-> l = nullptr; };
	virtual ~Expression() { };
	void print(std::ofstream &of);
};

class Add: public Expression {
private:

public:
	Add (Expression *rv, std::string s, int t): Expression(s,t) { this->r = rv; };
	virtual ~Add () { };
	void print(std::ofstream &of);
};

class Mul: public Expression {
private:

public:
	Mul (Expression *rv, std::string s, int t): Expression(s,t) { this->r = rv; };
	virtual ~Mul () { };
	void print(std::ofstream &of);
};

class Comp: public Expression {
private:

public:
	Comp (Expression *rv, std::string s, int t): Expression(s,t) { this->r = rv; };
	virtual ~Comp () { };
	void print(std::ofstream &of);
};

class Assign: public Expression {
private:

public:
	Assign (Expression *rv, std::string s, int t): Expression(s,t) { this->r = rv; };
	virtual ~Assign () { };
	void print(std::ofstream &of);
};

class Id: public Expression {
private:

public:
	Id (std::string s, int t): Expression(s,t) {  };
	virtual ~Id ()  { };
	void print(std::ofstream &of);
};

class Value: public Expression {
private:

public:
	Value (std::string s, int t): Expression(s,t) {  };
	virtual ~Value () { };
	void print(std::ofstream &of);
};

class FuncCall: public Expression {
private:

public:
	std::vector<Expression*> values;
	FuncCall (): Expression("",Token::FD) { };
	virtual ~FuncCall () { };
	void print(std::ofstream &of);
};

class DefVar: public Node {
private:

public:
	std::vector< std::pair < std::string, Expression* > > values;
	DefVar (int t) { this->type = t; this->symbol = ""; };
	virtual ~DefVar () { };
	void print(std::ofstream &of);
};

class Compound: public Node {
private:

public:
	std::vector< Node* > stmt;
	Compound () { };
	virtual ~Compound () { };
	void print(std::ofstream &of);
};

class DefFunc: public Node {
private:

public:
	Compound* compound;
	std::vector< Parameter* > parameters;
	DefFunc (std::string s, int t): Node(s,t) { };
	virtual ~DefFunc () { };
	void print(std::ofstream &of);
};

class Else: public Node {
private:

public:
	Node* statement;
	std::vector< Else* > els;
	Else ();
	virtual ~Else () { };
	void print(std::ofstream &of);
};

class If: public Node {
private:

public:
	Expression* exp;
	Node* statement;
	std::vector< Else* > els;
	If () { };
	virtual ~If () { };
	void print(std::ofstream &of);
};

class Iterator: public Node {
private:

public:
	Node* statement;
	std::vector< Expression* > lexpr;
	Iterator (std::string s, int t):
						Node(s,t) { this->statement = nullptr; };
	virtual ~Iterator() { };
	void print(std::ofstream &of);
};

class Jump: public Node {
private:

public:
	Expression* exp;
	Jump (std::string s, int t): Node(s,t) { this->exp = nullptr; };
	virtual ~Jump () { };
	void print(std::ofstream &of);
};

class Program: public Node {
private:

public:
	std::vector<Node* > nodes;
	Program () { };
	virtual ~Program () { };
	void print(std::ofstream &of);

};

#endif
