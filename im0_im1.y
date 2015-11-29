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
%token S_COMP L_COMP G_COMP COMP_CAP COMP_CARD INT ID STRING SET_BIT SPEC_VAR FRET LPAR RPAR INC DEC PLUS MINUS WEIGHT MOD DDIV OR AND XOR NUM LEAST_BIT CONSOLE L_ANG_BRACK R_ANG_BRACK SLASH LETTER_X STAR UP_ARROW SET_MIN SET_MAX ASSIGN SWAP TIME PAR EQ NEQ GEQ LEQ JUMP JUMPZ JUMPNZ LBRACK RBRACK QUOTE COMMA DOT LBRACE RBRACE ASM

%%

proc:
    head body FRET {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("proc"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $1);
        json_array_append($$, $2);
        result = $$;
    }
;

head:
    ID head_string {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("head"));
        json_object_set(root, "name", $1);

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
;

head_string:
    LPAR proc_args SLASH proc_args RPAR {
        json_t *in_args = json_array();
        json_t *in_args_root = json_object();
        json_object_set(in_args_root, "type", json_string("in_args"));
        json_array_append(in_args, in_args_root);
        json_array_extend(in_args, $2);

        json_t *out_args = json_array();
        json_t *out_args_root = json_object();
        json_object_set(out_args_root, "type", json_string("out_args"));
        json_array_append(out_args, out_args_root);
        json_array_extend(out_args, $4);

        $$ = json_array();
        json_array_append($$, in_args);
        json_array_append($$, out_args);
    }
    | LPAR proc_args SLASH RPAR {
        json_t *in_args = json_array();
        json_t *in_args_root = json_object();
        json_object_set(in_args_root, "type", json_string("in_args"));
        json_array_append(in_args, in_args_root);
        json_array_extend(in_args, $2);

        json_t *out_args = json_array();
        json_t *out_args_root = json_object();
        json_object_set(out_args_root, "type", json_string("out_args"));
        json_array_append(out_args, out_args_root);

        $$ = json_array();
        json_array_append($$, in_args);
        json_array_append($$, out_args);
    }
    | LPAR SLASH proc_args RPAR {
        json_t *in_args = json_array();
        json_t *in_args_root = json_object();
        json_object_set(in_args_root, "type", json_string("in_args"));
        json_array_append(in_args, in_args_root);

        json_t *out_args = json_array();
        json_t *out_args_root = json_object();
        json_object_set(out_args_root, "type", json_string("out_args"));
        json_array_append(out_args, out_args_root);
        json_array_extend(out_args, $3);

        $$ = json_array();
        json_array_append($$, in_args);
        json_array_append($$, out_args);
    }
    | LPAR SLASH RPAR {
        json_t *in_args = json_array();
        json_t *in_args_root = json_object();
        json_object_set(in_args_root, "type", json_string("in_args"));
        json_array_append(in_args, in_args_root);

        json_t *out_args = json_array();
        json_t *out_args_root = json_object();
        json_object_set(out_args_root, "type", json_string("out_args"));
        json_array_append(out_args, out_args_root);

        $$ = json_array();
        json_array_append($$, in_args);
        json_array_append($$, out_args);
    }
;

proc_args:
    proc_arg COMMA proc_args {
        $$ = json_array();   
        json_array_append($$, $1);
        json_array_extend($$, $3);
    }
    | proc_arg {
        $$ = json_array();   
        json_array_append($$, $1);
    }
;

proc_arg:
    arg {
        $$ = $1;
    }
    | comp {
        $$ = $1;
    }
;

body:
    preface paragraphs {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("body"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $1);
        json_array_extend($$, $2);
    }
    | preface {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("body"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $1);
    }
;

preface:
    expressions {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("preface"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $1);
    }
;

paragraphs:
    paragraph paragraphs {
        $$ = json_array();
        json_array_append($$, $1);
        json_array_extend($$, $2);
    }
    | paragraph {
        $$ = json_array();
        json_array_append($$, $1);
    }
;

paragraph:
    PAR INT expressions {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("par"));
        json_object_set(root, "number", $2);

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $3);
    }
    | PAR INT {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("par"));
        json_object_set(root, "number", $2);

        $$ = json_array();
        json_array_append($$, root);
    }
