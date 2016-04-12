#ifndef SYNTACTIC
#define SYNTACTIC
#include <iostream>
#include "lexic.hpp"

class Syntactic {

  public:
    Syntactic(Lexic*);

  private:
    Lexic* lexic;

    void Analize();

    void Check(int);
    void Check(std::string);

    void Error();

    void Translation_Unit();
    void External_Declaration();
    void _External_Declaration();
    void Specifier();
    void Parameter_List();
    void _Parameter_List();
    void Compound_S();
    void _Compound_S();
    void Initializer();
    void _Initializer();
    void Declaration_List();
    void Declarator();
    void Statement_List();
    void Expression_S();
    void Selection_S();
    void _Selection_S();
    void Iteration_S();
    void For_S();
    void _For_S();
    void Jump_S();
    void Expression();
    void EQ();
    void OP();
    void _OP();
    void ROP();
    void _ROP();
    void E();
    void _E();
    void T();
    void _T();
    void F();
    void FD();
    void F_List();
    void _F_List();
    void _RR();
};
#endif
