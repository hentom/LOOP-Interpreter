/*
 * main.c
 *
 * Tom René Hennig
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "var.h"


void executeProgram(Program *prog, VariableList vars);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: loop <program> [<x1> [<x2> [ ... ]]]\n");
        exit(EXIT_FAILURE);
    }
    FILE *stream = fopen(argv[1], "r");
    if (stream == NULL) {
        perror("ERROR: failed to open input file");
        exit(EXIT_FAILURE);
    }
    Program *prog = parse(stream);
    VariableList vars = addVariable(NULL, 0);
    for (int i = 2; i < argc; ++i) {
        int tmp = atoi(argv[i]);
        if (tmp < 0) {
            fprintf(stderr, "ERROR: negative value %d given for x%d\n", tmp, i - 1);
            exit(EXIT_FAILURE);
        }
        setValue(vars, i - 1, tmp);
    }
    executeProgram(prog, vars);
    printf("%ld\n", getValue(vars, 0));
    
    exit(EXIT_SUCCESS);
}

void executeProgram(Program *prog, VariableList vars)
{
    if (prog->statement->type == STAT_ASSIGNMENT) {
        Assignment *ass = prog->statement->data;
        long res = getValue(vars, ass->rvalue);
        if (ass->isAddition)
            res += ass->nat;
        else
            res -= ass->nat;
        if (res < 0)
            res = 0;
        setValue(vars, ass->lvalue, res);
        // fprintf(stderr, "DEBUG: x%ld := x%ld %c %ld // %ld\n", ass->lvalue, ass->rvalue, ass->isAddition ? '+' : '-', ass->nat, res);
    } else if (prog->statement->type == STAT_LOOP) {
        Loop *loop = prog->statement->data;
        long limit = getValue(vars, loop->var);
        // fprintf(stderr, "DEBUG: LOOP x%ld DO // %ld\n", loop->var, limit);
        for (int i = 1; i <= limit; ++i)
            executeProgram(loop->program, vars);
        // fprintf(stderr, "DEBUG: END\n");
    }
    if (prog->next != NULL) {
        // fprintf(stderr, "DEBUG: ;\n");
        executeProgram(prog->next, vars);
    }
}
