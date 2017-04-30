/*
 * token.c
 *
 * Simple lexer reading character by character and natural numbers.
 *
 * Tom René Hennig
 */

/******************************************************************************
 *                              INCLUDE SECTION
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "token.h"


/******************************************************************************
 *                            GLOBAL DECLARATIONS
 */

static Token buf;               // Buffer for returned tokens in token stream
static bool bufUsed = false;    // used-flag for token buffer


/******************************************************************************
 *                           FUNCTION DEFINITIONS
 */

/*
 * Read next token from given input stream.
 * ARGUMENTS    stream - input stream (libc)
 * RETURN       token read from input stream with type attribute is guaranteed
 *              to contain reasonable value
 */
Token nextToken(FILE *stream)
{
    // input check
    if (stream == NULL) {
        fprintf(stderr, "ERROR: invalid stream to read from\n");
        exit(EXIT_FAILURE);
    }
    
    // Return buffer content if buffer is used and reset used-flag
    if (bufUsed) {
        bufUsed = false;
        return buf;
    }
    
    // Skip leading blanks and whitespace characters
    int c = fgetc(stream);
    while (isspace(c) && c != EOF)
        c = fgetc(stream);
    
    Token tok;
    switch (c) {
    case 'x':       // beginning variable identifier, read identifier number
        tok.type = TOK_VAR_ID;
        tok.value = 0;
        while (isdigit(c = fgetc(stream))) {
            tok.value *= 10;
            tok.value += c - '0';
        }
        ungetc(c, stream);
        break;
    case '0': case '1': case '2': case '3': case '4':   // beginning natural
    case '5': case '6': case '7': case '8': case '9':   // number
        tok.type = TOK_NAT_NUM;
        tok.value = c - '0';
        while (isdigit(c = fgetc(stream))) {
            tok.value *= 10;
            tok.value += c - '0';
        }
        ungetc(c, stream);
        break;
    case ':':       // beginning assignment operator
        if ((c = fgetc(stream)) != '=') {
            tok.type = TOK_INVALID;
            tok.value = c;
            break;
        }
        tok.type = TOK_ASS;
        break;
    case '+':       // plus operator
        tok.type = TOK_PLUS;
        break;
    case '-':       // minus operator
        tok.type = TOK_MINUS;
        break;
    case ';':       // semicolon
        tok.type = TOK_SEM;
        break;
    case 'L':       // beginning keyword 'LOOP'
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
    case 'D':       // beginning keyword 'DO'
        if ((c = fgetc(stream)) != 'O') {
            tok.type = TOK_INVALID;
            tok.value = c;
            break;
        }
        tok.type = TOK_DO;
        break;
    case 'E':       // beginning keyword 'END'
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
    case EOF:       // reached end of file
        tok.type = TOK_EOF;
        break;
    default:        // found invalid character
        tok.type = TOK_INVALID;
        tok.value = c;
        break;
    }
    return tok;
}

/*
 * Write token to buffer, if possible.
 * ARGUMENTS    tok - token to be pushed into buffer
 * RETURN       return on success, exit on filled buffer
 */
void pushToken(Token tok)
{
    if (bufUsed) {
        fprintf(stderr, "ERROR: cannot push back more than one token\n");
        exit(EXIT_FAILURE);
    }
    buf = tok;
    bufUsed = true;
}

/*
 * Print human readable represantation of token to stream.
 * ARGUMENTS    stream - output file stream
 *              tok    - token to be printed (at least type is printed)
 * RETURN       always returns
 */
void printToken(FILE* stream, Token tok)
{
    // input check
    if (stream == NULL) {
        fprintf(stderr, "ERROR: invalid stream to print to\n");
        exit(EXIT_FAILURE);
    }
    
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
