#pragma once
#include "token.h"

typedef struct LEXER_STRUCT {
    char c;
    unsigned int i;
    char* contents; // source code to be parsed
} Lexer_T;

Lexer_T* Init_Lexer(char* contents);
void Lexer_Advance(Lexer_T* lexer);
void Lexer_Skip_WhiteSpace(Lexer_T* lexer);

Token_T* Lexer_Get_Next_Token(Lexer_T* lexer);
Token_T* Lexer_Collect_String(Lexer_T* lexer);
Token_T* Lexer_Collect_Number(Lexer_T* lexer);
Token_T* Lexer_Collect_Id(Lexer_T* lexer);
Token_T* Lexer_Advance_With_Token(Lexer_T* lexer, Token_T* token);
char* Lexer_Get_Current_Char_As_String(Lexer_T* lexer);
