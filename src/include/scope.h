#pragma once

#include "AST.h"

typedef struct SCOPE_STRUCT {
    AST_T** variable_definitions;
    size_t variable_definitions_size;
    AST_T** table_definitions;
    size_t table_definitions_size;
    AST_T** dictionary_definitions;
    size_t dictionary_definitions_size;
    AST_T** class_definitions;
    size_t class_definitions_size;
    AST_T** function_definitions;
    size_t function_definitions_size;
} Scope_T;

Scope_T* Init_Scope();
AST_T* Scope_Add_Function_Definition(Scope_T* Scope, AST_T* Fdef);
AST_T* Scope_Get_Function_Definition(Scope_T* Scope, const char* Fname);
AST_T* Scope_Add_Table_Definition(Scope_T* Scope, AST_T* Tdef);
AST_T* Scope_Get_Table_Definition(Scope_T* Scope, const char* Tname);
AST_T* Scope_Add_Dictionary_Definition(Scope_T* Scope, AST_T* Ddef);
AST_T* Scope_Get_Dictionary_Definition(Scope_T* Scope, const char* Dname);
AST_T* Scope_Add_Class_Definition(Scope_T* Scope, AST_T* Cdef);
AST_T* Scope_Get_Class_Definition(Scope_T* Scope, const char* Cname);
AST_T* Scope_Add_Variable_Definition(Scope_T* Scope, AST_T* Vdef);
AST_T* Scope_Get_Variable_Definition(Scope_T* Scope, const char* Vname);
