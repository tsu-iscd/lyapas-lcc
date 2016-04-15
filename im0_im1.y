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
%token S_COMP L_COMP G_COMP COMP_CAP COMP_CARD INT ID SCONST SET_BIT SPEC_VAR FRET LPAR RPAR INC DEC PLUS MINUS PERCENT MOD DDIV OR AND XOR LEAST_BIT CONSOLE L_ANG_BRACK R_ANG_BRACK SLASH LETTER_X STAR UP_ARROW SET_MIN SET_MAX ASSIGN SWAP TIME PAR EQ NEQ GEQ LEQ JUMP JUMPZ JUMPNZ LBRACK RBRACK QUOTE COMMA DOT LBRACE RBRACE ASM NOT AT_SIGN SHARP 

%%

prog:
    procs {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("program"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_extend($$, $1);
        result = $$;
    }
;

procs:
    proc procs {
        $$ = json_array();
        json_array_append($$, $1);
        json_array_extend($$, $2);
    }
    | proc {
        $$ = json_array();
        json_array_append($$, $1);
    }
;

proc:
    ID header body FRET {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("procedure"));
        json_object_set(root, "name", $1);

        $$ = json_array();
        json_array_append($$, root);
        json_array_extend($$, $2);
        json_array_extend($$, $3);
    }
    | ID header FRET {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("procedure"));
        json_object_set(root, "name", $1);

        $$ = json_array();
        json_array_append($$, root);
        json_array_extend($$, $2);
    }
;

header:
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
        $$ = json_array();
        json_array_append($$, $1);
        json_array_extend($$, $2);
    }
    | preface {
        $$ = json_array();
        json_array_append($$, $1);
    }
    | paragraphs {
        $$ = $1;
    }
;

preface:
    expressions {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("preface"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_extend($$, $1);
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
        json_array_extend($$, $3);
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
        $$ = $1;
        json_array_extend($$, $2);
    }
    | expression {
        $$ = $1;
    }
;

expression:
    operation {
        $$ = json_array();
        json_array_append($$, $1);
    }
    | jump_cond {
        $$ = $1;
    }
    | comp_op {
        $$ = json_array();
        json_array_append($$, $1);
    }
    | console_op {
        $$ = json_array();
        json_array_append($$, $1);
    }
;

operation:  
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
    | NOT {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("not"));

        $$ = json_array();
        json_array_append($$, root);
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
    | PERCENT {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("weight"));

        $$ = json_array();
        json_array_append($$, root);
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
    | SWAP LPAR ID ID RPAR {
        json_t *arg1_root = json_object();
        json_object_set(arg1_root, "type", json_string("var"));
        json_object_set(arg1_root, "name", $3);

        json_t *arg1 = json_array();
        json_array_append(arg1, arg1_root);

        json_t *arg2_root = json_object();
        json_object_set(arg2_root, "type", json_string("var"));
        json_object_set(arg2_root, "name", $4);

        json_t *arg2 = json_array();
        json_array_append(arg2, arg2_root);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("swap"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, arg1);
        json_array_append($$, arg2);
    }
    | SWAP LPAR comp ID ID RPAR {
        json_t *arg1_root = json_object();
        json_object_set(arg1_root, "type", json_string("var"));
        json_object_set(arg1_root, "name", $4);

        json_t *arg1 = json_array();
        json_array_append(arg1, arg1_root);

        json_t *arg2_root = json_object();
        json_object_set(arg2_root, "type", json_string("var"));
        json_object_set(arg2_root, "name", $5);

        json_t *arg2 = json_array();
        json_array_append(arg2, arg2_root);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("swap_comp_el"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $3);
        json_array_append($$, arg1);
        json_array_append($$, arg2);
    }
    | SWAP LPAR comp ID INT RPAR {
        json_t *arg1_root = json_object();
        json_object_set(arg1_root, "type", json_string("var"));
        json_object_set(arg1_root, "name", $4);

        json_t *arg1 = json_array();
        json_array_append(arg1, arg1_root);

        json_t *arg2_root = json_object();
        json_object_set(arg2_root, "type", json_string("const"));
        json_object_set(arg2_root, "value", $5);

        json_t *arg2 = json_array();
        json_array_append(arg2, arg2_root);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("swap_comp_el"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $3);
        json_array_append($$, arg1);
        json_array_append($$, arg2);
    }
    | SWAP LPAR comp DOT INT ID RPAR {
        json_t *arg1_root = json_object();
        json_object_set(arg1_root, "type", json_string("const"));
        json_object_set(arg1_root, "value", $5);

        json_t *arg1 = json_array();
        json_array_append(arg1, arg1_root);

        json_t *arg2_root = json_object();
        json_object_set(arg2_root, "type", json_string("var"));
        json_object_set(arg2_root, "name", $6);

        json_t *arg2 = json_array();
        json_array_append(arg2, arg2_root);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("swap_comp_el"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $3);
        json_array_append($$, arg1);
        json_array_append($$, arg2);
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
    | UP_ARROW R_ANG_BRACK {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("jump_by_deny"));

        $$ = json_array();
        json_array_append($$, root);
    }

    | arg {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("load"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $1);
    }
    | LETTER_X {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("get_random"));

        $$ = json_array();
        json_array_append($$, root);
    }
    | ASSIGN LETTER_X {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("set_random"));

        $$ = json_array();
        json_array_append($$, root);
    }
    | TIME {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("get_time"));

        $$ = json_array();
        json_array_append($$, root);
    }
    | LEAST_BIT {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("get_number_least_1"));

        $$ = json_array();
        json_array_append($$, root);
    }
    | UP_ARROW LETTER_X {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("enumeration_1"));

        $$ = json_array();
        json_array_append($$, root);
    }
    | LBRACE ASM RBRACE {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("asm"));
        json_object_set(root, "command", $2);

        $$ = json_array();
        json_array_append($$, root);
    }
    | STAR ID header {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("call"));
        json_object_set(root, "name", $2);

        $$ = json_array();
        json_array_append($$, root);
        json_array_extend($$, $3);
    }
