#include "syntactic.hpp"

Syntactic::Syntactic(Lexic* lex) {
    lexic = lex;
    Analize();
}

void Syntactic::Analize() {
    tree = Translation_Unit();
}

Node* Syntactic::Translation_Unit() {
    Program* nodent = new Program();
    Node* aux = nullptr;
    while( lexic->Token != Token::END_OF_FILE) {
		aux = External_Declaration()
		if (aux != nullptr)
        	nodent->nodes.push_back(aux);
	}
    return nodent;
}

Node* Syntactic::External_Declaration() {
	int type;
	std::string symbol;
    Node* nodent = nullptr;
	DefVar* defv = nullptr;
	DefFunc* deff = nullptr;

	type = lexic->Token;
    Specifier();
    symbol = lexic->Symbol;
    Check(Token::IDENTIFIER);
    nodent = _External_Declaration();

	defv = dynamic_cast<DefVar*>(nodent);
	deff = dynamic_cast<DefFunc*>(nodent);

	if(defv != nullptr) {
		defv->type = type;
		if(defv->values[0].first == "1")
			defv->values[0].first = symbol;
	} else if(deff != nullptr) {
		deff->type = type;
		deff->symbol = symbol;
	}
    return nodent;
}

Node* Syntactic::Specifier() {
    if (lexic->Token == Token::INT || lexic->Token == Token::VOID) {
        lexic->Next();
    } else
        Error();
    return nullptr;
}

Node* Syntactic::_External_Declaration() {
	DefVar* defv = nullptr;
    DefVar* auxv = nullptr;
	DefFunc* deff = nullptr;
	Compound* comp = nullptr;
	Expression* expr = nullptr;
	Node* nodent = nullptr;

    if (lexic->Token == Token::PARENTHESES_O) {
        lexic->Next();
        if (lexic->Token != Token::PARENTHESES_C)
            deff = Parameter_List();
		else
			deff = new DefFunc("",0);
        Check(")");
        comp = Compound_S();
		deff->compound = comp;
		nodent = deff;
    } else{
		defv = new DefVar(0);
        expr = _Initializer();
        auxv = Initializer();
		defv->values.push_back(std::pair<std::string, Expression* >("1", expr));
		if (auxv != nullptr)
			for(auto it: auxv->values)
				defv->values.push_back(it);
		nodent = defv;
    }
    return nodent;
}

DefFunc* Syntactic::Parameter_List() {
	DefFunc* deff = new DefFunc("",0);
	DefFunc* auxf = nullptr;
	int type;
	type = lexic->Token;
    Specifier();
	deff->parameters.push_back(new Parameter(lexic->Symbol, type));
    Check(Token::IDENTIFIER);
    auxf = _Parameter_List();
	if( auxf != nullptr)
		for(auto it: auxf->parameters)
			deff->parameters.push_back(it);
    return deff;
}

Node* Syntactic::_Parameter_List() {
    DefFunc* deff = nullptr;
	DefFunc* auxf = nullptr;
	int type;
    if (lexic->Token == Token::COMMA) {
		deff = new DefFunc("",0);
        lexic->Next();
		type = lexic->Token;
        Specifier()
        deff->parameters.push_back(new Parameter(lexic->Symbol, lexic->Token));
        Check(Token::IDENTIFIER);
        auxf = _Parameter_List();
		if( auxf != nullptr)
			for(auto it: auxf->parameters)
				deff->parameters.push_back(it);
    }
    return deff;
}

Node* Syntactic::Initializer() {
	DefVar* defv = nullptr;
    DefVar* auxv = nullptr;
	Expression* expr = nullptr;
	std::string symbol;
    if (lexic->Token == Token::COMMA) {
		defv
        lexic->Next();
		symbol = lexic->Symbol;
        Check(Token::IDENTIFIER);
        expr = _Initializer()
        auxv = Initializer();
		defv->values.push_back(std::pair<std::string, Expression* >(symbol,expr));
		if (auxv != nullptr)
			for(auto it: auxv->values)
				defv->values.push_back(it);
    }
    return defv;
}

Node* Syntactic::_Initializer() {
    Expression* expr = nullptr;
    if (lexic->Token == Token::EQUAL) {
        lexic->Next();
        expr = Expression();
    }
    return expr;
}

Node* Syntactic::Compound_S() {
	Compound* comp = nullptr;
    if (lexic->Token == Token::BRACE_O) {
        lexic->Next();
        comp = _Compound_S();
        Check("}");
    } else {
        Check(";");
    }
    return comp;
}

Node* Syntactic::_Compound_S() {
    Compound* comp = new Compound();
	Compound* aux = nullptr;
    if (lexic->Token == Token::INT || lexic->Token == Token::VOID) {
        comp->stmt.push_back(Declaration_List());
        aux = _Compound_S();
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
                comp->stmt.push_back(Statement_List());
                aux = _Compound_S();
                break;
        }
    }
	
	if(comp->stmt.empty()) {
		delete comp;
		comp = nullptr;
	}

	if(aux != nullptr) {
		for(auto it: aux->stmt)
			comp->stmt.push_back(it);
		delete aux;
	}
    return comp;
}

