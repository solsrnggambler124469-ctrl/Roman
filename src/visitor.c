#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "include/scope.h"
#include "include/visitor.h"
#include "include/builtin.h"
#include "include/token.h"

For_Tuple* get_for_data(Visitor_T* visitor, AST_T* node) {
    For_Tuple* Tuple = Init_For_Tuple();
    Tuple->condition_result = Visitor_Visit(visitor, node->for_condition);
    Tuple->for_var = Scope_Get_Variable_Definition(node->scope, node->for_variable->variable_name);
    Tuple->for_var2 = Visitor_Visit(visitor, node->for_variable);
    Tuple->for_end_do = Visitor_Visit(visitor, node->for_does_at_end);
    return Tuple;
};

While_Tuple* get_while_data(Visitor_T* visitor, AST_T* node) {
    While_Tuple* Tuple = Init_While_Tuple();
    Tuple->condition_result = Visitor_Visit(visitor, node->while_condition);
    return Tuple;
};

Visitor_T* Init_Visitor(){
    Visitor_T* visitor = calloc(1, sizeof(struct VISITOR_STRUCT));
    visitor->returning = 0;
    return visitor;
};

For_Tuple* Init_For_Tuple(){
    For_Tuple* tuple = calloc(1, sizeof(struct FOR_TUPLE_STRUCT));
    return tuple;
};

While_Tuple* Init_While_Tuple(){
    While_Tuple* tuple = calloc(1, sizeof(struct WHILE_TUPLE_STRUCT));
    return tuple;
};

