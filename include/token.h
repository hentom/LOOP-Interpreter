/*
 * token.h
 *
 * Simple lexer reading character by character and natural numbers.
 *
 * Tom René Hennig
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>


/******************************************************************************
 *                             TYPE DECLARATIONS
 */

/*
 * Type of the token.
 */
typedef enum {
    TOK_VAR_ID,     // variable identifier of form 'x' ('0' | ... | '9')*
    TOK_NAT_NUM,    // natural number of form ('0' | ... | '9')*
    TOK_ASS,        // assignment operator ':='
    TOK_PLUS,       // plus operator '+'
    TOK_MINUS,      // minus operator '-'
    TOK_SEM,        // semicolon ';'
    TOK_LOOP,       // keyword 'LOOP'
    TOK_DO,         // keyword 'DO'
    TOK_END,        // keyword 'END'
    TOK_EOF,        // reached end of file
    TOK_INVALID     // invalid token
} TokenType;

/*
 * Token consisting of its type and content. The content clearly depends on the
 * type. Variable identifiers, natural numbers and character IDs of ivalid
 * tokens are casted to long.
 */
typedef struct {
    TokenType type;
    long value;
} Token;


/******************************************************************************
 *                            FUNCTION DECLARATIONS
 */

/*
 * Read next token from given input stream, return value is stack object.
 */
Token nextToken(FILE *stream);

/*
 * Place already read token back in the imaginary token stream. Buffer size is
 * one and this call halts program on overflow. Cannot distinguish between
 * different streams!
 */
void pushToken(Token tok);


/*
 * Print type and content in short form to given stream.
 */
void printToken(FILE* stream, Token tok);

#endif /* TOKEN_H */
