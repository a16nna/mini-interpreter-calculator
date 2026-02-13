backend/parser.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "parser.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static int degree_mode = 0; // 0 = radians, 1 = degrees

void set_degree_mode(int mode) {
    degree_mode = mode;
}

static TokenArray tokens;
static int current = 0;

static Token current_token() {
    return tokens.tokens[current];
}

static void advance() {
    current++;
}

static ASTNode* create_node(TokenType type, double value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->name[0] = '\0';
    return node;
}

/* ========================= */

static ASTNode* parse_expression();
static ASTNode* parse_term();
static ASTNode* parse_power();
static ASTNode* parse_factor();

ASTNode* parse(TokenArray t) {
    tokens = t;
    current = 0;
    return parse_expression();
}

/* ========================= */

static ASTNode* parse_expression() {

    ASTNode* node = parse_term();

    while (current_token().type == TOKEN_PLUS ||
           current_token().type == TOKEN_MINUS) {

        TokenType op = current_token().type;
        advance();

        ASTNode* right = parse_term();

        ASTNode* parent = create_node(op, 0);
        parent->left = node;
        parent->right = right;

        node = parent;
    }

    return node;
}

static ASTNode* parse_term() {

    ASTNode* node = parse_power();

    while (current_token().type == TOKEN_MULTIPLY ||
           current_token().type == TOKEN_DIVIDE ||
           current_token().type == TOKEN_MOD) {

        TokenType op = current_token().type;
        advance();

        ASTNode* right = parse_power();

        ASTNode* parent = create_node(op, 0);
        parent->left = node;
        parent->right = right;

        node = parent;
    }

    return node;
}

static ASTNode* parse_power() {

    ASTNode* node = parse_factor();

    if (current_token().type == TOKEN_POWER) {

        advance();

        ASTNode* right = parse_power();  // recursive call for right-associativity

        ASTNode* parent = create_node(TOKEN_POWER, 0);
        parent->left = node;
        parent->right = right;

        return parent;
    }

    return node;
}

static ASTNode* parse_factor() {

    Token token = current_token();

    // Unary minus
    if (token.type == TOKEN_MINUS) {
        advance();
        ASTNode* node = create_node(TOKEN_MINUS, 0);
        node->left = create_node(TOKEN_NUMBER, 0);
        node->right = parse_factor();
        return node;
    }

    // Number
    if (token.type == TOKEN_NUMBER) {
        advance();
        return create_node(TOKEN_NUMBER, token.value);
    }

    // Identifier (functions or pi)
    if (token.type == TOKEN_IDENTIFIER) {

        char name[32];
        strcpy(name, token.name);
        advance();

        // Constant pi
        if (strcmp(name, "pi") == 0) {
            return create_node(TOKEN_NUMBER, M_PI);
        }

        // Function call
        if (current_token().type == TOKEN_LPAREN) {
            advance();

            ASTNode* arg = parse_expression();

            if (current_token().type == TOKEN_RPAREN)
                advance();

            ASTNode* node = create_node(TOKEN_IDENTIFIER, 0);
            strcpy(node->name, name);
            node->left = arg;

            return node;
        }
    }

    // Parentheses
    if (token.type == TOKEN_LPAREN) {
        advance();
        ASTNode* node = parse_expression();

        if (current_token().type == TOKEN_RPAREN)
            advance();

        return node;
    }

    printf("Unexpected token!\n");
    return NULL;
}

/* ========================= */

double evaluate(ASTNode* node) {

    if (!node) return 0;

    if (node->type == TOKEN_NUMBER)
        return node->value;

    if (node->type == TOKEN_IDENTIFIER) {

        double arg = evaluate(node->left);

        if (degree_mode)
            arg = arg * M_PI / 180.0;

        if (strcmp(node->name, "sin") == 0)
            return sin(arg);
        else if (strcmp(node->name, "cos") == 0)
            return cos(arg);
        else if (strcmp(node->name, "tan") == 0)
            return tan(arg);
        else if (strcmp(node->name, "log") == 0)
            return log(arg);
        else if (strcmp(node->name, "sqrt") == 0)
            return sqrt(arg);

        printf("Unknown function: %s\n", node->name);
        return 0;
    }

    double left = evaluate(node->left);
    double right = evaluate(node->right);

    switch (node->type) {
        case TOKEN_PLUS: return left + right;
        case TOKEN_MINUS: return left - right;
        case TOKEN_MULTIPLY: return left * right;
        case TOKEN_DIVIDE:
            if (right == 0) {
                printf("Division by zero!\n");
                return 0;
            }
            return left / right;
        case TOKEN_POWER:
            return pow(left, right);
        case TOKEN_MOD:
            if ((int)right == 0) {
                printf("Modulo by zero!\n");
                return 0;
            }
            return (int)left % (int)right;
        default:
            return 0;
    }
}

/* ========================= */

void free_ast(ASTNode* node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    free(node);
}
