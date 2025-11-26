#include "lexical_analyzer.h"

const char *keywords[32] = {
    "auto", "break", "case", "char",
    "const", "continue", "default", "do",
    "double", "else", "enum", "extern",
    "float", "for", "goto", "if",
    "int", "long", "register", "return",
    "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"};

char single_operators[16] = {
    '+', '-', '*', '/', '%', // arithmetic
    '=', '>', '<',           // relational / assignment
    '&', '|', '^', '~',      // bitwise
    '!', '?', ':', '.'       // logical / ternary / structure
};

// Double character operators
char *double_operators[21] = {
    "++", "--",                     // increment / decrement
    "+=", "-=", "*=", "/=", "%=",   // assignment
    "==", "!=", ">=", "<=",         // relational
    "&&", "||",                     // logical
    "<<", ">>",                     // bitwise shift
    "&=", "|=", "^=", "<<=", ">>=", // bitwise assignment
    "->"                            // pointer operator
};

const char punctuators[8] = {'(', ')', '{', '}', '[', ']', ';', ','};

Status ispunctuator(char current)
{
    for (int i = 0; i < 8; i++)
    {
        if (current == punctuators[i])
        {
            return Success;
        }
    }
    return Failure;
}

Status isunary_operators(char current)
{
    for (int i = 0; i < 16; i++)
    {
        if (current == single_operators[i])
        {
            return Success;
        }
    }
    return Failure;
}
Status isbinary_operators(char *current)
{
    int count = sizeof(double_operators) / sizeof(double_operators[0]);
    char op[3] = {current[0], current[1], '\0'};
    for (int i = 0; i < count; i++)
    {
        if (strcmp(op, double_operators[i]) == 0)
        {
            return Success;
        }
    }
    return Failure;
}

Status isKeyword(char token_buffer[])
{
    for (int i = 0; i < 32; i++)
    {
        if (strcmp(token_buffer, keywords[i]) == 0)
        {
            return Success;
        }
    }
    return Failure;
}
Status isconstant(char digit[])
{
    if (digit[0] == '\0')
    {
        return Failure;
    }
    for (int i = 0; digit[i] != '\0'; i++)
    {
        if (!isdigit(digit[i]))
        {
            return Failure;
        }
    }
    return Success;
}

