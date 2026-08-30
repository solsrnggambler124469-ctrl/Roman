#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "include/lexer.h"

size_t content_size = 0;

Lexer_T* Init_Lexer(char* contents){
    Lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->contents = contents;
    lexer->i = 0;
    lexer->c = contents[lexer->i];

    content_size = strlen(lexer->contents);

    return lexer;
};

void Lexer_Advance(Lexer_T* lexer){
    if (lexer->c != '\0' && lexer->i < content_size) {
        lexer->i += 1;
        lexer->c = lexer->contents[lexer->i];
    }
};

void Lexer_Skip_WhiteSpace(Lexer_T* lexer){
    while (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' || lexer->c == '\r') {
        Lexer_Advance(lexer); // Au jina la fomula yako inayosonga mbele (e.g., lexer_next_char)
    }
};

int collected = 0;

Token_T* Lexer_Get_Next_Token(Lexer_T* lexer){
    while (lexer->c != '\0' && lexer->i < content_size) {
        Lexer_Skip_WhiteSpace(lexer);

        if (isdigit((signed char)lexer->c)) {
            return Lexer_Collect_Number(lexer);
        }
        if (lexer->c == '&' && collected == 0) {
            collected = 1;
            return Lexer_Collect_Id(lexer);
        } else {
            collected = 0;
        };
        if (isalnum((unsigned char)lexer->c) || lexer->c == '_') {
            return Lexer_Collect_Id(lexer);
        }
        if (lexer->c == '"') {
            return Lexer_Collect_String(lexer);
        }

        switch(lexer->c) {
            case '=': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_EQUALS, Lexer_Get_Current_Char_As_String(lexer))); break;
            case ';': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_SEMI, Lexer_Get_Current_Char_As_String(lexer))); break;
            case ':': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_COLON, Lexer_Get_Current_Char_As_String(lexer))); break;
            case '(': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_LPAREN, Lexer_Get_Current_Char_As_String(lexer))); break;
            case ')': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_RPAREN, Lexer_Get_Current_Char_As_String(lexer))); break;
            case '{': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_LCURLY, Lexer_Get_Current_Char_As_String(lexer))); break;
            case '}': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_RCURLY, Lexer_Get_Current_Char_As_String(lexer))); break;
            case '[': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_LSQUARE, Lexer_Get_Current_Char_As_String(lexer))); break;
            case ']': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_RSQUARE, Lexer_Get_Current_Char_As_String(lexer))); break;
            case ',': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_COMMA, Lexer_Get_Current_Char_As_String(lexer))); break;

            case '+': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_PLUS, Lexer_Get_Current_Char_As_String(lexer))); break;
            case '-': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_MINUS, Lexer_Get_Current_Char_As_String(lexer))); break;
            case '*': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_MULTIPLY, Lexer_Get_Current_Char_As_String(lexer))); break;
            case '/': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_DIVIDE, Lexer_Get_Current_Char_As_String(lexer))); break;
            case '^': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_EXPONENT, Lexer_Get_Current_Char_As_String(lexer))); break;
            case '%': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_MODULO, Lexer_Get_Current_Char_As_String(lexer))); break;

            case '>': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_ARROW, Lexer_Get_Current_Char_As_String(lexer))); break; // for children of an object
            case '&': return Lexer_Advance_With_Token(lexer, Init_Token(TOKEN_AMPERSAND, Lexer_Get_Current_Char_As_String(lexer))); break;
        }
    }

    return Init_Token(TOKEN_EOF, "\0");
};

Token_T* Lexer_Collect_String(Lexer_T* lexer){
    char* value = calloc(2, sizeof(char));
    value[0]='\0';
    Lexer_Advance(lexer);
    while (lexer->c != '"' && lexer->c != '\0') {
        char* s = Lexer_Get_Current_Char_As_String(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        free(s);
        Lexer_Advance(lexer);
    };
    Lexer_Advance(lexer);
    return Init_Token(TOKEN_STRING, value);
};

Token_T* Lexer_Collect_Number(Lexer_T* lexer) {
    char* value = calloc(2, sizeof(char));
    value[0]='\0';
    while (isdigit((unsigned char)lexer->c) || lexer->c == '.') {
        char* s = Lexer_Get_Current_Char_As_String(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        free(s);
        Lexer_Advance(lexer);
    };
    return Init_Token(TOKEN_NUMBER, value);
};

Token_T* Lexer_Collect_Id(Lexer_T* lexer){
    char* value = calloc(2, sizeof(char));
    value[0]='\0';
    while (isalnum((unsigned char)lexer->c) || lexer->c == '_') {
        char* s = Lexer_Get_Current_Char_As_String(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        free(s);
        Lexer_Advance(lexer);
    };
    return Init_Token(TOKEN_ID, value);
};

Token_T* Lexer_Advance_With_Token(Lexer_T* lexer, Token_T* token){
    Lexer_Advance(lexer);
    return token;
};

char* Lexer_Get_Current_Char_As_String(Lexer_T* lexer){
    char* str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';

    return str;
};
