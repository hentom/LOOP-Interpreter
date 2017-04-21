/*
 * parser.h
 *
 * Tom René Hennig
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stdio.h>

/*
 * Declaration of primitive types
 */
typedef long VarID;

typedef long NatNum;

/*
 * Rules of the CFG
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


Program *parse(FILE *stream);

#endif /* PARSER_H */
