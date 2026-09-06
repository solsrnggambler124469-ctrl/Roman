#pragma once

#include "token.h"
#include "AST.h"
#include "lexer.h"
#include "scope.h"

typedef struct PARSER_STRUCT {
    Lexer_T* lexer;
    Token_T* current_token;
    Token_T* previous_token;
    Scope_T* Scope;

    char** table_names;
    size_t table_names_size;

    char** dict_names;
    size_t dict_names_size;

    char** class_names;
    size_t class_names_size;

    char** included_paths;
    size_t included_paths_size;
} Parser_T;

int Parser_Is_Known_Table(
    Parser_T*,
    const char*
);

int Parser_Is_Known_Class(
    Parser_T*,
    const char*
);

int Parser_Is_Known_Dict(
    Parser_T*,
    const char*
);

Parser_T* Init_Parser(
    Lexer_T*
);

void Parser_Eat(
    Parser_T*,
    int
);

AST_T* Parser_Parse(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Statement(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Statements(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Expr(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Factor(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Term(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Table_Definition(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Table(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Dictionary_Definition(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Dictionary(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Class_Definition(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Class(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Function_Definition(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Function_Call(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_If(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_If_Else(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_For(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_While(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Return(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_String(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Number(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Variable_Definition(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Variable(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Id(
    Parser_T*,
    Scope_T*
);

AST_T* Parser_Parse_Include(
    Parser_T*,
    Scope_T*
);
