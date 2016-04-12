#include "syntactic.hpp"

Syntactic::Syntactic(Lexic* lex) {
    lexic = lex;
    Analize();
}

void Syntactic::Analize() {
    Translation_Unit();
}

void Syntactic::Translation_Unit() {
    while( lexic->Token != Token::END_OF_FILE)
        External_Declaration();
}

void Syntactic::External_Declaration() {
    Specifier();
    Check(Token::IDENTIFIER);
    _External_Declaration();
}

void Syntactic::Specifier() {
    if (lexic->Token == Token::INT || lexic->Token == Token::VOID) {
        lexic->Next();
        return;
    } else
        Error();
}

void Syntactic::_External_Declaration() {
    if (lexic->Token == Token::PARENTHESES_O) {
        lexic->Next();
        if (lexic->Token != Token::PARENTHESES_C)
            Parameter_List();
        Check(")");
        Compound_S();
    } else{
        _Initializer();
        Initializer();
    }
}

void Syntactic::Parameter_List() {
    Specifier();
    Check(Token::IDENTIFIER);
    _Parameter_List();
}

void Syntactic::_Parameter_List() {
    if (lexic->Token == Token::COMMA) {
        lexic->Next();
        Specifier();
        Check(Token::IDENTIFIER);
        _Parameter_List();
    }
}

void Syntactic::Initializer() {
    if (lexic->Token == Token::COMMA) {
        lexic->Next();
        Check(Token::IDENTIFIER);
        _Initializer();
        Initializer();
    }
}

void Syntactic::_Initializer() {
    if (lexic->Token == Token::EQUAL) {
        lexic->Next();
        Expression();
    }
}

void Syntactic::Compound_S() {
    if (lexic->Token == Token::BRACE_O) {
        lexic->Next();
        _Compound_S();
        Check("}");
    } else {
        Check(";");
    }
}

void Syntactic::_Compound_S() {
    if (lexic->Token == Token::INT || lexic->Token == Token::VOID) {
        Declaration_List();
        _Compound_S();
    } else {
        switch (lexic->Token) {
            case Token::BRACE_O:
            case Token::SEMICOLON:
            case Token::IF:
            case Token::WHILE:
            case Token::DO:
            case Token::FOR:
            case Token::CONTINUE:
            case Token::BREAK:
            case Token::RETURN:
            case Token::NUMBER:
            case Token::IDENTIFIER:
            case Token::PARENTHESES_O:
                Statement_List();
                _Compound_S();
                break;
        }
    }
}

void Syntactic::Statement_List() {
    switch (lexic->Token) {
        case Token::BRACE_O:
        case Token::SEMICOLON:
            Compound_S();
            break;

        case Token::IF:
            Selection_S();
            break;

        case Token::WHILE:
        case Token::DO:
        case Token::FOR:
            Iteration_S();
            break;

        case Token::CONTINUE:
        case Token::BREAK:
        case Token::RETURN:
            Jump_S();
            break;

        case Token::NUMBER:
        case Token::IDENTIFIER:
        case Token::PARENTHESES_O:
            Expression();
            break;

        default:
            Error();
            break;
    }
}

void Syntactic::Selection_S() {
    Check(Token::IF);
    Check("(");
    Expression();
    Check(")");
    Statement_List();
    _Selection_S();
}

void Syntactic::_Selection_S() {
    if ( lexic->Token == Token::ELSE ) {
        lexic->Next();
        Statement_List();
    }
}

void Syntactic::Iteration_S() {
    if ( lexic->Token == Token::WHILE ) {
        lexic->Next();
        Check("(");
        Expression();
        Check(")");
        Statement_List();
    } else if ( lexic->Token == Token::FOR ) {
        lexic->Next();
        Check("(");
        For_S();
        For_S();
        _For_S();
        Check(")");
        Statement_List();
    } else if ( lexic->Token == Token::DO ) {
        lexic->Next();
        Statement_List();
        Check(Token::WHILE);
        Check("(");
        Expression();
        Check(")");
        Check(";");
    } else
        Error();
}

