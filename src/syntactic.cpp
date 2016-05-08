#include "syntactic.hpp"
#include "node.hpp"

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
		aux = External_Declaration();
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
	Expression* expp = nullptr;
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
        expp = _Initializer();
        auxv = Initializer();
		defv->values.push_back(std::pair<std::string, Expression* >("1", expp));
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

DefFunc* Syntactic::_Parameter_List() {
    DefFunc* deff = nullptr;
	DefFunc* auxf = nullptr;
	int type;
    if (lexic->Token == Token::COMMA) {
		deff = new DefFunc("",0);
        lexic->Next();
		type = lexic->Token;
        Specifier();
        deff->parameters.push_back(new Parameter(lexic->Symbol, type));
        Check(Token::IDENTIFIER);
        auxf = _Parameter_List();
		if( auxf != nullptr)
			for(auto it: auxf->parameters)
				deff->parameters.push_back(it);
    }
    return deff;
}

DefVar* Syntactic::Initializer() {
	DefVar* defv = nullptr;
    DefVar* auxv = nullptr;
	Expression* expr = nullptr;
	std::string symbol;
    if (lexic->Token == Token::COMMA) {
		defv = new DefVar(0);
        lexic->Next();
		symbol = lexic->Symbol;
        Check(Token::IDENTIFIER);
        expr = _Initializer();
        auxv = Initializer();
		defv->values.push_back(std::pair<std::string, Expression* >(symbol,expr));
		if (auxv != nullptr)
			for(auto it: auxv->values)
				defv->values.push_back(it);
    }
    return defv;
}

Expression* Syntactic::_Initializer() {
    Expression* expr = nullptr;
    if (lexic->Token == Token::EQUAL) {
        lexic->Next();
        expr = expression();
    }
    return expr;
}