AST_T* Visitor_Visit(Visitor_T* visitor, AST_T* node){
    if (node == (void*)0) {
        return Init_AST(AST_NOOP);
    }
    switch (node->type) {
        case AST_VARIABLE_DEFINITION: return VV_Variable_Definition(visitor, node); break;
        case AST_TABLE_DEFINITION: return VV_Table_Definition(visitor, node); break;
        case AST_DICTIONARY_DEFINITION: return VV_Dict_Definition(visitor, node); break;
        case AST_CLASS_DEFINITION: return VV_Class_Definition(visitor, node); break;
        case AST_FUNCTION_DEFINITION: return VV_Function_Definition(visitor, node); break;
        case AST_IF: return VV_If(visitor, node); break;
        case AST_IF_ELSE: return VV_If_Else(visitor, node); break;
        case AST_FOR: return VV_For(visitor, node); break;
        case AST_WHILE: return VV_While(visitor, node); break;
        case AST_BINOP: return VV_BinOp(visitor, node); break;
        case AST_ARROW: return VV_Arrow(visitor, node); break;
        case AST_RETURN: return VV_Return(visitor, node); break;
        case AST_VARIABLE: return VV_Variable(visitor, node); break;
        case AST_STRING: return VV_String(visitor, node); break;
        case AST_NUMBER: return VV_Number(visitor, node); break;
        case AST_BOOL: return VV_Bool(visitor, node); break;
        case AST_TABLE: return VV_Table(visitor, node); break;
        case AST_DICTIONARY: return VV_Dict(visitor, node); break;
        case AST_CLASS: return VV_Class(visitor, node); break;
        case AST_FUNCTION_CALL: return VV_Function_Call(visitor, node); break;
        case AST_COMPOUND: return VV_Compound(visitor, node); break;
        case AST_NOOP: return node; break;
    };

    printf("Tripped on statement, enum type is '%d'\n", node->type);
    exit(1);
    return Init_AST(AST_NOOP);
};
AST_T* VV_Variable_Definition(Visitor_T* visitor, AST_T* node) {
    AST_T* evaluated_value = Visitor_Visit(visitor, node->variable_definition_value);

    AST_T* vdef = Init_AST(AST_VARIABLE_DEFINITION);
    vdef->variable_definition_variable_name = node->variable_definition_variable_name;
    vdef->variable_definition_value = evaluated_value;
    vdef->scope = node->scope;

    Scope_Add_Variable_Definition(node->scope, vdef);
    return node;
};
AST_T* VV_Variable(Visitor_T* visitor, AST_T* node) {
    if (strcmp(node->variable_name, "true") == 0) {
        AST_T* vdef = Init_AST(AST_BOOL);
        vdef->bool_value = true;
        return Visitor_Visit(visitor, vdef);
    } else if (strcmp(node->variable_name, "false") == 0) {
        AST_T* vdef = Init_AST(AST_BOOL);
        vdef->bool_value = false;
        return Visitor_Visit(visitor, vdef);
    } else {
        AST_T* vdef = Scope_Get_Variable_Definition(node->scope, node->variable_name);

        if (vdef != (void*)0) {
            return Visitor_Visit(visitor, vdef->variable_definition_value);
        } else {
            printf("Tripped on undefined variable '%s'\n", node->variable_name);
            exit(1);
        }
    }
    return node;
};
AST_T* VV_If(Visitor_T* visitor, AST_T* node) {
    AST_T* condition_result = Visitor_Visit(visitor, node->if_condition);

    int is_truthy = 0;
    switch (condition_result->type) {
        case AST_NUMBER: is_truthy = (condition_result->number_value != 0.0f); break;
        case AST_STRING:  is_truthy = (condition_result->string_value != (void*)0 &&
                                        strlen(condition_result->string_value) > 0); break;
        case AST_BOOL:  is_truthy = (condition_result->bool_value != false); break;
        default: is_truthy = 0; break;
    }

    if (is_truthy) {
        return Visitor_Visit(visitor, node->if_body);
    }
    return Init_AST(AST_NOOP);
};
AST_T* VV_If_Else(Visitor_T* visitor, AST_T* node) {
    AST_T* condition_result = Visitor_Visit(visitor, node->if_condition);

    int is_truthy = 0;
    switch (condition_result->type) {
        case AST_NUMBER: is_truthy = (condition_result->number_value != 0.0f); break;
        case AST_STRING:  is_truthy = (condition_result->string_value != (void*)0 &&
                                        strlen(condition_result->string_value) > 0); break;
        case AST_BOOL:  is_truthy = (condition_result->bool_value != false); break;
        default: is_truthy = 0; break;
    }

    if (is_truthy) {
        return Visitor_Visit(visitor, node->if_body);
    };
    return Visitor_Visit(visitor, node->if_else_body);
};
AST_T* VV_For(Visitor_T* visitor, AST_T* node) {
    AST_T* init_result = Visitor_Visit(visitor, node->for_variable);
    if (init_result->type != AST_VARIABLE_DEFINITION) {
        printf("Tripped on for loop, loop variable must be a 'var' definition\n");
        exit(1);
    }
    char* loop_var_name = init_result->variable_definition_variable_name;

    AST_T* end_do = Visitor_Visit(visitor, node->for_does_at_end);
    if (end_do->type != AST_STRING) {
        printf("Tripped on for loop, unexpected argument types received\n");
        exit(1);
    }

    bool minus;
    if (strcmp(end_do->string_value, "++") == 0) {
        minus = false;
    } else if (strcmp(end_do->string_value, "--") == 0) {
        minus = true;
    } else {
        printf("Tripped on for loop, unknown step operator '%s'\n", end_do->string_value);
        exit(1);
    }

    bool condition_true() {
        AST_T* cond = Visitor_Visit(visitor, node->for_condition);
        switch (cond->type) {
            case AST_NUMBER: return cond->number_value != 0.0f;
            case AST_STRING:  return cond->string_value != (void*)0 && strlen(cond->string_value) > 0;
            case AST_BOOL:    return cond->bool_value != false;
            default: return false;
        }
    };

    void set_loop_var(float value) {
        AST_T* vdef = Init_AST(AST_VARIABLE_DEFINITION);
        vdef->variable_definition_variable_name = loop_var_name;
        AST_T* num = Init_AST(AST_NUMBER);
        num->number_value = value;
        vdef->variable_definition_value = num;
        vdef->scope = node->scope;
        Scope_Add_Variable_Definition(node->scope, vdef);
    };

    AST_T* current = Scope_Get_Variable_Definition(node->scope, loop_var_name);
    float counter = current->variable_definition_value->number_value;

    AST_T* last_var = Init_AST(AST_NOOP);
    while (condition_true()) {
        last_var = Visitor_Visit(visitor, node->for_body);
        if (visitor->returning) {
            break; // a 'return' fired inside the loop body -- stop iterating
        }
        counter += minus ? -1.0f : 1.0f;
        set_loop_var(counter);
    };

    return last_var;
};
AST_T* VV_While(Visitor_T* visitor, AST_T* node) {
    While_Tuple* ttt = get_while_data(visitor, node);

    bool get_res() {
        ttt->condition_result = Visitor_Visit(visitor, node->while_condition);
        bool is_truthy = false;
        switch (ttt->condition_result->type) {
            case AST_NUMBER: is_truthy = (ttt->condition_result->number_value != 0.0f); break;
            case AST_STRING:  is_truthy = (ttt->condition_result->string_value != (void*)0 &&
                                            strlen(ttt->condition_result->string_value) > 0); break;
            case AST_BOOL:  is_truthy = (ttt->condition_result->bool_value != false); break;
            default: is_truthy = false; break;
        };
        return is_truthy;
    };
    AST_T* last_var = Init_AST(AST_NOOP);

    while (get_res() == true) {
        last_var = Visitor_Visit(visitor, node->while_body);
        if (visitor->returning) {
            break; // a 'return' fired inside the loop body -- stop iterating
        }
    };
    return last_var;
};
AST_T* VV_BinOp(Visitor_T* visitor, AST_T* node) {
    AST_T* left = Visitor_Visit(visitor, node->binop_left);
    AST_T* right = Visitor_Visit(visitor, node->binop_right);

    if (left->type == AST_NUMBER && right->type == AST_NUMBER) {
        AST_T* result = Init_AST(AST_NUMBER);
        switch (node->binop_op) {
            case TOKEN_PLUS:  result->number_value = left->number_value + right->number_value; break;
            case TOKEN_MINUS: result->number_value = left->number_value - right->number_value; break;
            case TOKEN_MULTIPLY:   result->number_value = left->number_value * right->number_value; break;
            case TOKEN_DIVIDE:
                if (right->number_value == 0.0f) {
                    printf("Tripped on binary operation, division by zero\n");
                    exit(1);
                }
                result->number_value = left->number_value / right->number_value;
                break;
            default:
                printf("Tripped on binary operation, unsupported operator %d\n", node->binop_op);
                exit(1);
        }
        return result;
    } else if (left->type == AST_STRING && right->type == AST_STRING && node->binop_op == TOKEN_PLUS) {
        AST_T* result = Init_AST(AST_STRING);
        char* combined = calloc(strlen(left->string_value) + strlen(right->string_value) + 1, sizeof(char));
        strcpy(combined, left->string_value);
        strcat(combined, right->string_value);
        result->string_value = combined;
        return result;
    } else {
        printf("Tripped on binary operation, incompatible operand types (%d, %d)\n", left->type, right->type);
        exit(1);
    }

    return Init_AST(AST_NOOP);
};
AST_T* VV_Arrow(Visitor_T* visitor, AST_T* node) {
    AST_T* left = Visitor_Visit(visitor, node->arrow_left);
    AST_T* right = Visitor_Visit(visitor, node->arrow_right);

    if (right->type != AST_STRING) {
        printf("Tripped on arrow operation, key must be a string (got type %d)\n", right->type);
        exit(1);
    }

    if (left->type == AST_DICTIONARY_DEFINITION) {
        for (size_t i = 0; i < left->dictionary_size; i++) {
            AST_T* key = Visitor_Visit(visitor, left->dictionary_definition_value_name[i]);
            if (key->type == AST_STRING && strcmp(key->string_value, right->string_value) == 0) {
                return Visitor_Visit(visitor, left->dictionary_definition_value[i]);
            }
        }
        printf("Tripped on arrow operation, key '%s' not found in dictionary\n", right->string_value);
        exit(1);
    } else if (left->type == AST_CLASS_DEFINITION) {
        if (strcmp(right->string_value, "init") == 0) {
            if (left->init_step == 0) {
                printf("Tripped on arrow operation, class '%s' has no init defined\n", left->class_definition_name);
                exit(1);
            }
            return Visitor_Visit(visitor, left->init_value);
        }
        for (size_t i = 0; i < left->class_size; i++) {
            AST_T* key = Visitor_Visit(visitor, left->class_definition_value_name[i]);
            if (key->type == AST_STRING && strcmp(key->string_value, right->string_value) == 0) {
                return Visitor_Visit(visitor, left->class_definition_value[i]);
            }
        }
        printf("Tripped on arrow operation, key '%s' not found in class\n", right->string_value);
        exit(1);
    } else {
        printf("Tripped on arrow operation, invalid type accessed (%d)\n", left->type);
        exit(1);
    }

    return Init_AST(AST_NOOP);
};
AST_T* VV_Return(Visitor_T* visitor, AST_T* node) {
    AST_T* result = Visitor_Visit(visitor, node->return_value);
    visitor->returning = 1;
    return result;
};