Node* Syntactic::Statement_List() {
    Node* nodent = new NodeNT("statement_list", Token::STATEMENT_LIST);
    switch (lexic->Token) {
        case Token::BRACE_O:
        case Token::SEMICOLON:
            nodent->push(Compound_S());
            break;

        case Token::IF:
            nodent->push(Selection_S());
            break;

        case Token::WHILE:
        case Token::DO:
        case Token::FOR:
            nodent->push(Iteration_S());
            break;

        case Token::CONTINUE:
        case Token::BREAK:
        case Token::RETURN:
            nodent->push(Jump_S());
            break;

        case Token::NUMBER:
        case Token::IDENTIFIER:
        case Token::PARENTHESES_O:
            nodent->push(Expression());
            break;

        default:
            Error();
            break;
    }
    return nodent;
}

Node* Syntactic::Selection_S() {
    Node* nodent = new NodeNT("selection_s", Token::SELECTION_S);
    Check(Token::IF);
    Check("(");
    nodent->push(Expression());
    Check(")");
    nodent->push(Statement_List());
    nodent->push(_Selection_S());
    return nodent;
}

Node* Syntactic::_Selection_S() {
    Node* nodent = new NodeNT("_selection_s", Token::_SELECTION_S);
    if ( lexic->Token == Token::ELSE ) {
        nodent->push(new NodeT(lexic->Symbol, lexic->Token));
        lexic->Next();
        nodent->push(Statement_List());
    }
    return nodent;
}

Node* Syntactic::Iteration_S() {
    Node* nodent = new NodeNT("iteration_s", Token::ITERATION_S);
    if ( lexic->Token == Token::WHILE ) {
        nodent->push(new NodeT(lexic->Symbol, lexic->Token));
        lexic->Next();
        Check("(");
        nodent->push(Expression());
        Check(")");
        nodent->push(Statement_List());
    } else if ( lexic->Token == Token::FOR ) {
        nodent->push(new NodeT(lexic->Symbol, lexic->Token));
        lexic->Next();
        Check("(");
        nodent->push(For_S());
        nodent->push(For_S());
        nodent->push(_For_S());
        Check(")");
        nodent->push(Statement_List());
    } else if ( lexic->Token == Token::DO ) {
        nodent->push(new NodeT(lexic->Symbol, lexic->Token));
        lexic->Next();
        nodent->push(Statement_List());
        nodent->push(new NodeT(lexic->Symbol, lexic->Token));
        Check(Token::WHILE);
        Check("(");
        nodent->push(Expression());
        Check(")");
        Check(";");
    } else
        Error();
    return nodent;
}

Node* Syntactic::For_S() {
    Node* nodent = new NodeNT("for_s", Token::FOR_S);
    if (lexic->Token == Token::IDENTIFIER ||
        lexic->Token == Token::NUMBER) {
            nodent->push(Expression_S());
        }
    else
        Check(";");
    return nodent;
}

Node* Syntactic::_For_S() {
    Node* nodent = new NodeNT("_for_s", Token::_FOR_S);
    if (lexic->Token == Token::IDENTIFIER ||
        lexic->Token == Token::NUMBER) {
            nodent->push(Expression());
        }
    return nodent;
}

Node* Syntactic::Expression_S() {
    Node* nodent = new NodeNT("expression_s", Token::EXPRESSION_S);
    nodent->push(Expression());
    Check(";");
    return nodent;
}

Node* Syntactic::Expression() {
    Node* nodent = new NodeNT("expression", Token::EXPRESSION);
    nodent->push(OP());
    nodent->push(EQ());
    return nodent;
}

Node* Syntactic::EQ() {
    Node* nodent = new NodeNT("eq", Token::EQ);
    if (lexic->Token == Token::EQUAL) {
        nodent->push(new NodeT(lexic->Symbol, lexic->Token));
        lexic->Next();
        nodent->push(OP());
        nodent->push(EQ());
    }
    return nodent;
}
Node* Syntactic::OP() {
    Node* nodent = new NodeNT("op", Token::OP);
    nodent->push(ROP());
    nodent->push(_OP());
    return nodent;
}

Node* Syntactic::_OP() {
    Node* nodent = new NodeNT("_op", Token::_OP);
    if (lexic->Token == Token::DOUBLE_EQUAL ||
        lexic->Token == Token::NOT_EQUAL) {
            nodent->push(new NodeT(lexic->Symbol, lexic->Token));
            lexic->Next();
            nodent->push(ROP());
            nodent->push(_OP());
        }
    return nodent;
}