;

expressions:
    expression expressions {
        $$ = json_array();
        json_array_append($$, $1);
        json_array_extend($$, $2);
    }
    | expression {
        $$ = json_array();
        json_array_append($$, $1);
    }
;

expression:  
    PLUS arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("add"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | MINUS arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("sub"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | INC arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("inc"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | DEC arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("dec"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | STAR arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("mul"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | SLASH arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("div"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | DDIV arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("ddiv"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | MOD arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("mod"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | AND arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("and"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | OR arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("or"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | XOR arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("xor"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | SET_MIN arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("set_min"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | SET_MAX arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("set_max"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | ASSIGN arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("store"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | WEIGHT arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("weight"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | TIME arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("get_time"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | L_ANG_BRACK arg {
        puts(" <");
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("left_shift"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | R_ANG_BRACK arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("right_shift"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }

    | JUMP INT {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("jump"));

        json_t *arg_root = json_object();
        json_object_set(arg_root, "type", json_string("const"));
        json_object_set(arg_root, "value", $2);

        json_t *arg = json_array();
        json_array_append(arg, arg_root);

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, arg);
    }
    | JUMPZ INT {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("jump_z"));

        json_t *arg_root = json_object();
        json_object_set(arg_root, "type", json_string("const"));
        json_object_set(arg_root, "value", $2);

        json_t *arg = json_array();
        json_array_append(arg, arg_root);

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, arg);
    }
    | JUMPNZ INT {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("jump_nz"));

        json_t *arg_root = json_object();
        json_object_set(arg_root, "type", json_string("const"));
        json_object_set(arg_root, "value", $2);

        json_t *arg = json_array();
        json_array_append(arg, arg_root);

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, arg);
    }
    | jump_cond {
        $$ = $1;
    }

    | arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("load"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $1);
    }

    | LBRACE ASM RBRACE {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("asm"));
        json_object_set(root, "command", $2);

        $$ = json_array();
        json_array_append($$, root);
    }
    | STAR ID head_string {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("call"));
        json_object_set(root, "name", $2);

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $3);
    }
;

jump_cond:
    UP_ARROW LPAR arg EQ arg RPAR INT {
        json_t *cmp_root = json_object();
        json_object_set(cmp_root, "type", json_string("eq"));

        json_t *cmp = json_array();
        json_array_append(cmp, cmp_root);
        json_array_append(cmp, $3);
        json_array_append(cmp, $5);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("jump_cond"));

        json_t *dst_root = json_object();
        json_object_set(dst_root, "type", json_string("const"));
        json_object_set(dst_root, "value", $6);

        json_t *dst = json_array();
        json_array_append(dst, dst_root);

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, cmp);
        json_array_append($$, dst);
    }
    | UP_ARROW LPAR arg NEQ arg RPAR INT {
        json_t *cmp_root = json_object();
        json_object_set(cmp_root, "type", json_string("neq"));

        json_t *cmp = json_array();
        json_array_append(cmp, cmp_root);
        json_array_append(cmp, $3);
        json_array_append(cmp, $5);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("jump_cond"));

        json_t *dst_root = json_object();
        json_object_set(dst_root, "type", json_string("const"));
        json_object_set(dst_root, "value", $6);

        json_t *dst = json_array();
        json_array_append(dst, dst_root);

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, cmp);
        json_array_append($$, dst);
    }
    | UP_ARROW LPAR arg LEQ arg RPAR INT {
        json_t *cmp_root = json_object();
        json_object_set(cmp_root, "type", json_string("leq"));

        json_t *cmp = json_array();
        json_array_append(cmp, cmp_root);
        json_array_append(cmp, $3);
        json_array_append(cmp, $5);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("jump_cond"));

        json_t *dst_root = json_object();
        json_object_set(dst_root, "type", json_string("const"));
        json_object_set(dst_root, "value", $6);

        json_t *dst = json_array();
        json_array_append(dst, dst_root);

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, cmp);
        json_array_append($$, dst);
    }
    | UP_ARROW LPAR arg GEQ arg RPAR INT {
        json_t *cmp_root = json_object();
        json_object_set(cmp_root, "type", json_string("geq"));

        json_t *cmp = json_array();
        json_array_append(cmp, cmp_root);
        json_array_append(cmp, $3);
        json_array_append(cmp, $5);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("jump_cond"));

        json_t *dst_root = json_object();
        json_object_set(dst_root, "type", json_string("const"));
        json_object_set(dst_root, "value", $6);

        json_t *dst = json_array();
        json_array_append(dst, dst_root);

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, cmp);
        json_array_append($$, dst);
    }
    | UP_ARROW LPAR arg L_ANG_BRACK arg RPAR INT {
        json_t *cmp_root = json_object();
        json_object_set(cmp_root, "type", json_string("lt"));

        json_t *cmp = json_array();
        json_array_append(cmp, cmp_root);
        json_array_append(cmp, $3);
        json_array_append(cmp, $5);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("jump_cond"));

        json_t *dst_root = json_object();
        json_object_set(dst_root, "type", json_string("const"));
        json_object_set(dst_root, "value", $6);

        json_t *dst = json_array();
        json_array_append(dst, dst_root);

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, cmp);
        json_array_append($$, dst);
    }
    | UP_ARROW LPAR arg R_ANG_BRACK arg RPAR INT {
        json_t *cmp_root = json_object();
        json_object_set(cmp_root, "type", json_string("gt"));

        json_t *cmp = json_array();
        json_array_append(cmp, cmp_root);
        json_array_append(cmp, $3);
        json_array_append(cmp, $5);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("jump_cond"));

        json_t *dst_root = json_object();
        json_object_set(dst_root, "type", json_string("const"));
        json_object_set(dst_root, "value", $6);

        json_t *dst = json_array();
        json_array_append(dst, dst_root);

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, cmp);
        json_array_append($$, dst);
    }
