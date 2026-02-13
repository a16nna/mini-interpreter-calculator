backend/tokenizer.h
#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_POWER,
    TOKEN_MOD,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_ASSIGN,
    TOKEN_IDENTIFIER,
    TOKEN_END
} TokenType;


typedef struct {
    TokenType type;
    double value;
    char name[32];
} Token;

typedef struct {
    Token tokens[100];
    int count;
} TokenArray;

TokenArray tokenize(const char* input);

#endif
