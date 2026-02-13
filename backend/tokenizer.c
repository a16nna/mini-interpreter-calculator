backend/tokenizer.c
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "tokenizer.h"

TokenArray tokenize(const char* input) {

    TokenArray tokenArray;
    tokenArray.count = 0;

    int i = 0;

    while (input[i] != '\0') {

        if (isspace(input[i])) {
            i++;
            continue;
        }

        // IDENTIFIER (functions, variables, pi)
        if (isalpha(input[i])) {

            int start = i;

            while (isalnum(input[i])) {
                i++;
            }

            int length = i - start;

            strncpy(tokenArray.tokens[tokenArray.count].name,
                    &input[start], length);

            tokenArray.tokens[tokenArray.count].name[length] = '\0';
            tokenArray.tokens[tokenArray.count].type = TOKEN_IDENTIFIER;
            tokenArray.tokens[tokenArray.count].value = 0;
            tokenArray.count++;

            continue;
        }

        // NUMBER (integer or decimal)
        if (isdigit(input[i]) || input[i] == '.') {

            double value = 0;
            double decimal_place = 0.1;
            int is_decimal = 0;

            while (isdigit(input[i]) || input[i] == '.') {

                if (input[i] == '.') {
                    is_decimal = 1;
                    i++;
                    continue;
                }

                if (!is_decimal)
                    value = value * 10 + (input[i] - '0');
                else {
                    value += (input[i] - '0') * decimal_place;
                    decimal_place *= 0.1;
                }

                i++;
            }

            tokenArray.tokens[tokenArray.count].type = TOKEN_NUMBER;
            tokenArray.tokens[tokenArray.count].value = value;
            tokenArray.count++;

            continue;
        }

        TokenType type;

        switch (input[i]) {
            case '+': type = TOKEN_PLUS; break;
            case '-': type = TOKEN_MINUS; break;
            case '*': type = TOKEN_MULTIPLY; break;
            case '/': type = TOKEN_DIVIDE; break;
            case '^': type = TOKEN_POWER; break;
            case '%': type = TOKEN_MOD; break;
            case '(': type = TOKEN_LPAREN; break;
            case ')': type = TOKEN_RPAREN; break;
            case '=': type = TOKEN_ASSIGN; break;
            default:
                printf("Unknown character: %c\n", input[i]);
                i++;
                continue;
        }

        tokenArray.tokens[tokenArray.count].type = type;
        tokenArray.tokens[tokenArray.count].value = 0;
        tokenArray.count++;

        i++;
    }

    tokenArray.tokens[tokenArray.count].type = TOKEN_END;
    tokenArray.count++;

    return tokenArray;
}
