#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "scope.h"
#include "AST.h"
#include "visitor.h"

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))

static char *removeSub(const char *str, const char *sub, char *new) {
    char *p = new;
    size_t len = strlen(sub);
    if (len > 0) {
        const char *match;
        while ((match = strstr(str, sub)) != NULL) {
            memcpy(p, str, match - str);
            p += match - str;
            str = match + len;
        }
    }
    strcpy(p, str);
    return new;
};

AST_T* builtin_function_print(Visitor_T* visitor, AST_T** args, int args_size){
    if (args_size <= 0) {
        printf("Tripped on function 'print', argument overflow. (0 to 1)\n");
        exit(1);
    };

    for (int i=0;i<args_size;i++){
        AST_T* visited_ast = Visitor_Visit(visitor, args[i]);

        switch (visited_ast->type) {
            case AST_STRING: {
                if (strcmp(visited_ast->string_value, "%n") == 0) {
                    printf("\n");
                } else {
                    char *current = visited_ast->string_value;
                    char *next_match = strstr(current, "%n");
                    while (next_match != NULL) {
                        printf("%.*s", (int)(next_match - current), current);
                        printf("\n");
                        current = next_match + 2;
                        next_match = strstr(current, "%n");
                    };
                    printf("%s", current);
                };
                break;
            };
            case AST_BOOL: printf(" %s", visited_ast->bool_value ? "true" : "false"); break;
            case AST_NUMBER : printf("%f ", visited_ast->number_value); break;
            case AST_CLASS : printf("%f ", visited_ast->class_name); break;
            case AST_CLASS_INSTANTIATION :
                printf("{");
                AST_T* class_def = Scope_Get_Class_Definition(visited_ast->scope, visited_ast->instance_class_name);
                for (size_t j = 0; j < class_def->class_size; j++) {
                    AST_T* elem = Visitor_Visit(visitor, class_def->class_definition_value[j]);
                    switch (elem->type) {
                        case AST_STRING : printf(" %s", elem->string_value); break;
                        case AST_NUMBER : printf(" %f", elem->number_value); break;
                        case AST_BOOL: printf(" %s", elem->bool_value ? "true" : "false"); break;
                        case AST_TABLE_DEFINITION : printf(" "); AST_T** new_args = calloc(1, sizeof(struct AST_STRUCT*)); new_args[0] = elem; builtin_function_print(visitor, new_args, 1); break;
                        default : printf(" %p", elem); break;
                    }
                    if (j < (visited_ast->table_size-1)){
                        printf(",");
                    };
                }
                printf("}");
            case AST_TABLE_DEFINITION : {
                printf("[");
                for (size_t j = 0; j < visited_ast->table_size; j++) {
                    AST_T* elem = Visitor_Visit(visitor, visited_ast->table_definition_value[j]);
                    switch (elem->type) {
                        case AST_STRING : printf(" %s", elem->string_value); break;
                        case AST_NUMBER : printf(" %f", elem->number_value); break;
                        case AST_BOOL: printf(" %s", elem->bool_value ? "true" : "false"); break;
                        case AST_TABLE_DEFINITION : printf(" "); AST_T** new_args = calloc(1, sizeof(struct AST_STRUCT*)); new_args[0] = elem; builtin_function_print(visitor, new_args, 1); break;
                        default : printf(" %p", elem); break;
                    }
                    if (j < (visited_ast->table_size-1)){
                        printf(",");
                    };
                }
                printf(" ] ");
                break;
            };
            default: printf("%p ", visited_ast); break;
        }
    };

    return Init_AST(AST_NOOP);
};

