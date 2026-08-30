#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "include/AST.h"

AST_T* Init_AST(int type){
    AST_T* ast = calloc(1, sizeof(struct AST_STRUCT));
    ast->type = type;

    ast->scope = (void*)0;

    ast->variable_definition_variable_name = (void*)0;
    ast->variable_definition_value = (void*)0;

    ast->variable_name = (void*)0;

    ast->function_definition_body = (void*)0;
    ast->function_definition_name = (void*)0;

    ast->function_definition_args = (void*)0;
    ast->function_definition_args_size = 0;

    ast->function_call_name = (void*)0;
    ast->function_call_arguments = (void*)0;
    ast->function_call_arguments_size = 0;

    ast->if_condition = (void*)0;
    ast->if_body = (void*)0;
    ast->if_else_body = (void*)0;

    ast->for_variable = (void*)0;
    ast->for_condition = (void*)0;
    ast->for_does_at_end = (void*)0;
    ast->for_body = (void*)0;

    ast->while_condition = (void*)0;
    ast->while_body = (void*)0;

    ast->binop_left = (void*)0;
    ast->binop_right = (void*)0;
    ast->binop_op = 0;

    ast->arrow_left = (void*)0;
    ast->arrow_right = (void*)0;

    ast->return_value = (void*)0;

    ast->string_value = (void*)0;

    ast->number_value = 0.0f;

    ast->bool_value = false;

    ast->table_definition_name = (void*)0;
    ast->table_definition_value = (void*)0;
    ast->table_value = (void*)0;
    ast->table_size = 0;

    ast->table_name = (void*)0;

    ast->dictionary_definition_value_name = (void*)0;
    ast->dictionary_definition_name = (void*)0;
    ast->dictionary_definition_value = (void*)0;
    ast->dictionary_value = (void*)0;
    ast->dictionary_size = 0;

    ast->dictionary_name = (void*)0;

    ast->class_definition_name = (void*)0;
    ast->class_definition_value = (void*)0;
    ast->class_definition_value_name = (void*)0;
    ast->class_value = (void*)0;
    ast->class_size = 0;

    ast->init_value = (void*)0;
    ast->init_args = (void*)0;
    ast->init_args_size = 0;
    ast->init_step = 0;

    ast->class_name = (void*)0;

    ast->compound_value = (void*)0;
    ast->compound_size = 0;

    return ast;
};