Status is_identifier(char *token_buffer)
{
    if (token_buffer[0] == '\0')
        return Failure;
    if (!(isalpha(token_buffer[0]) || (token_buffer[0] == '_')))
    {
        return Failure;
    }
    for (int i = 0; token_buffer[i] != '\0'; i++)
    {
        if (!(isalnum(token_buffer[i]) || token_buffer[i] == '_'))
        {
            return Failure;
        }
    }
    if (isKeyword(token_buffer) == Success)
    {
        return Failure;
    }
    return Success;
}
void LexicalAnalyzer(Token *tokens)
{
    char *current = tokens->buffer;
    char token_buffer[MAX_TOKEN_LEN];
    int brace_balance = 0;
    int paren_balance = 0;
    char op_token[3];
    printf(YE1 "\n____________________________________________________________________________\n" END);
    printf(GRE "LEXEMES\t\t\t\t\t\tToken Type\n" END);
    printf(YE1 "____________________________________________________________________________\n" END);

    while (*current != '\0')
    {
        if (isspace(*current))
        {
            current++;
            continue;
        }

        if (*current == '#')
        {
            printf(CYA "%-40s : %-25s\n", "#", "Preprocessor Directive" END);
            current++;
            int i = 0;
            while (*current != '\0' && !isspace(*current) && i < MAX_TOKEN_LEN - 1 && (*current) != '<' && (*current) != '"')
            {
                token_buffer[i++] = *current++;
            }
            token_buffer[i] = '\0';
            printf(CYA "%-40s : %-25s\n", token_buffer, "directive (preprocessor command)" END);

            while (isspace(*current))
            {
                current++;
            }
            if (*current == '<' || *current == '"')
            {
                i = 0;
                token_buffer[i++] = *current++;
                while (*current != '>' && *current != '"' && i < MAX_TOKEN_LEN - 1 && *current != '\0')
                {
                    token_buffer[i++] = *current++;
                }
                token_buffer[i++] = *current++;
                token_buffer[i] = '\0';
            }
            printf(CYA "%-40s : %-25s\n", token_buffer, "Header File" END);
            continue;
        }
        if (*current == '/')
        {
            current++;
            if (*current == '/')
            {
                while (*current != '\n' && *current != '\0')
                {
                    current++;
                }
            }
            else if (*current == '*')
            {
                current++;
                while (*current != '\0' && !(*current == '*' && *(current + 1) == '/'))
                {

                    current++;
                }
                if (*current != '\0')
                    current += 2;
            }
            continue;
        }

        if (*current == '"' || *current == '\'')
        {
            char literal = *current;
            int i = 0;
            token_buffer[i++] = *current++;

            while (*current != literal && *current != '\n' && *current != '\0' && i < MAX_TOKEN_LEN - 1)
            {
                if (*current == '\\' && *(current + 1) != '\0')
                {
                    token_buffer[i++] = *current++;
                }
                token_buffer[i++] = *current++;
            }
            token_buffer[i++] = *current;
            token_buffer[i] = '\0';

            if (*current == literal)
            {
                current++;
                char *type = (literal == '"') ? "String Literal" : "Character Literal";
                printf(CYA "%-40s : %-25s\n" END, token_buffer, type);
            }

            continue;
        }

        if (ispunctuator(*current) == Success)
        {
            printf(CYA "%-40c : %-25s\n", *current, "Punctuator" END);
            if (*current == '{')
                brace_balance++;
            if (*current == '}')
                brace_balance--;
            if (*current == '(')
                paren_balance++;
            if (*current == ')')
                paren_balance--;

            current++;
            continue;
        }

        if (isbinary_operators(current) == Success)
        {
            printf(CYA "%-40s : %-25s\n" END, (char[]){current[0], current[1], '\0'}, "Operator");
            current += 2;
            continue;
        }

        if (isunary_operators(*current) == Success)
        {
            printf(CYA "%-40c : %-25s\n" END, *current, "Operator");
            current++;
            continue;
        }
        int i = 0;
        while (*current != '\0' && !isspace(*current) && !isunary_operators(*current) && !isbinary_operators(current) && !ispunctuator(*current))
        {
            token_buffer[i++] = *current++;
        }
        token_buffer[i] = '\0';

        if (i == 0)
        {
            current++;
            continue;
        }

        if (isKeyword(token_buffer) == Success)
        {
            printf(CYA "%-40s : %-25s\n" END, token_buffer, "Keyword");
            continue;
        }
        else if (isconstant(token_buffer) == Success)
        {
            printf(CYA "%-40s : %-25s\n" END, token_buffer, "Constant");
            continue;
        }
        else if (is_identifier(token_buffer) == Success)
        {
            printf(CYA "%-40s : %-25s\n" END, token_buffer, "Identifier" END);
            continue;
        }
        else
        {
            printf(RED "Error:%-34s : %-25s\n", token_buffer, "Unknown" END);
            continue;
        }
        current++;
    }
    printf(YE1 "____________________________________________________________________________\n\n" END);

    if (brace_balance < 0)
    {
        printf(RED "Error:expected declaration specifiers before ‘}’ token ('{' missing)\n" END);
    }
    else if (brace_balance > 0)
    {
        printf(RED "Error: expected declaration or statement at end of input ('}' missing)\n" END);
    }
    else if (paren_balance > 0)
    {
        printf(RED "Error:expected ‘)’ before ‘;’ token (')' missing)\n" END);
    }
    else if (paren_balance < 0)
    {
        printf(RED "Error: expected statement before ‘)’ token ( '(' missing)\n" END);
    }
}
