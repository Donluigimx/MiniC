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
    Node* Parameter_List();
    Node* _Parameter_List();
    Node* Compound_S();
    Node* _Compound_S();
    Node* Initializer();
    Node* _Initializer();
    Node* Declaration_List();
    Node* Declarator();
    Node* Statement_List();
    Node* Expression_S();
    Node* Selection_S();
    Node* _Selection_S();
    Node* Iteration_S();
    Node* For_S();
    Node* _For_S();
    Node* Jump_S();
    Node* Expression();
    Node* EQ();
    Node* OP();
    Node* _OP();
    Node* ROP();
    Node* _ROP();
    Node* E();
    Node* _E();
    Node* T();
    Node* _T();
    Node* F();
    Node* FD();
    Node* F_List();
    Node* _F_List();
    Node* _RR();
};
#endif
