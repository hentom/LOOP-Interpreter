/*
 * parser.c
 *
 * Simple LL(1) parser with the following grammar:
 *  Program    := Statement Program | NULL
 *  Statement  := Assignment | Loop
 *  Assignment := VarID ':=' VarID '+' NatNum | VarID ':=' VarID '-' NatNum
 *  Loop       := 'LOOP' VarID 'DO' Program 'END'
 *
 * Tom René Hennig
 */


/******************************************************************************
 *                              INCLUDE SECTION
 */

#include <stdlib.h>

#include "parser.h"
#include "token.h"


/******************************************************************************
 *                           FUNCTION DEFINITIONS
 */

/*
 * Foward declaration because of the cyclic structure of LOOP programs (nesting
 * of LOOPs).
 */
Program *readProgram(FILE *stream);

/*
 * Read a statement from stream determining its type with the first token read.
 * ARGS     stream - libc stream to read from
 * RETURN   pointer to the newly read and allocated statement structure
 */
Statement *readStatement(FILE *stream)
{
    // input check
    if (stream == NULL) {
        fprintf(stderr, "ERROR: invalid stream to read from\n");
        exit(EXIT_FAILURE);
    }
    
    // allocate memory for the new statement
    Statement *stat = malloc(sizeof(Statement));
    if (stat == NULL) {
        fprintf(stderr, "ERROR: unable to to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    // read first token to decide statement type from
    Token tok = nextToken(stream);
    switch (tok.type) {
    case TOK_VAR_ID:                    // start reading assignment
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
    case TOK_LOOP:                      // start reading a loop
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
    default:                            // print error message on all other tokens
        fprintf(stderr, "PARSER: expected variable identifier or \'LOOP\' instead of ");
        printToken(stderr, tok);
        fputc('\n', stderr);
        exit(EXIT_FAILURE);
    }
    return stat;
}

/*
 * Read program as a sequence of semicolon separted statements
 * ARGS     stream - libc stream to read from
 * RETURN   pointer to the newly read and allocated statement structure
 */
Program *readProgram(FILE *stream)
{
    // input check
    if (stream == NULL) {
        fprintf(stderr, "ERROR: invalid stream to read from\n");
        exit(EXIT_FAILURE);
    }
    
    // allocate memory for the new statement
    Program *prog = malloc(sizeof(Program));
    if (prog == NULL) {
        fprintf(stderr, "ERROR: unable to to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    // read first program statement
    prog->statement = readStatement(stream);
    
    // Check if semicolon signals a following statement, and read recursively
    // it if possible
    Token tok = nextToken(stream);
    if (tok.type == TOK_SEM) {
        prog->next = readProgram(stream);
    } else {
        pushToken(tok);
    }
    return prog;
}

/*
 * Parse input from stream with the help of the lexer into an AST of the
 * structure as shown above.
 * ARGS     stream - libc standard stream
 * RETURN   pointer to the first executable program statement/the AST.
 */
Program *parse(FILE *stream)
{
    // input check
    if (stream == NULL) {
        fprintf(stderr, "ERROR: invalid stream to read from\n");
        exit(EXIT_FAILURE);
    }
    
    // read program and check for terminating end of file character (EOF)
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