AST_T* VV_Function_Definition(Visitor_T* visitor, AST_T* node) {
    Scope_Add_Function_Definition(node->scope, node);
    return node;
};
AST_T* VV_Function_Call(Visitor_T* visitor, AST_T* node) {
    if (strcmp(node->function_call_name, "print") == 0) { // sees if func name is print
        return builtin_function_print(visitor, node->function_call_arguments, node->function_call_arguments_size);
    } else if (strcmp(node->function_call_name, "type") == 0) {
        return builtin_function_type(visitor, node->function_call_arguments, node->function_call_arguments_size);
    } else if (strcmp(node->function_call_name, "table_get_index") == 0) {
        return builtin_function_table_get_index(visitor, node->function_call_arguments, node->function_call_arguments_size);
    } else if (strcmp(node->function_call_name, "ifcomp") == 0) {
        return builtin_function_if_comp(visitor, node->function_call_arguments, node->function_call_arguments_size);
    } else {
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

        return result;
    }
};
AST_T* VV_Table_Definition(Visitor_T* visitor, AST_T* node) {
    Scope_Add_Table_Definition(node->scope, node);
    return node;
};
AST_T* VV_Table(Visitor_T* visitor, AST_T* node) {
    AST_T* tdef = Scope_Get_Table_Definition(node->scope, node->table_name);

    if (tdef != (void*)0) {
        return tdef;
    } else {
        printf("Tripped on undefined table '%s'\n", node->table_name);
        exit(1);
    }
    return node;
};
AST_T* VV_Dict_Definition(Visitor_T* visitor, AST_T* node) {
    Scope_Add_Dictionary_Definition(node->scope, node);
    return node;
};
AST_T* VV_Dict(Visitor_T* visitor, AST_T* node) {
    AST_T* ddef = Scope_Get_Dictionary_Definition(node->scope, node->dictionary_name);

    if (ddef != (void*)0) {
        return ddef;
    } else {
        printf("Tripped on undefined dictionary '%s'\n", node->dictionary_name);
        exit(1);
    }
    return node;
};
AST_T* VV_Class_Definition(Visitor_T* visitor, AST_T* node) {
    Scope_Add_Class_Definition(node->scope, node);
    return node;
};
AST_T* VV_Class(Visitor_T* visitor, AST_T* node) {
    AST_T* tdef = Scope_Get_Class_Definition(node->scope, node->class_name);

    if (tdef != (void*)0) {
        return tdef;
    } else {
        printf("Tripped on undefined class '%s'\n", node->class_name);
        exit(1);
    }
    return node;
};

AST_T* VV_String(Visitor_T* visitor, AST_T* node) {
    return node;
};
AST_T* VV_Number(Visitor_T* visitor, AST_T* node) {
    return node;
};
AST_T* VV_Bool(Visitor_T* visitor, AST_T* node) {
    return node;
};
AST_T* VV_Compound(Visitor_T* visitor, AST_T* node) {
    AST_T* result = Init_AST(AST_NOOP);
    for (int i=0;i<node->compound_size;i++) {
        result = Visitor_Visit(visitor, node->compound_value[i]);
        if (visitor->returning) {
            return result;   // a return fired somewhere in this statement (directly or nested in if/ifelse) - stop
        }
    };
    return Init_AST(AST_NOOP);
};


void Visitor_Clean(Visitor_T* visitor, AST_T* root){
    free(visitor);
    free(root);
}
