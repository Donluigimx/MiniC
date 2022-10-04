# Grammar #

```
TRANSLATION_UNIT -> EXTERNAL_DECLARATION TRANSLATION_UNIT
				   |ε

EXTERNAL_DECLARATION -> SPECIFIER id _EXTERNAL_DECLARATION

_EXTERNAL_DECLARATION -> ( PARAMETER_LIST ) COMPOUND_S|
						|_INITIALIZER INITIALIZER;

INITIALIZER -> , id _INITIALIZER INITIALIZER
			   |ε

_INITIALIZER -> = EXPRESSION
			   |ε

SPECIFIER -> int
			|void

COMPOUND_S -> { _COMPOUND_S }
			 |;

_COMPOUND_S -> DECLARATION_LIST _COMPOUND_S
			  |STATEMENT_LIST _COMPOUND_S
			  |ε

DECLARATION_LIST -> DECLARATOR ;

DECLARATOR -> SPECIFIER id _INITIALIZER INITIALIZER

STATEMENT_LIST -> EXPRESSION_S
				 |COMPOUND_S
				 |SELECTION_S
				 |ITERATION_S
				 |JUMP_S

JUMP_S -> continue;
		  break;
		  return _RR;

_RR -> EXPRESSION
	  |ε

PARAMETER_LIST -> SPECIFIER id _PARAMETER_LIST
				 |ε

_PARAMETER_LIST -> , SPECIFIER id _PARAMETER_LIST
				  |ε

SELECTION_S -> if ( EXPRESSION ) STATEMENT_LIST _SELECTION_S

_SELECTION_S -> else STATEMENT_LIST
			   |ε

ITERATION_S -> while( EXPRESSION ) STATEMENT_LIST
			  |do STATEMENT_LIST while (EXPRESSION);
			  |for (FOR_S FOR_S FOR_S) STATEMENT_LIST

FOR_S -> EXPRESSION_S
		|;

_FOR_S -> EXPRESSION
		 |ε

EXPRESSION_S -> EXPRESSION ;

EXPRESSION -> OP EQ

EQ -> = OP EQ
	 |ε

OP -> ROP _OP

_OP -> == ROP _OP
	 |!= ROP _OP
	 |ε

ROP -> E _ROP

_ROP -> < E _ROP
	   |> E _ROP
	   |<= E _ROP
	   |>= E _ROP

E -> T _E

_E-> + T _E
    |- T _E
    |ε

T -> F _T

_T-> * F _T
    |/ F _T
    |% F _T
    |ε

F -> id FD
	|value
	|(EXPRESSION)

FD -> |( F_LIST )
	  |ε

F_LIST -> EXPRESSION _F_LIST
		 |ε

_F_LIST -> , EXPRESSION _F_LIST
		  |ε
```
