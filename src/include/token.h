#pragma once

typedef struct TOKEN_STRUCT {
    enum {
        TOKEN_ID,
        TOKEN_EQUALS,
        TOKEN_STRING,
        TOKEN_NUMBER,
        TOKEN_SEMI,
        TOKEN_COLON,
        TOKEN_AMPERSAND,
        TOKEN_ARROW,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_COMMA,
        TOKEN_FUNCTION,
        TOKEN_LCURLY,
        TOKEN_RCURLY,
        TOKEN_LSQUARE,
        TOKEN_RSQUARE,
        TOKEN_PLUS,
        TOKEN_MINUS,
        TOKEN_MULTIPLY,
        TOKEN_DIVIDE,
        TOKEN_EXPONENT,
        TOKEN_MODULO,
        TOKEN_EOF
        //TOKEN_UNKNOWN
    } type;
    char* value;
} Token_T;

Token_T* Init_Token(int type, char* value);
