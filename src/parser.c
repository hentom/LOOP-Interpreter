/*
 * parser.c
 *
 * Tom René Hennig
 */

#include <stdlib.h>

#include "parser.h"
#include "token.h"


Program *readProgram(FILE *stream);

Statement *readStatement(FILE *stream)
{
    Statement *stat = malloc(sizeof(Statement));
    if (stat == NULL) {
        fprintf(stderr, "ERROR: unable to to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    Token tok = nextToken(stream);
    switch (tok.type) {
    case TOK_VAR_ID:
        stat->type = STAT_ASSIGNMENT;
        stat->data = malloc(sizeof(Assignment));
        if (stat->data == NULL) {
            fprintf(stderr, "ERROR: unable to to allocate memory\n");
            exit(EXIT_FAILURE);
        }
        Assignment *ass = stat->data;
        ass->lvalue = tok.value;
        tok = nextToken(stream);
        if (tok.type != TOK_ASS) {
            fprintf(stderr, "PARSER: expected \':=\' instead of ");
            printToken(stderr, tok);
            fputc('\n', stderr);
            exit(EXIT_FAILURE);
        }
        tok = nextToken(stream);
        if (tok.type != TOK_VAR_ID) {
            fprintf(stderr, "PARSER: expected variable identifier instead of ");
            printToken(stderr, tok);
            fputc('\n', stderr);
            exit(EXIT_FAILURE);
        }
        ass->rvalue = tok.value;
        tok = nextToken(stream);
        if (tok.type == TOK_PLUS) {
            ass->isAddition = true;
        } else if (tok.type == TOK_MINUS) {
            ass->isAddition = false;
        } else {
            fprintf(stderr, "PARSER: expected \'+\' or \'-\' instead of ");
            printToken(stderr, tok);
            fputc('\n', stderr);
            exit(EXIT_FAILURE);
        }
        tok = nextToken(stream);
        if (tok.type != TOK_NAT_NUM) {
            fprintf(stderr, "PARSER: expected variable identifier instead of ");
            printToken(stderr, tok);
            fputc('\n', stderr);
            exit(EXIT_FAILURE);
        }
        ass->nat = tok.value;
        break;
    case TOK_LOOP:
        stat->type = STAT_LOOP;
        stat->data = malloc(sizeof(Loop));
        if (stat->data == NULL) {
            fprintf(stderr, "ERROR: unable to to allocate memory\n");
            exit(EXIT_FAILURE);
        }
        Loop *loop = stat->data;
        tok = nextToken(stream);
        if (tok.type != TOK_VAR_ID) {
            fprintf(stderr, "PARSER: expected variable identifier instead of ");
            printToken(stderr, tok);
            fputc('\n', stderr);
            exit(EXIT_FAILURE);
        }
        loop->var = tok.value;
        tok = nextToken(stream);
        if (tok.type != TOK_DO) {
            fprintf(stderr, "PARSER: expected \'DO\' instead of ");
            printToken(stderr, tok);
            fputc('\n', stderr);
            exit(EXIT_FAILURE);
        }
        loop->program = readProgram(stream);
        tok = nextToken(stream);
        if (tok.type != TOK_END) {
            fprintf(stderr, "PARSER: expected \'END\' instead of ");
            printToken(stderr, tok);
            fputc('\n', stderr);
            exit(EXIT_FAILURE);
        }
        break;
    default:
        fprintf(stderr, "PARSER: expected variable identifier or \'LOOP\' instead of ");
        printToken(stderr, tok);
        fputc('\n', stderr);
        exit(EXIT_FAILURE);
    }
    return stat;
}

Program *readProgram(FILE *stream)
{
    Program *prog = malloc(sizeof(Program));
    if (prog == NULL) {
        fprintf(stderr, "ERROR: unable to to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    prog->statement = readStatement(stream);
    Token tok = nextToken(stream);
    if (tok.type == TOK_SEM) {
        prog->next = readProgram(stream);
    } else {
        pushToken(tok);
    }
    return prog;
}

Program *parse(FILE *stream)
{
    Program *prog = readProgram(stream);
    Token tok = nextToken(stream);
    if (tok.type != TOK_EOF) {
        fprintf(stderr, "PARSER: expected EOF instead of ");
        printToken(stderr, tok);
        fputc('\n', stderr);
        exit(EXIT_FAILURE);
    }
    return prog;
}