AST_T* builtin_function_type(Visitor_T* visitor, AST_T** args, int args_size){
    if (args_size > 1) {
        printf("Tripped on function 'type', argument overflow. (%d to 1)\n", args_size);
        exit(1);
    } else if (args_size < 1) {
        printf("Tripped on function 'type', argument underflow. (1 to %d)\n", args_size);
        exit(1);
    }

    AST_T* newvar = Init_AST(AST_NUMBER);

    if (args[0]->type == AST_TABLE) {
        newvar->type = AST_STRING;
        newvar->string_value = "TABLE";
        return newvar;
    };

    AST_T* var = Visitor_Visit(visitor, args[0]);

    switch (var->type) {
        case AST_STRING:
            newvar->type = AST_STRING;
            newvar->string_value = "STRING";
            return newvar;
            break;
        case AST_NUMBER:
            newvar->type = AST_STRING;
            newvar->string_value = "NUMBER";
            return newvar;
            break;
        case AST_BOOL:
            newvar->type = AST_STRING;
            newvar->string_value = "BOOL";
            return newvar;
            break;
        default:
            printf("Tripped on function 'type', unsupported type %d\n", var->type);
            exit(1);
    };

    return Init_AST(AST_NOOP);
};

AST_T* builtin_function_table_get_index(Visitor_T* visitor, AST_T** args, int args_size){
    if (args_size > 2) {
        printf("Tripped on function 'table_get_index', argument overflow. (%d to 2)\n", args_size);
        exit(1);
    } else if (args_size < 2) {
        printf("Tripped on function 'table_get_index', argument underflow. (2 to %d)\n", args_size);
        exit(1);
    }

    AST_T* table = Visitor_Visit(visitor, args[0]);
    AST_T* index = Visitor_Visit(visitor, args[1]);

    if (index->type != AST_NUMBER) {
        printf("Tripped on function 'table_get_index', type of argument 1 expects a table but did not receive one\n");
        exit(1);
    }
    if ((int)index->number_value <= 0){
        printf("Tripped on function 'table_get_index', index is out of bounds (0)\n");
        exit(1);
    };
    if (table->type != AST_TABLE_DEFINITION) {
        printf("Tripped on function 'table_get_index', type of argument 1 expects a table but did not receive one\n");
        exit(1);
    }
    if ((size_t)index->number_value > table->table_size){
        printf("Tripped on function 'table_get_index', index is out of bounds (1)\n");
        exit(1);
    };

    AST_T* elem = Visitor_Visit(visitor, table->table_definition_value[(int)index->number_value - 1]);
    AST_T* ast_var = Init_AST(AST_STRING);

    switch (elem->type) {
        case AST_STRING: ast_var->scope = table->scope; ast_var->string_value = elem->string_value; return ast_var; break;
        case AST_NUMBER: ast_var->type = AST_NUMBER; ast_var->scope = table->scope; ast_var->number_value = elem->number_value; return ast_var; break;
        case AST_BOOL: ast_var->type = AST_BOOL; ast_var->scope = table->scope; ast_var->bool_value = elem->bool_value; return ast_var; break;
        case AST_TABLE_DEFINITION : {
            ast_var->type = AST_TABLE_DEFINITION;
            ast_var->scope = table->scope;
            ast_var->table_definition_value = elem->table_definition_value;
            return ast_var;
            break;
        }
        default: printf("Tripped on function 'table_get_index', unsupported type %d\n", elem->type); exit(1);
    };

    return Init_AST(AST_NOOP);
};

AST_T* builtin_function_table_set_index(Visitor_T* visitor, AST_T** args, int args_size){
    if (args_size > 3) {
        printf("Tripped on function 'table_set_index', argument overflow. (%d to 3)\n", args_size);
        exit(1);
    } else if (args_size < 3) {
        printf("Tripped on function 'table_set_index', argument underflow. (3 to %d)\n", args_size);
        exit(1);
    }

    AST_T* table = Visitor_Visit(visitor, args[0]);
    AST_T* index = Visitor_Visit(visitor, args[1]);
    AST_T* value = Visitor_Visit(visitor, args[2]);

    if (index== NULL || index->type != AST_NUMBER) {
        printf("Tripped on function 'table_set_index', type of argument 1 expects a table but did not receive one\n");
        exit(1);
    }
    if (value == NULL || value->type == AST_NOOP) {
        printf("Tripped on function 'table_set_index', type of argument 1 expects a table but did not receive one\n");
        exit(1);
    }
    if ((int)index->number_value <= 0){
        printf("Tripped on function 'table_set_index', index is out of bounds (0)\n");
        exit(1);
    };
    if (table->type != AST_TABLE_DEFINITION) {
        printf("Tripped on function 'table_set_index', type of argument 1 expects a table but did not receive one\n");
        exit(1);
    }
    if ((size_t)index->number_value > table->table_size){
        printf("Tripped on function 'table_set_index', index is out of bounds (1)\n");
        exit(1);
    };

    table->table_definition_value[(int)index->number_value - 1] = value;

    return Init_AST(AST_NOOP);
};

