/*
 * parser.h
 *
 * Simple LL(1) parser with the following grammar:
 *  Program    := Statement Program | NULL
 *  Statement  := Assignment | Loop
 *  Assignment := VarID ':=' VarID '+' NatNum | VarID ':=' VarID '-' NatNum
 *  Loop       := 'LOOP' VarID 'DO' Program 'END'
 *
 * Tom René Hennig
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stdio.h>


/******************************************************************************
 *                             TYPE DECLARATIONS
 */

/*
 * Declaration of primitive types
 */
typedef long VarID;

typedef long NatNum;

/*
 * Rules of the CFG listed at the beginning of this file
 */
typedef enum {
    STAT_ASSIGNMENT,
    STAT_LOOP,
} StatementType;

typedef struct {
    StatementType type;
    void *data;
} Statement;

typedef struct sProgram {
    Statement *statement;
    struct sProgram *next;
} Program;

typedef struct {
    VarID lvalue;
    VarID rvalue;
    NatNum nat;
    bool isAddition;
} Assignment;

typedef struct {
    VarID var;
    Program *program;
} Loop;


/******************************************************************************
 *                            FUNCTION DECLARATIONS
 */

/*
 * Parse input from stream with the help of the lexer into an AST of the
 * structure as shown above.
 * ARGS     stream - libc standard stream
 * RETURN   pointer to the first executable program statement/the AST.
 */
Program *parse(FILE *stream);

#endif /* PARSER_H */