Node* Syntactic::ROP() {
    Node* nodent = new NodeNT("rop", Token::ROP);
    nodent->push(E());
    nodent->push(_ROP());
    return nodent;
}

Node* Syntactic::_ROP() {
    Node* nodent = new NodeNT("_rop", Token::_ROP);
    if (lexic->Token == Token::GREATER ||
        lexic->Token == Token::LESS ||
        lexic->Token == Token::GREATER_OR_EQUAL ||
        lexic->Token == Token::LESS_OR_EQUAL) {
            nodent->push(new NodeT(lexic->Symbol, lexic->Token));
            lexic->Next();
            nodent->push(E());
            nodent->push(_ROP());
        }
    return nodent;
}

Node* Syntactic::E() {
    Node* nodent = new NodeNT("e", Token::E);
    nodent->push(T());
    nodent->push(_E());
    return nodent;
}

Node* Syntactic::_E() {
    Node* nodent = new NodeNT("_e", Token::_E);
    if (lexic->Token == Token::PLUS ||
        lexic->Token == Token::MINUS) {
             nodent->push(new NodeT(lexic->Symbol, lexic->Token));
             lexic->Next();
             nodent->push(T());
             nodent->push(_E());
         }
    return nodent;
}

Node* Syntactic::T() {
    Node* nodent = new NodeNT("t", Token::T);
    nodent->push(F());
    nodent->push(_T());
    return nodent;
}
Node* Syntactic::_T() {
    Node* nodent = new NodeNT("_t", Token::_T);
    if (lexic->Token == Token::MULTIPLICATION ||
        lexic->Token == Token::DIVISION ||
        lexic->Token == Token::MODULE) {
            nodent->push(new NodeT(lexic->Symbol, lexic->Token));
            lexic->Next();
            nodent->push(F());
            nodent->push(_T());
        }
    return nodent;
}

Node* Syntactic::F() {
    Node* nodent = new NodeNT("f", Token::F);
    if (lexic->Token == Token::IDENTIFIER) {
        nodent->push(new NodeT(lexic->Symbol, lexic->Token));
        lexic->Next();
        nodent->push(FD());
    } else if (lexic->Token == Token::NUMBER) {
        nodent->push(new NodeT(lexic->Symbol, lexic->Token));
        lexic->Next();
    } else if (lexic->Token == Token::PARENTHESES_O) {
        lexic->Next();
        nodent->push(Expression());
        Check(")");
    } else
        Error();
    return nodent;
}

Node* Syntactic::FD() {
    Node* nodent = new NodeNT("fd", Token::FD);
    if (lexic->Token == Token::PARENTHESES_O) {
        lexic->Next();
        nodent->push(F_List());
        Check(")");
    }
    return nodent;
}

Node* Syntactic::F_List() {
    Node* nodent = new NodeNT("f_list", Token::F_LIST);
    if (lexic->Token == Token::NUMBER ||
        lexic->Token == Token::IDENTIFIER ||
        lexic->Token == Token::PARENTHESES_O) {
            nodent->push(Expression());
            nodent->push(_F_List());
        }
    return nodent;
}

Node* Syntactic::_F_List() {
    Node* nodent = new NodeNT("_f_list", Token::_F_LIST);
    if (lexic->Token == Token::COMMA) {
        lexic->Next();
        nodent->push(Expression());
        nodent->push(_F_List());
    }
    return nodent;
}

Node* Syntactic::Jump_S() {
    Node* nodent = new NodeNT("jump_s", Token::JUMP_S);
    if (lexic->Token == Token::CONTINUE ||
        lexic->Token == Token::BREAK) {
            nodent->push(new NodeT(lexic->Symbol, lexic->Token));
            lexic->Next();
            Check(";");
        } else if (lexic->Token == Token::RETURN) {
            nodent->push(new NodeT(lexic->Symbol, lexic->Token));
            lexic->Next();
            nodent->push(_RR());
            Check(";");
        } else
            Error();
    return nodent;
}

Node* Syntactic::_RR() {
    Node* nodent = new NodeNT("_rr", Token::_RR);
    if (lexic->Token == Token::NUMBER ||
        lexic->Token == Token::IDENTIFIER ||
        lexic->Token == Token::PARENTHESES_O)
            nodent->push(Expression());
    return nodent;
}

Node* Syntactic::Declaration_List() {
    Node* nodent = new NodeNT("declaration_list", Token::DECLARATION_LIST);
    nodent->push(Declarator());
    Check(";");
    return nodent;
}

Node* Syntactic::Declarator() {
    Node* nodent = new NodeNT("declarator", Token::DECLARATOR);
    nodent->push(Specifier());
    nodent->push(new NodeT(lexic->Symbol, lexic->Token));
    Check(Token::IDENTIFIER);
    nodent->push(_Initializer());
    nodent->push(Initializer());
    return nodent;
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