void Syntactic::For_S() {
    if (lexic->Token == Token::IDENTIFIER ||
        lexic->Token == Token::NUMBER) {
            Expression_S();
        }
    else
        Check(";");
}

void Syntactic::_For_S() {
    if (lexic->Token == Token::IDENTIFIER ||
        lexic->Token == Token::NUMBER) {
            Expression();
        }
}

void Syntactic::Expression_S() {
    Expression();
    Check(";");
}

void Syntactic::Expression() {
    OP();
    EQ();
}

void Syntactic::EQ() {
    if (lexic->Token == Token::EQUAL) {
        lexic->Next();
        OP();
        EQ();
    }
}
void Syntactic::OP() {
    ROP();
    _OP();
}

void Syntactic::_OP() {
    if (lexic->Token == Token::DOUBLE_EQUAL ||
        lexic->Token == Token::NOT_EQUAL) {
            lexic->Next();
            ROP();
            _OP();
        }
}

void Syntactic::ROP() {
    E();
    _ROP();
}

void Syntactic::_ROP() {
    if (lexic->Token == Token::GREATER ||
        lexic->Token == Token::LESS ||
        lexic->Token == Token::GREATER_OR_EQUAL ||
        lexic->Token == Token::LESS_OR_EQUAL) {
            lexic->Next();
            E();
            _ROP();
        }
}

void Syntactic::E() {
    T();
    _E();
}

void Syntactic::_E() {
    if (lexic->Token == Token::PLUS ||
        lexic->Token == Token::MINUS) {
             lexic->Next();
             T();
             _E();
         }
}

void Syntactic::T() {
    F();
    _T();
}
void Syntactic::_T() {
    if (lexic->Token == Token::MULTIPLICATION ||
        lexic->Token == Token::DIVISION ||
        lexic->Token == Token::MODULE) {
            lexic->Next();
            F();
            _T();
        }
}

void Syntactic::F() {
    if (lexic->Token == Token::IDENTIFIER) {
        lexic->Next();
        FD();
    } else if (lexic->Token == Token::NUMBER) {
        lexic->Next();
    } else if (lexic->Token == Token::PARENTHESES_O) {
        lexic->Next();
        Expression();
        Check(")");
    } else
        Error();
}

void Syntactic::FD() {
    if (lexic->Token == Token::PARENTHESES_O) {
        lexic->Next();
        F_List();
        Check(")");
    }
}

void Syntactic::F_List() {
    if (lexic->Token == Token::NUMBER ||
        lexic->Token == Token::IDENTIFIER ||
        lexic->Token == Token::PARENTHESES_O) {
            Expression();
            _F_List();
        }

}

void Syntactic::_F_List() {
    if (lexic->Token == Token::COMMA) {
        lexic->Next();
        Expression();
        _F_List();
    }
}

void Syntactic::Jump_S() {
    if (lexic->Token == Token::CONTINUE ||
        lexic->Token == Token::BREAK) {
            lexic->Next();
            Check(";");
        } else if (lexic->Token == Token::RETURN) {
            lexic->Next();
            _RR();
            Check(";");
        } else
            Error();
}

void Syntactic::_RR() {
    if (lexic->Token == Token::NUMBER ||
        lexic->Token == Token::IDENTIFIER ||
        lexic->Token == Token::PARENTHESES_O)
            Expression();
}

void Syntactic::Declaration_List() {
    Declarator();
    Check(";");
}

void Syntactic::Declarator() {
    Specifier();
    Check(Token::IDENTIFIER);
    _Initializer();
    Initializer();
}

void Syntactic::Check(int value) {
    if (value == lexic->Token) {
        lexic->Next();
    } else {
        Error();
    }
}

void Syntactic::Check(std::string value) {
    if (value == lexic->Symbol) {
        lexic->Next();
    } else
        Error();
}

void Syntactic::Error() {
    std::cout << "ERROR IN SYNTACTIC PHASE" << std::endl;
    exit(0);
}