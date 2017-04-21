/*
 * token.c
 *
 * Tom René Hennig
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "token.h"


static Token buf;
static bool bufUsed = false;


Token nextToken(FILE *stream)
{
    if (bufUsed) {
        bufUsed = false;
        return buf;
    }
    
    // skip leading blanks
    int c = fgetc(stream);
    while (isspace(c) && c != EOF)
        c = fgetc(stream);
    
    Token tok;
    switch (c) {
    case 'x': // variable identifier, read number
        tok.type = TOK_VAR_ID;
        tok.value = 0;
        while (isdigit(c = fgetc(stream))) {
            tok.value *= 10;
            tok.value += c - '0';
        }
        ungetc(c, stream);
        break;
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        tok.type = TOK_NAT_NUM;
        tok.value = c - '0';
        while (isdigit(c = fgetc(stream))) {
            tok.value *= 10;
            tok.value += c - '0';
        }
        ungetc(c, stream);
        break;
    case ':':
        if ((c = fgetc(stream)) != '=') {
            tok.type = TOK_INVALID;
            tok.value = c;
            break;
        }
        tok.type = TOK_ASS;
        break;
    case '+':
        tok.type = TOK_PLUS;
        break;
    case '-':
        tok.type = TOK_MINUS;
        break;
    case ';':
        tok.type = TOK_SEM;
        break;
    case 'L':
        if ((c = fgetc(stream)) != 'O') {
            tok.type = TOK_INVALID;
            tok.value = c;
            break;
        }
        if ((c = fgetc(stream)) != 'O') {
            tok.type = TOK_INVALID;
            tok.value = c;
            break;
        }
        if ((c = fgetc(stream)) != 'P') {
            tok.type = TOK_INVALID;
            tok.value = c;
            break;
        }
        tok.type = TOK_LOOP;
        break;
    case 'D':
        if ((c = fgetc(stream)) != 'O') {
            tok.type = TOK_INVALID;
            tok.value = c;
            break;
        }
        tok.type = TOK_DO;
        break;
    case 'E':
        if ((c = fgetc(stream)) != 'N') {
            tok.type = TOK_INVALID;
            tok.value = c;
            break;
        }
        if ((c = fgetc(stream)) != 'D') {
            tok.type = TOK_INVALID;
            tok.value = c;
            break;
        }
        tok.type = TOK_END;
        break;
    case EOF:
        tok.type = TOK_EOF;
        break;
    default:
        tok.type = TOK_INVALID;
        tok.value = c;
        break;
    }
    return tok;
}

void pushToken(Token tok)
{
    if (bufUsed) {
        fprintf(stderr, "ERROR: cannot push back more than one token\n");
        exit(EXIT_FAILURE);
    }
    buf = tok;
    bufUsed = true;
}

void printToken(FILE* stream, Token tok)
{
    switch (tok.type) {
    case TOK_VAR_ID:
        fprintf(stream, "variable identifier x%ld", tok.value);
        break;
    case TOK_NAT_NUM:
        fprintf(stream, "number %ld", tok.value);
        break;
    case TOK_ASS:
        fprintf(stream, "assignment operator \':=\'");
        break;
    case TOK_PLUS:
        fprintf(stream, "plus operator");
        break;
    case TOK_MINUS:
        fprintf(stream, "minus operator");
        break;
    case TOK_SEM:
        fprintf(stream, "semicolon");
        break;
    case TOK_LOOP:
        fprintf(stream, "\'LOOP\' keyword");
        break;
    case TOK_DO:
        fprintf(stream, "\'DO\' keyword");
        break;
    case TOK_END:
        fprintf(stream, "\'END\' keyword");
        break;
    case TOK_EOF:
        fprintf(stream, "EOF");
        break;
    case TOK_INVALID:
        fprintf(stream, "invalid character \'%c\'", (int)tok.value);
        break;
    }
}