AST_T* builtin_function_for_each(Visitor_T* visitor, AST_T** args, int args_size){
    /* if (args_size > 2) {
        printf("Tripped on function 'ForEach', argument overflow. (%d to 2)\n", args_size);
        exit(1);
    }; */
    if (args_size < 2) {
        printf("Tripped on function 'ForEach', argument underflow. (2 to %d)\n", args_size);
        exit(1);
    }

    AST_T* table = Visitor_Visit(visitor, args[0]);
    AST_T* func_name = Visitor_Visit(visitor, args[1]);

    if (table == NULL || table->type != AST_TABLE_DEFINITION) {
        printf("Tripped on function 'ForEach', type of argument 1 expects a table but did not receive one\n");
        exit(1);
    };
    if (func_name == NULL || func_name->type != AST_STRING) {
        printf("Tripped on function 'ForEach', type of argument 2 expects a function but did not receive one\n");
        exit(1);
    };

    AST_T* node = Init_AST(AST_FUNCTION_CALL);
    node->scope = func_name->scope;
    node->function_call_name = func_name->string_value;

    node->function_call_arguments_size = 0;

    for (int in=0;in<table->table_size;in++) {
        AST_T* fdef = Scope_Get_Function_Definition(node->scope, node->function_call_name);

        if (fdef == (void*)0) {
            printf("Tripped on undefined method '%s'\n", node->function_call_name);
            exit(1);
        }
        if (node->function_call_arguments_size != fdef->function_definition_args_size) {
            printf("Tripped on function call '%s', expected %zu args, got %zu\n",
                   node->function_call_name, fdef->function_definition_args_size, node->function_call_arguments_size);
            exit(1);
        }
        Scope_T* call_scope = fdef->function_definition_body->scope;
        size_t saved_scope_size = call_scope->variable_definitions_size;

        for (int i=0;i<(int)node->function_call_arguments_size;i++){
            AST_T* ast_var = (AST_T*) fdef->function_definition_args[i];
            AST_T* ast_value = (AST_T*) node->function_call_arguments[i];
            AST_T* evaluated_value = Visitor_Visit(visitor, ast_value);   // evaluate now, using the caller's current bindings

            AST_T* ast_vardef = Init_AST(AST_VARIABLE_DEFINITION);
            ast_vardef->variable_definition_value = evaluated_value;
            ast_vardef->variable_definition_variable_name = (char*) calloc(strlen(ast_var->variable_name) + 1, sizeof(char));
            strcpy(ast_vardef->variable_definition_variable_name, ast_var->variable_name);
            Scope_Add_Variable_Definition(call_scope, ast_vardef);
        }
        AST_T* result = Visitor_Visit(visitor, fdef->function_definition_body);
        visitor->returning = 0;

        call_scope->variable_definitions_size = saved_scope_size;
    };
    return Init_AST(AST_NOOP);
};