;

comp_op:
    AT_SIGN PLUS comp LPAR arg RPAR {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("create_complex"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $3);
        json_array_append($$, $5);
    }
    | AT_SIGN MINUS comp {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("remove_complex"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $3);
    }
    | AT_SIGN PERCENT comp {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("reduce_complex"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $3);
    }
    | AT_SIGN SET_MIN comp {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("clear_complex"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $3);
    }
    | AT_SIGN QUOTE SCONST QUOTE R_ANG_BRACK comp {
        json_t *str_root = json_object();
        json_object_set(str_root, "type", json_string("string"));
        json_object_set(str_root, "value", $3);

        json_t *str = json_array();
        json_array_append(str, str_root);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("insert_string_in_complex"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $6);
        json_array_append($$, str);
    }
    | AT_SIGN R_ANG_BRACK comp {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("insert_element_in_complex"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $3);
    }
    | AT_SIGN R_ANG_BRACK comp_el {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("insert_element_in_complex"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $3);
    }
    | AT_SIGN L_ANG_BRACK comp {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("remove_element_from_complex"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $3);
    }
    | AT_SIGN L_ANG_BRACK comp_el {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("remove_element_from_complex"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $3);
    }
    | AT_SIGN SHARP comp comp LPAR arg COMMA arg COMMA arg RPAR {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("copy_complex"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $3);
        json_array_append($$, $4);
        json_array_append($$, $6);
        json_array_append($$, $8);
        json_array_append($$, $10);
    }
;

