#ifndef NODO
#define NODO

#include <iostream>
#include <vector>
#include <fstream>

class Node {
public:
	std::string symbol;
	int type;

	Node(std::string str, int t): symbol(str), type(t) { };
	Node() { };
	virtual ~Node() { };
	virtual void print(std::ofstream &of);
	virtual void push(Node* nd) { };
private:
};

class NodeNT: public Node {
public:
	NodeNT(std::string str, int t): Node(str,t) { };
	std::vector<Node*> tree;

    void print(std::ofstream &of);
	void push(Node* nd) { tree.push_back(nd); };
private:
};

class NodeT: public Node {
public:
	NodeT(std::string str, int t): Node(str,t) { };
    void print(std::ofstream &of);
};

#endif