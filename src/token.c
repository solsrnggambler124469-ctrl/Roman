#include <stdlib.h>
#include "include/token.h"

Token_T* Init_Token(int type, char* value){
    Token_T* token = calloc(1, sizeof(struct TOKEN_STRUCT));
    token->type = type;
    token->value = value;
    return token;
};
