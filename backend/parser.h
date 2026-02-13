backend/parser.h
#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"

typedef struct ASTNode {
    TokenType type;
    double value;
    char name[32];

    struct ASTNode* left;
    struct ASTNode* right;

} ASTNode;

ASTNode* parse(TokenArray tokens);
double evaluate(ASTNode* node);
void free_ast(ASTNode* node);

#endif
