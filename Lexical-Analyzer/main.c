#include "lexical_analyzer.h"

Token tokens;
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./a.out <filename>.c\n");
        return 0;
    }

    if (argv[1] != NULL)
    {
        if (validate_file(argv) == Success)
        {
            tokens.fptr = fopen(argv[1], "r");
            if (tokens.fptr == NULL)
            {
                printf("Error:While Opening the .c file\n");
                return 0;
            }
            else
            {

                fseek(tokens.fptr, 0, SEEK_END);
                tokens.filesize = ftell(tokens.fptr);
                tokens.buffer = malloc(tokens.filesize + 1);
                if (tokens.buffer == NULL)
                {
                    printf("Error:Memory Allocation Failed!\n");
                    return 0;
                }
                rewind(tokens.fptr);
                fread(tokens.buffer, tokens.filesize + 1, 1, tokens.fptr);
                tokens.buffer[tokens.filesize] = '\0';
                LexicalAnalyzer(&tokens);
                free(tokens.buffer);
            }
            fclose(tokens.fptr);
        }
        else
        {
            printf("Invalid file.please pass the correct .c file\n");
        }
    }
}

Status validate_file(char *argv[])
{
    char *file_extn = strstr(argv[1], ".c");
    if (file_extn != NULL)
    {
        if (strlen(file_extn) == 2)
        {
            return Success;
        }
        else
        {
            return Failure;
        }
    }
}