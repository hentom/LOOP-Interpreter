/*
 * token.h
 *
 * Tom René Hennig
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

typedef enum {
    TOK_VAR_ID,
    TOK_NAT_NUM,
    TOK_ASS,
    TOK_PLUS,
    TOK_MINUS,
    TOK_SEM,
    TOK_LOOP,
    TOK_DO,
    TOK_END,
    TOK_EOF,
    TOK_INVALID
} TokenType;

typedef struct {
    TokenType type;
    long value;
} Token;


Token nextToken(FILE *stream);
void pushToken(Token tok);

void printToken(FILE* stream, Token tok);

#endif /* TOKEN_H */
