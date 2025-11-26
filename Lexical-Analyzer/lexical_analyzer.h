#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H
#define MAX_TOKEN_LEN 100

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define RED "\033[1;31m"
#define YEL "\033[4;33m"
#define YE1 "\033[1;33m"
#define GRE "\033[1;32m"
#define CYA "\033[1;36m"
#define MAG "\033[1;35m"
#define END "\033[0m"

typedef struct
{
    FILE *fptr;
    char *buffer;
    long filesize;

} Token;

typedef enum
{
    Failure,
    Success
} Status;


void LexicalAnalyzer(Token *tokens);

Status validate_file(char *argv[]);

Status ispunctuator(char current);

Status isbinary_operators(char *current);

Status isKeyword(char token_buffer[]);

Status isconstant(char digit[]);

Status isunary_operators(char current);

Status is_identifier(char *token_buffer);

#endif
