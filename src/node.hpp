#ifndef NODE
#define NODE

#include <iostream>

class Node{
  public:
    std::string Symbol;
    Node(std::string);
    Node(){};

    virtual void showMustGoOn();
};

class ID: public Node{
	public:
	    ID* next;
	    ID(std::string str) : Node(str){ next = nullptr; };
};

class Type: public Node{
  public:
    Type(std::string str) : Node(str){};
};

class DefVar: public Node{
  public:
    Type* type;
    ID* id;
    DefVar(ID*,Type*);
};

class Expression: public Node {
    public:
        Expression(){this->l = nullptr; this->r = nullptr;};
        Expression *l, *r;
};

class Value: public Expression {
    public:
        Value(std::string str);
};

class Operator: public Expression {
    public:
        Operator(Expression *rv, std::string str);
};
#endif
