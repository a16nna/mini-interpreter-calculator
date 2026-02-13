backend/main.c
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include "parser.h"

int main(int argc, char* argv[]) {

    // Single expression mode
    if (argc > 1) {
        TokenArray tokens = tokenize(argv[1]);
        ASTNode* root = parse(tokens);

        if (root != NULL) {
            double result = evaluate(root);
            printf("%g\n", result);
            free_ast(root);
        }
        return 0;
    }

    // REPL mode
    char input[256];

    while (1) {
        printf(">> ");

        if (!fgets(input, sizeof(input), stdin))
            break;

        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0)
            continue;

        if (strcmp(input, "exit") == 0)
            break;

        TokenArray tokens = tokenize(input);
        ASTNode* root = parse(tokens);

        if (root != NULL) {
            double result = evaluate(root);
            printf("%g\n", result);
            free_ast(root);
        }
    }

    return 0;
}
