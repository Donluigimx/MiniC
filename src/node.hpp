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
	virtual void push(Node* nd) { };
};

class Parameter: public Node {
private:

public:
	Parameter (std::string s, int t): Node(s,t) { };
	virtual ~Parameter ();
};

class Expression: public Node {
private:

public:
	Expression *r, *l;
	Expression(std::string s, int t): Node(s,t) { this-> r = this-> l = nullptr; };
	virtual ~Expression() { };
};

class Add: public Expression {
private:

public:
	Add (Expression *rv, std::string s, int t): Expression(s,t) { this->r = rv; };
	virtual ~Add () { };
};

class Mul: public Expression {
private:

public:
	Mul (Expression *rv, std::string s, int t): Expression(s,t) { this->r = rv; };
	virtual ~Mul () { };
};

class Comp: public Expression {
private:

public:
	Comp (Expression *rv, std::string s, int t): Expression(s,t) { this->r = rv; };
	virtual ~Comp () { };
};

class Assign: public Expression {
private:

public:
	Assign (Expression *rv, std::string s, int t): Expression(s,t) { this->r = rv; };
	virtual ~Assign ()  ;
};

class DefVar: public Node {
private:

public:
	std::vector< std::pair < std::string, Expression* > > values;
	DefVar (int t) { this->type = t; this->symbol = ""; };
	virtual ~DefVar () { };
};

class Compound: public Node {
private:

public:
	std::vector< Node* > stmt;
	Compound ();
	virtual ~Compound () { };
};

class DefFunc: public Node {
private:

public:
	Compound* compound;
	std::vector< Parameter* > parameters;
	DefFunc (std::string s, int t) Node(s,t);
	virtual ~DefFunc () { };
};

class If: public Node {
private:

public:
	Expression* exp;
	Node* statement;
	std::vector< Else* > els;
	If ();
	virtual ~If () { };
};

class Else: public Node {
private:

public:
	Node* statement;
	std::vector< Else* > els;
	Else ();
	virtual ~Else () { };
};

class Iterator: public Node {
private:

public:
	Node* statement;
	std::vector< Expression* > lexpr;
	Iterator (std::string s, int t, Node* n, std::vector< Expression > v):
						Node(s,t) { this->statement = n; this->lexpr = v; };
	virtual ~Iterator() { };
};

class Jump: public Node {
private:

public:
	Jump (std::string s, int t): Node(s,t) { };
	virtual ~Jump ();

};

class Program: public Node {
private:

public:
	std::vector<Node* > nodes;
	Program ();
	virtual ~Program ();

};
#endif
