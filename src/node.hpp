#ifndef NODO
#define NODO

#include <iostream>
#include <vector>
#include <fstream>
#include "token.hpp"

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

class Expression: public Node {
private:

public:
	Expression *r, *l;
	Expression(std::string s, int t): Node(s,t) { this-> r = this-> l = nullptr; };
	virtual ~Expression() { };
}

class Add: public Expression {
private:

public:
	Add (Expression *rv, std::string s, int t): Expression(s,t) { this->r = rv; };
	virtual ~Add ();
};

class Mul: public Expression {
private:

public:
	Mul (Expression *rv, std::string s, int t): Expression(s,t) { this->r = rv; };
	virtual ~Mul ();
};

class Comp: public Expression {
private:

public:
	Comp (Expression *rv, std::string s, int t): Expression(s,t) { this->r = rv; };
	virtual ~Comp ();
};

class Assign: public Expression {
private:

public:
	Assign (Expression *rv, std::string s, int t): Expression(s,t) { this->r = rv; };
	virtual ~Assign ();
};

class DefVar: public Node {
private:

public:
	std::vector< std::pair < Id*, Expression* > > values;
	DefVar (int t) { this->type = t; this->symbol = ""; };
	virtual ~DefVar ();
};

class DefFunc: public Node {
private:

public:
	Compound* compound;
	std::vector< std::pair < type, ID* > > parameters;
	DefFunc (std::string s, int t) Node(s,t);
	virtual ~DefFunc ();
};

class Compound: public Node {
private:

public:
	std::vector< Node* > stmt;
	Compound ();
	virtual ~Compound ();
};

class If: public Node {
private:

public:
	Expression* exp;
	Compound* comp;
	std::vector< Else* > v;
	If ();
	virtual ~If ();

};
class NodeT: public Node {
public:
	NodeT(std::string str, int t): Node(str,t) { };
    void print(std::ofstream &of);
};

#endif
