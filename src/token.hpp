#ifndef TOKEN
#define TOKEN

namespace Token {

  enum {
    IDENTIFIER = 1,
    NUMBER,
    MULTIPLICATION,
    PLUS,
    MINUS,
    DIVISION,
    MODULE,
    GREATER,
    GREATER_OR_EQUAL,
    LESS,
    LESS_OR_EQUAL,
    EQUAL,
    DOUBLE_EQUAL,
    NOT_EQUAL = 15,
    PARENTHESES_O,
    PARENTHESES_C,
    BRACE_O,
    BRACE_C,
    SEMICOLON,
    COMMENT,
    COMMENT_O = 23,
    COMMA,
    ERROR,
    IF,
    INT,
    VOID,
    BREAK,
    CONTINUE,
    ELSE,
    FOR,
    WHILE,
    DO,
    RETURN,
    END_OF_FILE
  };
};
#endif
