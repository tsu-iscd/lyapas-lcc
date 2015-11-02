%{
#include <stdio.h>

int first_time = 1;
%}

%%

[a-zA-Z_][a-zA-Z0-9_]*  {
                            if (!first_time) {
                                printf(",");
                            } else {
                                first_time = 0;
                            }
                            printf("\n  {\"type\": \"id\", \"value\": \"%s\"}", yytext);
                        }

%%

int main(int argc, char *argv[]) {
    yyin = stdin;
    printf("[");
    yylex();
    printf("]\n");
    return 0;
}