;

arg:
    INT {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("const"));
        json_object_set(root, "value", $1);

        $$ = json_array();
        json_array_append($$, root);
    }
    | ID {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("var"));
        json_object_set(root, "name", $1);

        $$ = json_array();
        json_array_append($$, root);
    }
    | comp_el {
        $$ = json_array();
        json_array_append($$, $1);
    }
;

comp:
    L_COMP INT {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("logic_complex"));
        json_object_set(root, "number", $2);

        $$ = json_array();
        json_array_append($$, root);
    }
    | S_COMP INT {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("symbol_complex"));
        json_object_set(root, "number", $2);

        $$ = json_array();
        json_array_append($$, root);
    }
;

comp_el:
    G_COMP ID { 
        $$ = json_object();
        json_object_set($$, "type", json_string("global_complex_1"));
        json_object_set($$, "idx", $2);
    }
    | G_COMP INT {
        $$ = json_object();
        json_object_set($$, "type", json_string("global_complex_1"));
        json_object_set($$, "idx", $2);
    }
    | G_COMP LBRACK ID RBRACK {
        $$ = json_object();
        json_object_set($$, "type", json_string("global_complex_4"));
        json_object_set($$, "idx", $3);
    }
    | G_COMP LBRACK INT RBRACK {
        $$ = json_object();
        json_object_set($$, "type", json_string("global_complex_4"));
        json_object_set($$, "idx", $3);
    }
    | L_COMP INT DOT INT {
        $$ = json_object();
        json_object_set($$, "type", json_string("logic_complex"));
        json_object_set($$, "number", $2);
        json_object_set($$, "idx", $4);
    }
    | L_COMP INT ID {
        $$ = json_object();
        json_object_set($$, "type", json_string("logic_complex"));
        json_object_set($$, "number", $2);
        json_object_set($$, "idx", $3);
    }
    | S_COMP INT DOT INT {
        $$ = json_object();
        json_object_set($$, "type", json_string("symbol_complex"));
        json_object_set($$, "number", $2);
        json_object_set($$, "idx", $4);
    }
    | S_COMP INT ID {
        $$ = json_object();
        json_object_set($$, "type", json_string("symbol_complex"));
        json_object_set($$, "number", $2);
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
                } else if (!strcmp(json_string_value(element_type), "lbrack")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return LBRACK;
                } else if (!strcmp(json_string_value(element_type), "rbrack")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return RBRACK;
                } else if (!strcmp(json_string_value(element_type), "lbrace")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return LBRACE;
                } else if (!strcmp(json_string_value(element_type), "rbrace")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return RBRACE;
                } else if (!strcmp(json_string_value(element_type), "slash")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return SLASH;
                } else if (!strcmp(json_string_value(element_type), "comma")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return COMMA;
                } else if (!strcmp(json_string_value(element_type), "int")) {
                    if (json_typeof(element_value) == JSON_INTEGER)
                        return INT;

                } else if (!strcmp(json_string_value(element_type), "plus")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return PLUS;
                } else if (!strcmp(json_string_value(element_type), "and")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return AND;
                } else if (!strcmp(json_string_value(element_type), "assign")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return ASSIGN;
                } else if (!strcmp(json_string_value(element_type), "asm")) {
                    if (json_typeof(element_value) == JSON_STRING)
                        return ASM;
                } else if (!strcmp(json_string_value(element_type), "dec")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return DEC;
                } else if (!strcmp(json_string_value(element_type), "dot")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return DOT;
                } else if (!strcmp(json_string_value(element_type), "inc")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return INC;
                } else if (!strcmp(json_string_value(element_type), "l_ang_brack")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return L_ANG_BRACK;
                } else if (!strcmp(json_string_value(element_type), "r_ang_brack")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return R_ANG_BRACK;
                } else if (!strcmp(json_string_value(element_type), "mod")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return MOD;
                } else if (!strcmp(json_string_value(element_type), "or")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return OR;
                } else if (!strcmp(json_string_value(element_type), "paragraph")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return PAR;
                } else if (!strcmp(json_string_value(element_type), "slash")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return SLASH;
                } else if (!strcmp(json_string_value(element_type), "star")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return STAR;
                } else if (!strcmp(json_string_value(element_type), "minus")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return MINUS;
                } else if (!strcmp(json_string_value(element_type), "weight")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return WEIGHT;
                } else if (!strcmp(json_string_value(element_type), "xor")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return XOR;
                } else if (!strcmp(json_string_value(element_type), "set_min")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return SET_MIN;
                } else if (!strcmp(json_string_value(element_type), "set_max")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return SET_MAX;
                } else if (!strcmp(json_string_value(element_type), "je")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return EQ;
                } else if (!strcmp(json_string_value(element_type), "jne")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return NEQ;
                } else if (!strcmp(json_string_value(element_type), "jle")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return LEQ;
                } else if (!strcmp(json_string_value(element_type), "jge")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return GEQ;
                } else if (!strcmp(json_string_value(element_type), "up_arrow")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return UP_ARROW;

                } else if (!strcmp(json_string_value(element_type), "jmp")) {
                    if (json_typeof(element_value) == JSON_NULL) 
                        return JUMP;
                } else if (!strcmp(json_string_value(element_type), "jz")) {
                    if (json_typeof(element_value) == JSON_NULL) 
                        return JUMPZ;
                } else if (!strcmp(json_string_value(element_type), "jnz")) {
                    if (json_typeof(element_value) == JSON_NULL) 
                        return JUMPNZ;
                } else if (!strcmp(json_string_value(element_type), "complex")) {
                    if (json_typeof(element_value) == JSON_STRING) {
                        if (!strcmp(json_string_value(element_value), "global"))
                            return G_COMP;
                        else if (!strcmp(json_string_value(element_value), "logic"))
                            return L_COMP;
                        else if (!strcmp(json_string_value(element_value), "symbol"))
                            return S_COMP;
                    }
                } else if (!strcmp(json_string_value(element_type), "fret")) {
                    if (json_typeof(element_value) == JSON_NULL)
                        return FRET;
                }
            }
        }
    }

    return 0;
}

void yyerror(const char *msg) {
    fprintf(stderr, "err: %s\n", msg);
    printf("program index: %d\n", program_index);
    json_t *program_element = json_array_get(program, program_index);
    print_json(program_element);
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
