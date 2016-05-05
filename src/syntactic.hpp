#ifndef SYNTACTIC
#define SYNTACTIC

#include <iostream>
#include "lexic.hpp"
#include "node.hpp"

class Syntactic {

  public:
    Syntactic(Lexic*);
    Node* tree;

  private:
    Lexic* lexic;

    void Analize();

    void Check(int);
    void Check(std::string);

    void Error();

    Node* Translation_Unit();
    Node* External_Declaration();
    Node* _External_Declaration();
    Node* Specifier();
    DefFunc* Parameter_List();
    DefFunc* _Parameter_List();
    Compound* Compound_S();
    Compound* _Compound_S();
    DefVar* Initializer();
    Expression* _Initializer();
    DefVar* Declaration_List();
    DefVar* Declarator();
    Node* Statement_List();
    Expression* Expression_S();
    If* Selection_S();
    Else* _Selection_S();
    Iterator* Iteration_S();
    Expression* For_S();
    Expression* _For_S();
    Jump* Jump_S();
    Expression* expression();
    Expression* EQ();
    Expression* OP();
    Expression* _OP();
    Expression* ROP();
    Expression* _ROP();
    Expression* E();
    Expression* _E();
    Expression* T();
    Expression* _T();
    Expression* F();
    FuncCall* FD();
    FuncCall* F_List();
    FuncCall* _F_List();
    Expression* _RR();
};
#endif
