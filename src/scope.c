#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "include/scope.h"

Scope_T* Init_Scope(){
    Scope_T* Scope = calloc(1, sizeof(struct SCOPE_STRUCT));
    Scope->function_definitions = (void*)0;
    Scope->function_definitions_size = 0;
    Scope->table_definitions = (void*)0;
    Scope->table_definitions_size = 0;
    Scope->dictionary_definitions = (void*)0;
    Scope->dictionary_definitions_size = 0;
    Scope->class_definitions = (void*)0;
    Scope->class_definitions_size = 0;
    Scope->variable_definitions = (void*)0;
    Scope->variable_definitions_size = 0;
    return Scope;
};
AST_T* Scope_Add_Function_Definition(Scope_T* Scope, AST_T* Fdef){
    Scope->function_definitions_size +=1;

    if (Scope->function_definitions ==  (void*)0) {
        Scope->function_definitions = calloc(1, sizeof(struct AST_STRUCT*));
    } else {
        Scope->function_definitions = realloc(
            Scope->function_definitions,
            Scope->function_definitions_size * sizeof(struct AST_STRUCT*)
        );
    }
    Scope->function_definitions[Scope->function_definitions_size-1] = Fdef;
    return Fdef;
};

AST_T* Scope_Get_Function_Definition(Scope_T* Scope, const char* Fname){
    for (int i=0;i<Scope->function_definitions_size;i++){
        AST_T* Fdef = Scope->function_definitions[i];
        if (strcmp(Fdef->function_definition_name, Fname) == 0) {
            return Fdef;
        }
    }
    return (void*)0;
};

AST_T* Scope_Add_Table_Definition(Scope_T* Scope, AST_T* Tdef){

    if (Scope->table_definitions == (void*)0) {
        Scope->table_definitions = calloc(1, sizeof(struct AST_STRUCT*));
        Scope->table_definitions[0] = Tdef;
        Scope->table_definitions_size +=1;
    } else {
        Scope->table_definitions_size +=1;
        Scope->table_definitions = realloc(
            Scope->table_definitions,
            Scope->table_definitions_size * sizeof(struct AST_STRUCT*)
        );
    }
    Scope->table_definitions[Scope->table_definitions_size - 1] = Tdef;

    return Tdef;
};
AST_T* Scope_Get_Table_Definition(Scope_T* Scope, const char* Tname){
    for (int i = (int)Scope->table_definitions_size - 1; i >= 0; i--){
        AST_T* tdef = Scope->table_definitions[i];
        if (strcmp(tdef->table_definition_name, Tname) == 0) {
            return tdef;
        }
    }
    return (void*)0;
};

AST_T* Scope_Add_Dictionary_Definition(Scope_T* Scope, AST_T* Ddef){

    if (Scope->dictionary_definitions == (void*)0) {
        Scope->dictionary_definitions = calloc(1, sizeof(struct AST_STRUCT*));
        Scope->dictionary_definitions[0] = Ddef;
        Scope->dictionary_definitions_size +=1;
    } else {
        Scope->dictionary_definitions_size +=1;
        Scope->dictionary_definitions = realloc(
            Scope->dictionary_definitions,
            Scope->dictionary_definitions_size * sizeof(struct AST_STRUCT*)
        );
    }
    Scope->dictionary_definitions[Scope->dictionary_definitions_size - 1] = Ddef;

    return Ddef;
};
AST_T* Scope_Get_Dictionary_Definition(Scope_T* Scope, const char* Dname){
    for (int i = (int)Scope->dictionary_definitions_size - 1; i >= 0; i--){
        AST_T* ddef = Scope->dictionary_definitions[i];
        if (strcmp(ddef->dictionary_definition_name, Dname) == 0) {
            return ddef;
        }
    }
    return (void*)0;
};

AST_T* Scope_Add_Class_Definition(Scope_T* Scope, AST_T* Cdef){

    if (Scope->class_definitions == (void*)0) {
        Scope->class_definitions = calloc(1, sizeof(struct AST_STRUCT*));
        Scope->class_definitions[0] = Cdef;
        Scope->class_definitions_size += 1;
    } else {
        Scope->class_definitions_size += 1;
        Scope->class_definitions = realloc(
            Scope->class_definitions,
            Scope->class_definitions_size * sizeof(struct AST_STRUCT*)
        );
    }
    Scope->class_definitions[Scope->class_definitions_size - 1] = Cdef;

    return Cdef;
};
AST_T* Scope_Get_Class_Definition(Scope_T* Scope, const char* Cname){
    for (int i = (int)Scope->class_definitions_size - 1; i >= 0; i--){
        AST_T* cdef = Scope->class_definitions[i];
        if (strcmp(cdef->class_definition_name, Cname) == 0) {
            return cdef;
        }
    }
    return (void*)0;
};

AST_T* Scope_Add_Variable_Definition(Scope_T* Scope, AST_T* Vdef){

    if (Scope->variable_definitions == (void*)0) {
        Scope->variable_definitions = calloc(1, sizeof(struct AST_STRUCT*));
        Scope->variable_definitions[0] = Vdef;
        Scope->variable_definitions_size +=1;
    } else {
        Scope->variable_definitions_size +=1;
        Scope->variable_definitions = realloc(
            Scope->variable_definitions,
            Scope->variable_definitions_size * sizeof(struct AST_STRUCT*)
        );
    }
    Scope->variable_definitions[Scope->variable_definitions_size - 1] = Vdef;

    return Vdef;
};
AST_T* Scope_Get_Variable_Definition(Scope_T* Scope, const char* Vname){
    for (int i = (int)Scope->variable_definitions_size - 1; i >= 0; i--){
        AST_T* vdef = Scope->variable_definitions[i];
        if (strcmp(vdef->variable_definition_variable_name, Vname) == 0) {
            return vdef;
        }
    }
    return (void*)0;
};
