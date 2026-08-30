#pragma once

#include <ctype.h>
#include <stdbool.h>

typedef struct AST_STRUCT {
    enum {
        AST_VARIABLE_DEFINITION,
        AST_TABLE_DEFINITION,
        AST_DICTIONARY_DEFINITION,
        AST_CLASS_DEFINITION,
        AST_FUNCTION_DEFINITION,
        AST_IF,
        AST_IF_ELSE,
        AST_FOR,
        AST_WHILE,
        AST_BINOP,
        AST_ARROW,
        AST_VARIABLE,
        AST_TABLE,
        AST_DICTIONARY,
        AST_CLASS,
        AST_FUNCTION_CALL,
        AST_STRING,
        AST_NUMBER,
        AST_BOOL,
        AST_RETURN,
        AST_COMPOUND,
        AST_NOOP
    } type;

    struct SCOPE_STRUCT* scope;

    /* AST_VARIABLE_DEFINTION */
    char* variable_definition_variable_name;
    struct AST_STRUCT* variable_definition_value;

    /* AST_VARIABLE */
    char* variable_name;

    /* AST_FUNCTION_DEFINITION */

    struct AST_STRUCT* function_definition_body;
    char* function_definition_name;
    struct AST_STRUCT** function_definition_args;
    size_t function_definition_args_size;

    /* AST_FUNCTION_CALL */
    char* function_call_name;
    struct AST_STRUCT** function_call_arguments;
    size_t function_call_arguments_size;

    /* AST_IF */

    struct AST_STRUCT* if_condition;
    struct AST_STRUCT* if_body;
    struct AST_STRUCT* if_else_body; /* AST_IF_ELSE */

    /* AST_FOR */

    struct AST_STRUCT* for_variable;
    struct AST_STRUCT* for_condition;
    struct AST_STRUCT* for_does_at_end;
    struct AST_STRUCT* for_body;

    /* AST_WHILE */

    struct AST_STRUCT* while_condition;
    struct AST_STRUCT* while_body;

    /* AST_BINOP */

    struct AST_STRUCT* binop_left;
    struct AST_STRUCT* binop_right;
    int binop_op;

    /* AST_ARROW */

    struct AST_STRUCT* arrow_left;
    struct AST_STRUCT* arrow_right;

    /* AST_RETURN */

    struct AST_STRUCT* return_value;

    /* AST_STRING*/
    char* string_value;

    /*AST_NUMBER*/
    float number_value;

    /*AST_BOOL*/
    bool bool_value;

    /*AST_TABLE_DEFINITION*/
    char* table_definition_name;
    struct AST_STRUCT** table_definition_value;
    struct AST_STRUCT* table_value;
    size_t table_size;

    /* AST_TABLE */
    char* table_name;

    /*AST_DICTIONARY_DEFINITION*/
    char* dictionary_definition_name;
    struct AST_STRUCT** dictionary_definition_value;
    struct AST_STRUCT** dictionary_definition_value_name;
    struct AST_STRUCT* dictionary_value;
    size_t dictionary_size;

    /* AST_DICTIONARY */
    char* dictionary_name;

    /*AST_CLASS_DEFINITION*/
    char* class_definition_name;
    struct AST_STRUCT** class_definition_value;
    struct AST_STRUCT** class_definition_value_name;
    struct AST_STRUCT* class_value;
    size_t class_size;

    struct AST_STRUCT* init_value;
    struct AST_STRUCT** init_args;
    size_t init_args_size;
    int init_step;

    /* AST_CLASS */
    char* class_name;

    /* AST_COMPUND*/
    struct AST_STRUCT** compound_value;
    size_t compound_size;
} AST_T;

AST_T* Init_AST(int type);