AST_T* builtin_function_if_comp(Visitor_T* visitor, AST_T** args, int args_size){
    if (args_size > 3) {
        printf("Tripped on function 'ifcomp', argument overflow. (%d to 2)\n", args_size);
        exit(1);
    } else if (args_size < 3) {
        printf("Tripped on function 'ifcomp', argument underflow. (2 to %d)\n", args_size);
        exit(1);
    }

    AST_T* comp1 = Visitor_Visit(visitor, args[0]);
    AST_T* comp2 = Visitor_Visit(visitor, args[1]);
    AST_T* comparison_type = Visitor_Visit(visitor, args[2]);

    if (comparison_type->type != AST_NUMBER) {
        printf("Tripped on function 'ifcomp', ineligible type for comparison (%d) - must be a number type between 1 and 4\n", comparison_type->type);
        exit(1);
    };

    if (comp1->type != comp2->type) {
        printf("Tripped on function 'ifcomp', incomparable types - got type '%d' and type '%d' (did you compare a variable and raw number?)\n", comp1->type, comp2->type);
        exit(1);
    }

    AST_T* num = Init_AST(AST_NUMBER);
    num->number_value=0;

    if (comparison_type->number_value == 0) {// is comp1 equal to comp2? (Returns [0-no, 1-yes)
        switch (comp1->type) {
            case AST_STRING:
                if (strcmp(comp1->string_value, comp2->string_value) == 0) {
                    num->number_value=1;
                    return num;
                } else {
                    return num;
                }
                break;
            case AST_NUMBER:
                if (comp1->number_value == comp2->number_value) {
                    num->number_value=1;
                    return num;
                } else {
                    return num;
                }
                break;
            case AST_BOOL:
                if (comp1->bool_value == comp2->bool_value) {
                    num->number_value=1;
                    return num;
                } else {
                    return num;
                }
                break;
            default:
                printf("Tripped on 'ifcomp', unsupported type %d\n", comp1->type);
                exit(1);
        };
    } else if (comparison_type->number_value == 1) {// is comp1 greater than comp2? (Returns [0-no, 1-yes)
        switch (comp1->type) {
            case AST_NUMBER:
                if (comp1->number_value > comp2->number_value) {
                    num->number_value=1;
                    return num;
                } else {
                    return num;
                }
                break;
            default:
                printf("Tripped on 'ifcomp', unsupported type %d\n", comp1->type);
                exit(1);
        };
    } else if (comparison_type->number_value == 2) {// is comp1 less than comp2? (Returns [0-no, 1-yes)
        switch (comp1->type) {
            case AST_NUMBER:
                if (comp1->number_value < comp2->number_value) {
                    num->number_value=1;
                    return num;
                } else {
                    return num;
                }
                break;
            default:
                printf("Tripped on 'ifcomp', unsupported type %d\n", comp1->type);
                exit(1);
        };
    } else if (comparison_type->number_value == 3) {// is comp1 greater than or equal to comp2? (Returns [0-no, 1-yes)
        switch (comp1->type) {
            case AST_NUMBER:
                if (comp1->number_value >= comp2->number_value) {
                    num->number_value=1;
                    return num;
                } else {
                    return num;
                }
                break;
            default:
                printf("Tripped on 'ifcomp', unsupported type %d\n", comp1->type);
                exit(1);
        };
    } else if (comparison_type->number_value == 4) {// is comp1 less than or equal to comp2? (Returns [0-no, 1-yes)
        switch (comp1->type) {
            case AST_NUMBER:
                if (comp1->number_value <= comp2->number_value) {
                    num->number_value=1;
                    return num;
                } else {
                    return num;
                }
                break;
            default:
                printf("Tripped on 'ifcomp', unsupported type %d\n", comp1->type);
                exit(1);
        };
    } else if (comparison_type->number_value == 5) {// is comp1 not equal to comp2? (Returns [0-no, 1-yes)
        switch (comp1->type) {
            case AST_STRING:
                if (strcmp(comp1->string_value, comp2->string_value) != 0) {
                    num->number_value=1;
                    return num;
                } else {
                    return num;
                }
                break;
            case AST_NUMBER:
                if (comp1->number_value != comp2->number_value) {
                    num->number_value=1;
                    return num;
                } else {
                    return num;
                }
                break;
            case AST_BOOL:
                if (comp1->bool_value != comp2->bool_value) {
                    num->number_value=1;
                    return num;
                } else {
                    return num;
                }
                break;
            default:
                printf("Tripped on 'ifcomp', unsupported type %d\n", comp1->type);
                exit(1);
        };
    }

    return Init_AST(AST_NOOP);
};