console_op:
    SLASH comp R_ANG_BRACK CONSOLE {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("write_complex"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | SLASH comp L_ANG_BRACK CONSOLE {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("read_complex"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
    }
    | SLASH QUOTE SCONST QUOTE R_ANG_BRACK CONSOLE {
        json_t *str_root = json_object();
        json_object_set(str_root, "type", json_string("string"));
        json_object_set(str_root, "value", $3);

        json_t *str = json_array();
        json_array_append(str, str_root);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("write_string"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, str);
    }
    | SLASH comp R_ANG_BRACK CONSOLE INT LBRACK QUOTE SCONST QUOTE RBRACK {
        json_t *str_root = json_object();
        json_object_set(str_root, "type", json_string("string"));
        json_object_set(str_root, "value", $8);

        json_t *str = json_array();
        json_array_append(str, str_root);

        json_t *num_root = json_object();
        json_object_set(num_root, "type", json_string("const"));
        json_object_set(num_root, "value", $5);

        json_t *num = json_array();
        json_array_append(num, num_root);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("write_archive"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
        json_array_append($$, num);
        json_array_append($$, str);
    }
    | SLASH comp R_ANG_BRACK CONSOLE INT LBRACK comp RBRACK {
        json_t *num_root = json_object();
        json_object_set(num_root, "type", json_string("const"));
        json_object_set(num_root, "value", $5);

        json_t *num = json_array();
        json_array_append(num, num_root);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("write_archive"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
        json_array_append($$, num);
        json_array_append($$, $7);
    }
    | SLASH comp L_ANG_BRACK CONSOLE INT LBRACK QUOTE SCONST QUOTE RBRACK {
        json_t *str_root = json_object();
        json_object_set(str_root, "type", json_string("string"));
        json_object_set(str_root, "value", $8);

        json_t *str = json_array();
        json_array_append(str, str_root);

        json_t *num_root = json_object();
        json_object_set(num_root, "type", json_string("const"));
        json_object_set(num_root, "value", $5);

        json_t *num = json_array();
        json_array_append(num, num_root);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("read_archive"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
        json_array_append($$, num);
        json_array_append($$, str);
    }
    | SLASH comp L_ANG_BRACK CONSOLE INT LBRACK comp RBRACK {
        json_t *num_root = json_object();
        json_object_set(num_root, "type", json_string("const"));
        json_object_set(num_root, "value", $5);

        json_t *num = json_array();
        json_array_append(num, num_root);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("operation"));
        json_object_set(root, "name", json_string("read_archive"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, $2);
        json_array_append($$, num);
        json_array_append($$, $7);
    }
;

jump_cond:
    UP_ARROW LPAR arg EQ arg RPAR INT {
        json_t *load_root = json_object();
        json_object_set(load_root, "type", json_string("operation"));
        json_object_set(load_root, "name", json_string("load"));

        json_t *load = json_array();
        json_array_append(load, load_root);
        json_array_append(load, $3);

        json_t *cmp_root = json_object();
        json_object_set(cmp_root, "type", json_string("operation"));
        json_object_set(cmp_root, "name", json_string("compare"));

        json_t *cmp = json_array();
        json_array_append(cmp, cmp_root);
        json_array_append(cmp, $5);

        json_t *dst_root = json_object();
        json_object_set(dst_root, "type", json_string("const"));
        json_object_set(dst_root, "value", $7);

        json_t *dst = json_array();
        json_array_append(dst, dst_root);

        json_t *jmp_root = json_object();
        json_object_set(jmp_root, "type", json_string("operation"));
        json_object_set(jmp_root, "name", json_string("jump_eq"));

        json_t *jmp = json_array();
        json_array_append(jmp, jmp_root);
        json_array_append(jmp, dst);

        $$ = json_array();
        json_array_append($$, load);
        json_array_append($$, cmp);
        json_array_append($$, jmp);
    }
    | UP_ARROW LPAR arg NEQ arg RPAR INT {
        json_t *load_root = json_object();
        json_object_set(load_root, "type", json_string("operation"));
        json_object_set(load_root, "name", json_string("load"));

        json_t *load = json_array();
        json_array_append(load, load_root);
        json_array_append(load, $3);

        json_t *cmp_root = json_object();
        json_object_set(cmp_root, "type", json_string("operation"));
        json_object_set(cmp_root, "name", json_string("compare"));

        json_t *cmp = json_array();
        json_array_append(cmp, cmp_root);
        json_array_append(cmp, $5);

        json_t *dst_root = json_object();
        json_object_set(dst_root, "type", json_string("const"));
        json_object_set(dst_root, "value", $7);

        json_t *dst = json_array();
        json_array_append(dst, dst_root);

        json_t *jmp_root = json_object();
        json_object_set(jmp_root, "type", json_string("operation"));
        json_object_set(jmp_root, "name", json_string("jump_neq"));

        json_t *jmp = json_array();
        json_array_append(jmp, jmp_root);
        json_array_append(jmp, dst);

        $$ = json_array();
        json_array_append($$, load);
        json_array_append($$, cmp);
        json_array_append($$, jmp);
    }
    | UP_ARROW LPAR arg LEQ arg RPAR INT {
        json_t *load_root = json_object();
        json_object_set(load_root, "type", json_string("operation"));
        json_object_set(load_root, "name", json_string("load"));

        json_t *load = json_array();
        json_array_append(load, load_root);
        json_array_append(load, $3);

        json_t *cmp_root = json_object();
        json_object_set(cmp_root, "type", json_string("operation"));
        json_object_set(cmp_root, "name", json_string("compare"));

        json_t *cmp = json_array();
        json_array_append(cmp, cmp_root);
        json_array_append(cmp, $5);

        json_t *dst_root = json_object();
        json_object_set(dst_root, "type", json_string("const"));
        json_object_set(dst_root, "value", $7);

        json_t *dst = json_array();
        json_array_append(dst, dst_root);

        json_t *jmp_root = json_object();
        json_object_set(jmp_root, "type", json_string("operation"));
        json_object_set(jmp_root, "name", json_string("jump_leq"));

        json_t *jmp = json_array();
        json_array_append(jmp, jmp_root);
        json_array_append(jmp, dst);

        $$ = json_array();
        json_array_append($$, load);
        json_array_append($$, cmp);
        json_array_append($$, jmp);
    }
    | UP_ARROW LPAR arg GEQ arg RPAR INT {
        json_t *load_root = json_object();
        json_object_set(load_root, "type", json_string("operation"));
        json_object_set(load_root, "name", json_string("load"));

        json_t *load = json_array();
        json_array_append(load, load_root);
        json_array_append(load, $3);

        json_t *cmp_root = json_object();
        json_object_set(cmp_root, "type", json_string("operation"));
        json_object_set(cmp_root, "name", json_string("compare"));

        json_t *cmp = json_array();
        json_array_append(cmp, cmp_root);
        json_array_append(cmp, $5);

        json_t *dst_root = json_object();
        json_object_set(dst_root, "type", json_string("const"));
        json_object_set(dst_root, "value", $7);

        json_t *dst = json_array();
        json_array_append(dst, dst_root);

        json_t *jmp_root = json_object();
        json_object_set(jmp_root, "type", json_string("operation"));
        json_object_set(jmp_root, "name", json_string("jump_geq"));

        json_t *jmp = json_array();
        json_array_append(jmp, jmp_root);
        json_array_append(jmp, dst);

        $$ = json_array();
        json_array_append($$, load);
        json_array_append($$, cmp);
        json_array_append($$, jmp);
    }
    | UP_ARROW LPAR arg L_ANG_BRACK arg RPAR INT {
        json_t *load_root = json_object();
        json_object_set(load_root, "type", json_string("operation"));
        json_object_set(load_root, "name", json_string("load"));

        json_t *load = json_array();
        json_array_append(load, load_root);
        json_array_append(load, $3);

        json_t *cmp_root = json_object();
        json_object_set(cmp_root, "type", json_string("operation"));
        json_object_set(cmp_root, "name", json_string("compare"));

        json_t *cmp = json_array();
        json_array_append(cmp, cmp_root);
        json_array_append(cmp, $5);

        json_t *dst_root = json_object();
        json_object_set(dst_root, "type", json_string("const"));
        json_object_set(dst_root, "value", $7);

        json_t *dst = json_array();
        json_array_append(dst, dst_root);

        json_t *jmp_root = json_object();
        json_object_set(jmp_root, "type", json_string("operation"));
        json_object_set(jmp_root, "name", json_string("jump_lt"));

        json_t *jmp = json_array();
        json_array_append(jmp, jmp_root);
        json_array_append(jmp, dst);

        $$ = json_array();
        json_array_append($$, load);
        json_array_append($$, cmp);
        json_array_append($$, jmp);
    }
    | UP_ARROW LPAR arg R_ANG_BRACK arg RPAR INT {
        json_t *load_root = json_object();
        json_object_set(load_root, "type", json_string("operation"));
        json_object_set(load_root, "name", json_string("load"));

        json_t *load = json_array();
        json_array_append(load, load_root);
        json_array_append(load, $3);

        json_t *cmp_root = json_object();
        json_object_set(cmp_root, "type", json_string("operation"));
        json_object_set(cmp_root, "name", json_string("compare"));

        json_t *cmp = json_array();
        json_array_append(cmp, cmp_root);
        json_array_append(cmp, $5);

        json_t *dst_root = json_object();
        json_object_set(dst_root, "type", json_string("const"));
        json_object_set(dst_root, "value", $7);

        json_t *dst = json_array();
        json_array_append(dst, dst_root);

        json_t *jmp_root = json_object();
        json_object_set(jmp_root, "type", json_string("operation"));
        json_object_set(jmp_root, "name", json_string("jump_gt"));

        json_t *jmp = json_array();
        json_array_append(jmp, jmp_root);
        json_array_append(jmp, dst);

        $$ = json_array();
        json_array_append($$, load);
        json_array_append($$, cmp);
        json_array_append($$, jmp);
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
        $$ = $1;
    }
    | COMP_CAP INT {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("complex_capacity"));
        json_object_set(root, "number", $2);

        $$ = json_array();
        json_array_append($$, root);
    }
    | COMP_CARD INT {
        json_t *root = json_object();
        json_object_set(root, "type", json_string("complex_cardinality"));
        json_object_set(root, "number", $2);

        $$ = json_array();
        json_array_append($$, root);
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
        json_t *index_root = json_object();
        json_object_set(index_root, "type", json_string("var"));
        json_object_set(index_root, "name", $2);

        json_t *index = json_array();
        json_array_append(index, index_root);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("global_complex_1"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, index);
    }
    | G_COMP INT {
        json_t *index_root = json_object();
        json_object_set(index_root, "type", json_string("const"));
        json_object_set(index_root, "value", $2);

        json_t *index = json_array();
        json_array_append(index, index_root);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("global_complex_1"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, index);
    }
    | G_COMP LBRACK ID RBRACK {
        json_t *index_root = json_object();
        json_object_set(index_root, "type", json_string("var"));
        json_object_set(index_root, "name", $3);

        json_t *index = json_array();
        json_array_append(index, index_root);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("global_complex_4"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, index);
    }
    | G_COMP LBRACK INT RBRACK {
        json_t *index_root = json_object();
        json_object_set(index_root, "type", json_string("const"));
        json_object_set(index_root, "value", $3);

        json_t *index = json_array();
        json_array_append(index, index_root);

        json_t *root = json_object();
        json_object_set(root, "type", json_string("global_complex_4"));

        $$ = json_array();
        json_array_append($$, root);
        json_array_append($$, index);
    }
    | comp DOT INT {
        json_t *index_root = json_object();
        json_object_set(index_root, "type", json_string("const"));
        json_object_set(index_root, "value", $3);

        json_t *index = json_array();
        json_array_append(index, index_root);

        $$ = $1;
        json_array_append($$, index);
    }
    | comp ID {
        json_t *index_root = json_object();
        json_object_set(index_root, "type", json_string("var"));
        json_object_set(index_root, "name", $2);

        json_t *index = json_array();
        json_array_append(index, index_root);

        $$ = $1;
        json_array_append($$, index);
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
                //
                    if (!strcmp(json_string_value(element_type), "id")) {
                        if (json_typeof(element_value) == JSON_STRING)
                            return ID;
                    } else if (!strcmp(json_string_value(element_type), "int")) {
                        if (json_typeof(element_value) == JSON_INTEGER)
                            return INT;
                    } else if (!strcmp(json_string_value(element_type), "paragraph")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return PAR;
                    } else if (!strcmp(json_string_value(element_type), "sconst")) {
                        if (json_typeof(element_value) == JSON_STRING)
                            return SCONST;
                    } else if (!strcmp(json_string_value(element_type), "spec_var")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return SPEC_VAR;
                    } else if (!strcmp(json_string_value(element_type), "asm")) {
                        if (json_typeof(element_value) == JSON_STRING)
                            return ASM;
                // ( ) [ ] { } < >
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
                    } else if (!strcmp(json_string_value(element_type), "l_ang_brack")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return L_ANG_BRACK;
                    } else if (!strcmp(json_string_value(element_type), "r_ang_brack")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return R_ANG_BRACK;
                // = + - * /
                    } else if (!strcmp(json_string_value(element_type), "assign")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return ASSIGN;

                    } else if (!strcmp(json_string_value(element_type), "plus")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return PLUS;
                    } else if (!strcmp(json_string_value(element_type), "minus")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return MINUS;
                    } else if (!strcmp(json_string_value(element_type), "inc")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return INC;
                    } else if (!strcmp(json_string_value(element_type), "dec")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return DEC;

                    } else if (!strcmp(json_string_value(element_type), "star")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return STAR;
                    } else if (!strcmp(json_string_value(element_type), "slash")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return SLASH;
                    } else if (!strcmp(json_string_value(element_type), "mod")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return MOD;
                    } else if (!strcmp(json_string_value(element_type), "div")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return DDIV;

                    } else if (!strcmp(json_string_value(element_type), "and")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return AND;
                    } else if (!strcmp(json_string_value(element_type), "or")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return OR;
                    } else if (!strcmp(json_string_value(element_type), "xor")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return XOR;
                    } else if (!strcmp(json_string_value(element_type), "not")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return NOT;
                // jump
                    } else if (!strcmp(json_string_value(element_type), "jmp")) {
                        if (json_typeof(element_value) == JSON_NULL) 
                            return JUMP;
                    } else if (!strcmp(json_string_value(element_type), "jz")) {
                        if (json_typeof(element_value) == JSON_NULL) 
                            return JUMPZ;
                    } else if (!strcmp(json_string_value(element_type), "jnz")) {
                        if (json_typeof(element_value) == JSON_NULL) 
                            return JUMPNZ;
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
                // . , % @ # '
                    } else if (!strcmp(json_string_value(element_type), "dot")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return DOT;
                    } else if (!strcmp(json_string_value(element_type), "comma")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return COMMA;
                    } else if (!strcmp(json_string_value(element_type), "percent")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return PERCENT;
                    } else if (!strcmp(json_string_value(element_type), "at_sign")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return AT_SIGN;
                    } else if (!strcmp(json_string_value(element_type), "copy")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return SHARP;
                    } else if (!strcmp(json_string_value(element_type), "quote")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return QUOTE;
                //
                    } else if (!strcmp(json_string_value(element_type), "set_min")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return SET_MIN;
                    } else if (!strcmp(json_string_value(element_type), "set_max")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return SET_MAX;
                    } else if (!strcmp(json_string_value(element_type), "set_bit")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return SET_BIT;
                    } else if (!strcmp(json_string_value(element_type), "num_right_one")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return LEAST_BIT;
                    } else if (!strcmp(json_string_value(element_type), "console")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return CONSOLE;
                    } else if (!strcmp(json_string_value(element_type), "letter_X")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return LETTER_X;
                    } else if (!strcmp(json_string_value(element_type), "swap")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return SWAP;
                    } else if (!strcmp(json_string_value(element_type), "get_time")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return TIME;
                    } else if (!strcmp(json_string_value(element_type), "up_arrow")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return UP_ARROW;
                    } else if (!strcmp(json_string_value(element_type), "fret")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return FRET;
                // complex
                    } else if (!strcmp(json_string_value(element_type), "complex")) {
                        if (json_typeof(element_value) == JSON_STRING) {
                            if (!strcmp(json_string_value(element_value), "global"))
                                return G_COMP;
                            else if (!strcmp(json_string_value(element_value), "logic"))
                                return L_COMP;
                            else if (!strcmp(json_string_value(element_value), "symbol"))
                                return S_COMP;
                        }
                    } else if (!strcmp(json_string_value(element_type), "capacity")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return COMP_CAP;
                    } else if (!strcmp(json_string_value(element_type), "cardinality")) {
                        if (json_typeof(element_value) == JSON_NULL)
                            return COMP_CARD;
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
