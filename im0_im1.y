%{
/* See this for code examples:
 *
 * https://github.com/akheron/jansson/blob/master/examples/simple_parse.c
 * http://www.gnu.org/software/bison/manual/bison.html#Multi_002dfunction-Calc
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

#include "print_json.h"

#define YYSTYPE_IS_DECLARED 1
typedef json_t * YYSTYPE;

json_t *program;
json_t *result;
int program_index;
size_t program_size;

int yylex();
void yyerror(const char *);


%}


%define api.value.type {char *}
%token ID
%token LPAR
%token RPAR
%token LSBKT
%token RSBKT
%token SLASH
%token CONST
%token OP
%token ZJUMP
%token NZJUMP
%token JUMP
%token GCX
%token PAR
%token EOP

%%

proc:
    head code EOP {
        $$ = json_array();
        json_array_append($$, $1);
        json_array_append($$, $2);
        result = $$;
    }
;

head:
    ID LPAR SLASH RPAR {
        $$ = json_object();
        json_object_set($$, "type", json_string("proc"));
        json_object_set($$, "name", $1);
        json_t *j = json_object();
        json_object_set(j, "in", json_array());
        json_object_set(j, "out", json_array());
        json_object_set($$, "args", j);
    }
;

code:
    preface paragraph {
        $$ = $2;
        json_array_insert($$, 0, $1);
    }
  | preface {
        $$ = json_array();
        json_array_append($$, $1);
  }
;

preface:
    expressions {
        json_t *j = json_object();
        json_object_set(j, "type", json_string("preface"));
        $$ = json_array();
        json_array_append($$, j);
        json_array_append($$, $1);
    }
;

paragraph:
    PAR CONST expressions paragraph {
        json_t *j = json_object();
        json_object_set(j, "type", json_string("par"));
        json_object_set(j, "number", $2);
        json_t *a = json_array();
        json_array_append(a, j);
        json_array_append(a, $3);
        $$ = $4;
        json_array_insert($$, 0, a);
    }
  | PAR CONST expressions {
        json_t *j = json_object();
        json_object_set(j, "type", json_string("par"));
        json_object_set(j, "number", $2);
        json_t *a = json_array();
        json_array_append(a, j);
        json_array_append(a, $3);
        $$ = json_array();
        json_array_append($$, a);
        result = $$;
    }
;

expressions:
    operation expressions {
        $$ = $2;
        json_array_insert($$, 0, $1);
    }
  | operation {
        $$ = json_array();
        json_array_append($$, $1);
  }
;

operation:  
    OP arg {
        json_t *j = json_object();
        json_object_set(j, "type", json_string("op"));
        json_object_set(j, "name", $1);
        $$ = json_array();
        json_array_append($$, j);
        json_array_append($$, $2);
    }
  | arg {
        json_t *j = json_object();
        json_object_set(j, "type", json_string("op"));
        json_object_set(j, "name", json_string("load"));
        $$ = json_array();
        json_array_append($$, j);
        json_array_append($$, $1);
  }
;

arg:
    CONST {
        json_t *j = json_object();
        json_object_set(j, "type", json_string("const"));
        json_object_set(j, "value", $1);
        $$ = json_array();
        json_array_append($$, j);
    }
  | ID {
        json_t *j = json_object();
        json_object_set(j, "type", json_string("var"));
        json_object_set(j, "name", $1);
        $$ = json_array();
        json_array_append($$, j);
    }
  | comp_el {
        $$ = json_array();
        json_array_append($$, $1);
  }
;

comp_el:
    GCX ID                { 
        $$ = json_object();
        json_object_set($$, "type", json_string("g1cx"));
        json_object_set($$, "idx", $2);
    }
  | GCX CONST             {
        $$ = json_object();
        json_object_set($$, "type", json_string("g1cx"));
        json_object_set($$, "idx", $2);
    }
  | GCX LSBKT ID RSBKT    {
        $$ = json_object();
        json_object_set($$, "type", json_string("g4cx"));
        json_object_set($$, "idx", $3);
    }
  | GCX LSBKT CONST RSBKT {
        $$ = json_object();
        json_object_set($$, "type", json_string("g4cx"));
        json_object_set($$, "idx", $3);
    }
;
    

%%


int yylex() {
    /* See: http://www.gnu.org/software/bison/manual/html_node/Rpcalc-Lexer.html#Rpcalc-Lexer */

    //static int program_index = 0;
    //static size_t program_size = json_array_size(program);
    if (program_index >= program_size)
        return 0;
    json_t *program_element = json_array_get(program, program_index);
    ++program_index;
    if (json_typeof(program_element) == JSON_OBJECT) {
        json_t *element_type = json_object_get(program_element, "type");
        json_t *element_value = json_object_get(program_element, "value");
        if (element_type && element_value) {
            if (json_typeof(element_type) == JSON_STRING) {
                yylval = element_value;
                if (!strcmp(json_string_value(element_type), "id")) {
                    if (json_typeof(element_value) == JSON_STRING)
                        return ID;
                } else if (!strcmp(json_string_value(element_type), "lpar")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return LPAR;
                } else if (!strcmp(json_string_value(element_type), "rpar")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return RPAR;
                } else if (!strcmp(json_string_value(element_type), "lsbkt")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return LSBKT;
                } else if (!strcmp(json_string_value(element_type), "rsbkt")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return RSBKT;
                } else if (!strcmp(json_string_value(element_type), "slash")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return SLASH;
                } else if (!strcmp(json_string_value(element_type), "const")) {
                    if (json_typeof(element_value) == JSON_INTEGER)
                        return CONST;
                } else if (!strcmp(json_string_value(element_type), "operation")) {
                    if (json_typeof(element_value) == JSON_STRING)
                        return OP;
                } else if (!strcmp(json_string_value(element_type), "jump")) {
                    if (json_typeof(element_value) == JSON_STRING) 
                        return JUMP;
                } else if (!strcmp(json_string_value(element_type), "complex")) {
                    if (json_typeof(element_value) == JSON_STRING) 
                        return GCX;
                } else if (!strcmp(json_string_value(element_type), "par")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return PAR;
                } else if (!strcmp(json_string_value(element_type), "eop")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return EOP;
                }
            }
        }
    }

    return 0;
}

void yyerror(const char *msg) {
    fprintf(stderr, "err: %s\n", msg);
}

int main(int argc, char *argv[]) {
    json_error_t json_err;
    program = json_loadf(stdin, 0, &json_err);
    if (!program) {
        fprintf(stderr, "Ircorrect JSON\n");
        return 0;
    }
    if (json_typeof(program) != JSON_ARRAY) {
        fprintf(stderr, "Ircorrect JSON\n");
        return 0;
    }
    program_size = json_array_size(program);
    result = json_array();
    int retcode;
    retcode = yyparse();
    print_json(result);
    return retcode;
}
