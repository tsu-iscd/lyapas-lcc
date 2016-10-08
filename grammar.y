%token S_COMP L_COMP G_COMP COMP_CAP COMP_CARD INT ID SCONST SET_BIT SPEC_VAR FRET LPAR RPAR INC DEC PLUS MINUS PERCENT MOD DDIV OR AND XOR LEAST_BIT CONSOLE L_ANG_BRACK R_ANG_BRACK SLASH LETTER_X STAR UP_ARROW SET_MIN SET_MAX ASSIGN SWAP TIME PAR EQ NEQ GEQ LEQ JUMP JUMPZ JUMPNZ LBRACK RBRACK QUOTE COMMA DOT LBRACE RBRACE ASM NOT AT_SIGN SHARP 

%%
prog:
    procs {}

procs:
    proc procs {}
    | proc {}

proc:
    ID header body FRET {}
    | ID header FRET {}

header:
    LPAR proc_args SLASH proc_args RPAR {}
    | LPAR proc_args SLASH RPAR {}
    | LPAR SLASH proc_args RPAR {}
    | LPAR SLASH RPAR {}

proc_args:
    proc_arg COMMA proc_args {}
    | proc_arg {}

proc_arg:
    arg {}
    | comp {}

body:
    preface paragraphs {}
    | preface {}
    | paragraphs {}

preface:
    expressions {}

paragraphs:
    paragraph paragraphs {}
    | paragraph {}

paragraph:
    PAR INT expressions {}
    | PAR INT {}

expressions:
    expression expressions {}
    | expression {}

expression:
    operation {}
    | jump_cond {}
    | comp_op {}
    | console_op {}

value_transfer_op:
    SET_MIN arg {}
    | SET_MAX arg {}
    | ASSIGN arg {}
    | arg {}
    | SWAP LPAR ID ID RPAR {}
    | SWAP LPAR comp ID ID RPAR {}
    | SWAP LPAR comp ID INT RPAR {}
    | SWAP LPAR comp DOT INT ID RPAR {}
    | SWAP LPAR comp DOT INT DOT INT RPAR {}
    | LETTER_X {}
    | ASSIGN LETTER_X {}
    | TIME {}

logic_arithm_op:
    LEAST_BIT {}
    | NOT {}
    | PERCENT {}
    | OR arg {}
    | AND arg {}
    | XOR arg {}
    | L_ANG_BRACK arg {}
    | R_ANG_BRACK arg {}
    | PLUS arg {}
    | MINUS arg {}
    | STAR arg {}
    | DDIV arg {}
    | SLASH arg {}
    | MOD arg {}
    | INC arg {}
    | DEC arg {}

jump_op:
    JUMP INT {}
    | JUMPZ INT {}
    | JUMPNZ INT {}
    | UP_ARROW LPAR arg RPAR {}
    | UP_ARROW LETTER_X INT arg arg {}
    | UP_ARROW R_ANG_BRACK {}
    | LBRACE ASM RBRACE {}

jump_cond:
    UP_ARROW LPAR arg EQ arg RPAR INT {}
    | UP_ARROW LPAR arg NEQ arg RPAR INT {}
    | UP_ARROW LPAR arg LEQ arg RPAR INT {}
    | UP_ARROW LPAR arg GEQ arg RPAR INT {}
    | UP_ARROW LPAR arg L_ANG_BRACK arg RPAR INT {}
    | UP_ARROW LPAR arg R_ANG_BRACK arg RPAR INT {}

call_op:
    STAR ID header {}

operation:  
    value_transfer_op {}
    | logic_arithm_op {}
    | jump_op {}
    | call_op {}

comp_op:
    AT_SIGN PLUS comp LPAR arg RPAR {}
    | AT_SIGN MINUS comp {}
    | AT_SIGN PERCENT comp {}
    | SET_MIN comp {}
    | AT_SIGN QUOTE SCONST QUOTE R_ANG_BRACK comp {}
    | AT_SIGN R_ANG_BRACK comp {}
    | AT_SIGN R_ANG_BRACK comp_el {}
    | AT_SIGN L_ANG_BRACK comp {}
    | AT_SIGN L_ANG_BRACK comp_el {}
    | AT_SIGN SHARP comp comp LPAR arg COMMA arg COMMA arg RPAR {}

console_op:
    SLASH comp R_ANG_BRACK CONSOLE {}
    | SLASH comp L_ANG_BRACK CONSOLE {}
    | SLASH QUOTE SCONST QUOTE R_ANG_BRACK CONSOLE {}
    | SLASH comp R_ANG_BRACK CONSOLE INT LBRACK QUOTE SCONST QUOTE RBRACK {}
    | SLASH comp R_ANG_BRACK CONSOLE INT LBRACK comp RBRACK {}
    | SLASH comp L_ANG_BRACK CONSOLE INT LBRACK QUOTE SCONST QUOTE RBRACK {}
    | SLASH comp L_ANG_BRACK CONSOLE INT LBRACK comp RBRACK {}

arg:
    INT {}
    | comp_el {}
    | ID {}
    | COMP_CAP INT {}
    | COMP_CARD INT {}

comp:
    L_COMP INT {}
    | S_COMP INT {}

comp_el:
    G_COMP ID {}
    | G_COMP INT {}
    | G_COMP LBRACK ID RBRACK {}
    | G_COMP LBRACK INT RBRACK {}
    | comp DOT INT {}
    | comp ID {}
;
%%
