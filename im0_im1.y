%{
/* See this for code examples:
 *
 * https://github.com/akheron/jansson/blob/master/examples/simple_parse.c
 * http://www.gnu.org/software/bison/manual/bison.html#Multi_002dfunction-Calc
 */

#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>

char fake_id[] = "x";

int yylex();
void yyerror(const char *);

%}


%define api.value.type {char *}
%token ID


%%

prog:
    ID  { printf("%s\n", $1); }
;

%%


int yylex() {
    /* See: http://www.gnu.org/software/bison/manual/html_node/Rpcalc-Lexer.html#Rpcalc-Lexer */

    static int first_time = 1;

    if (first_time) {
        first_time = 0;
        yylval = fake_id;
        return ID;
    }

    return 0;
}

void yyerror(const char *msg) {
    fprintf(stderr, "err: %s\n", msg);
}

int main(int argc, char *argv[]) {
    int retcode;
    retcode = yyparse();
    return retcode;
}