Compound* Syntactic::Compound_S() {
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

Compound* Syntactic::_Compound_S() {
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
    Node* nodent = nullptr;
    switch (lexic->Token) {
        case Token::BRACE_O:
        case Token::SEMICOLON:
            nodent = Compound_S();
            break;

        case Token::IF:
            nodent = Selection_S();
            break;

        case Token::WHILE:
        case Token::DO:
        case Token::FOR:
            nodent = Iteration_S();
            break;

        case Token::CONTINUE:
        case Token::BREAK:
        case Token::RETURN:
            nodent = Jump_S();
            break;

        case Token::NUMBER:
        case Token::IDENTIFIER:
        case Token::PARENTHESES_O:
            nodent = expression();
            break;

        default:
            Error();
            break;
    }
    return nodent;
}

If* Syntactic::Selection_S() {
    If* nodeif = new If();
	Else* nodels = nullptr;
    Check(Token::IF);
    Check("(");
    nodeif->exp = expression();
    Check(")");
    nodeif->statement = Statement_List();
    nodels = _Selection_S();
	if (nodels != nullptr) {
		for(auto it: nodels->els)
			nodeif->els.push_back(it);
		nodels->els.clear();
	}
    return nodeif;
}

Else* Syntactic::_Selection_S() {
    Else* els = nullptr;
	If* nodeif = nullptr;

    if ( lexic->Token == Token::ELSE ) {
        lexic->Next();
        els->statement = Statement_List();
		nodeif = dynamic_cast<If*>(els->statement);
		if (nodeif != nullptr) {
			for(auto it: nodeif->els)
				els->els.push_back(it);
			nodeif->els.clear();
		}
    }
    return els;
}

Iterator* Syntactic::Iteration_S() {
    Iterator* iter = new Iterator(lexic->Symbol, lexic->Token);
    if ( lexic->Token == Token::WHILE ) {
        lexic->Next();
        Check("(");
        iter->lexpr.push_back(expression());
        Check(")");
        iter->statement = Statement_List();
    } else if ( lexic->Token == Token::FOR ) {
        lexic->Next();
        Check("(");
        iter->lexpr.push_back(For_S());
        iter->lexpr.push_back(For_S());
        iter->lexpr.push_back(_For_S());
        Check(")");
        iter->statement = Statement_List();
    } else if ( lexic->Token == Token::DO ) {
        lexic->Next();
        iter->statement = Statement_List();
        Check(Token::WHILE);
        Check("(");
        iter->lexpr.push_back(expression());
        Check(")");
        Check(";");
    } else
        Error();
    return iter;
}

Expression* Syntactic::For_S() {
    Expression* nodent = nullptr;
    if (lexic->Token == Token::IDENTIFIER ||
        lexic->Token == Token::NUMBER) {
            nodent = Expression_S();
        }
    else
        Check(";");
    return nodent;
}

Expression* Syntactic::_For_S() {
    Expression* nodent = nullptr;
    if (lexic->Token == Token::IDENTIFIER ||
        lexic->Token == Token::NUMBER) {
            nodent = expression();
        }
    return nodent;
}

Expression* Syntactic::Expression_S() {
    Expression* expr = nullptr;
    expr = expression();
    Check(";");
    return expr;
}

Expression* Syntactic::expression() {
	Expression* ope = nullptr;
    Expression* oper = nullptr;
    ope = OP();
    oper = EQ();

	if (oper != nullptr)
		oper->l = ope;
	else
		oper = ope;

	return oper;
}

Expression* Syntactic::EQ() {
	Expression* ope = nullptr;
    Expression* aux = nullptr;
    Expression* aux2 = nullptr;

	std::string auxs;
	int auxt;
    if (lexic->Token == Token::EQUAL) {
		auxs = lexic->Symbol;
		auxt = lexic->Token;
        lexic->Next();
        aux = OP();
        aux2 = EQ();

		if (aux2 != nullptr)
			aux2->l = aux;
		else
			aux2 = aux;

		ope = new Assign(aux2, auxs, auxt);
    }
    return ope;
}
Expression* Syntactic::OP() {
	Expression* ope = nullptr;
    Expression* oper = nullptr;
    ope = ROP();
    oper = _OP();

	if (oper != nullptr)
		oper->l = ope;
	else
		oper = ope;

    return oper;
}

Expression* Syntactic::_OP() {
	Expression* ope = nullptr;
    Expression* aux = nullptr;
    Expression* aux2 = nullptr;

	std::string auxs;
	int auxt;
    if (lexic->Token == Token::DOUBLE_EQUAL ||
        lexic->Token == Token::NOT_EQUAL) {
            auxs = lexic->Symbol;
			auxt = lexic->Token;
            lexic->Next();
            aux = _OP();
			aux2 = ROP();

			if (aux2 != nullptr)
				aux2->l = aux;
			else
				aux2 = aux;

			ope = new Comp(aux2, auxs, auxt);
        }
    return ope;
}

Expression* Syntactic::ROP() {
	Expression* ope = nullptr;
    Expression* oper = nullptr;

    ope = E();
    oper = _ROP();

	if (oper != nullptr)
		oper->l = ope;
	else
		oper = ope;

    return oper;
}

Expression* Syntactic::_ROP() {
	Expression* ope = nullptr;
    Expression* aux = nullptr;
    Expression* aux2 = nullptr;

	std::string auxs;
	int auxt;
    if (lexic->Token == Token::GREATER ||
        lexic->Token == Token::LESS ||
        lexic->Token == Token::GREATER_OR_EQUAL ||
        lexic->Token == Token::LESS_OR_EQUAL) {
			auxs = lexic->Symbol;
			auxt = lexic->Token;
            lexic->Next();
            aux = E();
            aux2 = _ROP();
			if (aux2 != nullptr)
				aux2->l = aux;
			else
				aux2 = aux;

			ope = new Comp(aux2, auxs, auxt);
        }
    return ope;
}

Expression* Syntactic::E() {
	Expression* ope = nullptr;
    Expression* oper = nullptr;

    ope = T();
    oper = _E();

	if (oper != nullptr)
		oper->l = ope;
	else
		oper = ope;

    return oper;
}

Expression* Syntactic::_E() {
	Expression* ope = nullptr;
    Expression* aux = nullptr;
    Expression* aux2 = nullptr;

	std::string auxs;
	int auxt;
    if (lexic->Token == Token::PLUS ||
        lexic->Token == Token::MINUS) {
			 auxs = lexic->Symbol;
		   	 auxt = lexic->Token;
             lexic->Next();
             aux = T();
             aux2 = _E();
			 if (aux2 != nullptr)
 				aux2->l = aux;
 			else
 				aux2 = aux;

 			ope = new Add(aux2, auxs, auxt);
         }
    return ope;
}

Expression* Syntactic::T() {
	Expression* ope = nullptr;
    Expression* oper = nullptr;

    ope = F();
    oper = _T();

	if (oper != nullptr)
		oper->l = ope;
	else
		oper = ope;

    return oper;
}
Expression* Syntactic::_T() {
	Expression* ope = nullptr;
    Expression* aux = nullptr;
    Expression* aux2 = nullptr;

	std::string auxs;
	int auxt;
    if (lexic->Token == Token::MULTIPLICATION ||
        lexic->Token == Token::DIVISION ||
        lexic->Token == Token::MODULE) {
			auxs = lexic->Symbol;
			auxt = lexic->Token;
            lexic->Next();
            aux = F();
            aux2 = _T();
			if (aux2 != nullptr)
			   aux2->l = aux;
		   	else
			   aux2 = aux;

		   	ope = new Mul(aux2, auxs, auxt);
        }
    return ope;
}

Expression* Syntactic::F() {
    Expression* nodent = nullptr;
	FuncCall* aux = nullptr;
	std::string auxs = lexic->Symbol;
	int auxt = lexic->Token;
    if (lexic->Token == Token::IDENTIFIER) {
        lexic->Next();
        aux = FD();
		if (aux != nullptr) {
			aux->symbol = auxs;
			aux->type = auxt;
			nodent = aux;
		} else {
			nodent = new Id(auxs, auxt);
		}
    } else if (lexic->Token == Token::NUMBER) {
        nodent = new Value(lexic->Symbol, lexic->Token);
        lexic->Next();
    } else if (lexic->Token == Token::PARENTHESES_O) {
        lexic->Next();
        nodent = expression();
        Check(")");
    } else
        Error();
    return nodent;
}

FuncCall* Syntactic::FD() {
	FuncCall* funcc = nullptr;
    FuncCall* auxc = nullptr;
    if (lexic->Token == Token::PARENTHESES_O) {
		funcc = new FuncCall();
        lexic->Next();
        auxc = F_List();
        Check(")");
		if (auxc != nullptr) {
			for (auto it: auxc->values)
				funcc->values.push_back(it);
			delete auxc;
		}
    }
    return funcc;
}

FuncCall* Syntactic::F_List() {
	FuncCall* funcc = nullptr;
    FuncCall* auxc = nullptr;
    if (lexic->Token == Token::NUMBER ||
        lexic->Token == Token::IDENTIFIER ||
        lexic->Token == Token::PARENTHESES_O) {
			funcc = new FuncCall();
            funcc->values.push_back(expression());
            auxc = _F_List();
			if (auxc != nullptr) {
				for (auto it: auxc->values)
					funcc->values.push_back(it);
				delete auxc;
			}
        }
    return funcc;
}

FuncCall* Syntactic::_F_List() {
	FuncCall* funcc = nullptr;
    FuncCall* auxc = nullptr;
    if (lexic->Token == Token::COMMA) {
		funcc = new FuncCall();
        lexic->Next();
		funcc->values.push_back(expression());
		auxc = _F_List();
		if (auxc != nullptr) {
			for (auto it: auxc->values)
				funcc->values.push_back(it);
			delete auxc;
		}
    }
    return funcc;
}

Jump* Syntactic::Jump_S() {
    Jump* jump = new Jump(lexic->Symbol, lexic->Token);
    if (lexic->Token == Token::CONTINUE ||
        lexic->Token == Token::BREAK) {
            lexic->Next();
            Check(";");
        } else if (lexic->Token == Token::RETURN) {
            lexic->Next();
            jump->exp = _RR();
            Check(";");
        } else
            Error();
    return jump;
}

Expression* Syntactic::_RR() {
    Expression* nodent = nullptr;
    if (lexic->Token == Token::NUMBER ||
        lexic->Token == Token::IDENTIFIER ||
        lexic->Token == Token::PARENTHESES_O)
            nodent = expression();
    return nodent;
}

DefVar* Syntactic::Declaration_List() {
    DefVar* defv = nullptr;
    defv = Declarator();
    Check(";");
    return defv;
}

DefVar* Syntactic::Declarator() {
    DefVar* defv = nullptr;
    DefVar* auxv = nullptr;
	Expression* expr = nullptr;
	int type = lexic->Token;
	std::string symbol;
    Specifier();
    symbol = lexic->Symbol;
    Check(Token::IDENTIFIER);
    expr = _Initializer();
    auxv = Initializer();
	defv = new DefVar(type);
	defv->values.push_back( std::pair<std::string, Expression*>(symbol,expr));
	if (auxv != nullptr)
		for (auto it: auxv->values)
			defv->values.push_back(it);
    return defv;
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
