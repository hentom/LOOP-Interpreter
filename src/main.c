/*
 * main.c
 *
 * Simple and slow LOOP interpreter using a top down parser LL(1) without an
 * intermediate representation, and thus executing the AST directly.
 *
 * Tom René Hennig
 */


/******************************************************************************
 *                              INCLUDE SECTION
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "var.h"


/******************************************************************************
 *                            FUNCTION DECLARATIONS
 */

void executeProgram(Program *prog, VariableList vars);


/******************************************************************************
 *                           FUNCTION DEFINITIONS
 */

/*
 * Main function checking the command line parameters starting the parser,
 * initializing the varaible list and starting the execution of the AST.
 * ARGS     argc - number of command line parameters
 *          argv - vector of command line parameters
 */
int main(int argc, char *argv[])
{
    // check the number of command line parameters
    if (argc < 2) {
        fprintf(stderr, "Usage: loop <program> [<x1> [<x2> [ ... ]]]\n");
        exit(EXIT_FAILURE);
    }
    
    // try to open the given LOOP program
    FILE *stream = fopen(argv[1], "r");
    if (stream == NULL) {
        perror("ERROR: failed to open input file");
        exit(EXIT_FAILURE);
    }
    
    // build the syntax/semantics tree
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
    
    // start execution
    executeProgram(prog, vars);
    
    // print result of LOOP program (x_0 per definition)
    printf("%ld\n", getValue(vars, 0));
    
    exit(EXIT_SUCCESS);
}

/*
 * Traverse the syntax/semantics tree and simulate execution.
 * ARGS     prog - first statement of the program
 *          vars - initialized varaible list
 */
void executeProgram(Program *prog, VariableList vars)
{
    // input check
    if (prog == NULL || vars == NULL) {
        fprintf(stderr, "ERROR: cannot execute empty program or emtpy varaible list\n");
        exit(EXIT_FAILURE);
    }
    
    // execute the linked list of program statement separated by semicolons
    do {
        if (prog->statement->type == STAT_ASSIGNMENT) { // execute: x_i := x_j +- nat
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
        } else if (prog->statement->type == STAT_LOOP) { // execute LOOP nat times
            Loop *loop = prog->statement->data;
            long limit = getValue(vars, loop->var);
            // fprintf(stderr, "DEBUG: LOOP x%ld DO // %ld\n", loop->var, limit);
            for (int i = 1; i <= limit; ++i)
                executeProgram(loop->program, vars);
            // fprintf(stderr, "DEBUG: END\n");
        }
        prog = prog->next;
    } while (prog != NULL);
}
