# -*- coding: utf-8 -*-

import sys
import lyapaslex
import ply.yacc as yacc

# Get the token map
tokens = lyapaslex.tokens

# Parsing rules

precedence = (
    ('left', 'INC', 'DEC', '!',
        'NOT', 'WEIGHT', '/', ';',
        ':', 'TIMES', 'PLUS', 'MINUS',
        'LSHIFT', 'RSHIFT', '&',
        'XOR', 'OR'),
    )

# dictionary of names
#names = { }

def p_write_to_console(p):
    '''term : "/" TUPLE ">" "C"
            | "/" ONE ">" "C"
            | "/" SCONST ">" "C"
    '''
    pass

def p_read_from_console(p):
    '''term : "/" TUPLE "<" "C"
    '''
    p[0] = p[2]

def p_assignment(p):
    '''term : "O" gamma
            | MAX gamma
            | ASSIGN gamma
    '''
    p[0] = p[2]

def p_exchange(p):
    '''term : EXCHANGE LPAREN ID ID RPAREN
            | EXCHANGE LPAREN TUPLE ID ID RPAREN
            | EXCHANGE LPAREN TUPLE ID ICONST RPAREN
            | EXCHANGE LPAREN TUPLE "." ICONST ID RPAREN
            | EXCHANGE LPAREN TUPLE "." ICONST "." ICONST RPAREN
    '''
    if len(p) == 6:
        p[0] = "IDs exchanged"
    else:
        p[0] = p[3]

def p_function(p):
    'function : head statements end'
    p[0] = p[1] # пока сомнительно


def p_head(p):
    '''head : FNAME parameters RPAREN
            | FNAME parameters "/" parameters RPAREN
    '''
    p[0] = p[1]

def p_parameters(p):
    '''parameters : parameters COMMA parameter
        | parameter'''
    if len(p) == 3:
        p[0] = p[1] + p[2]
    else:
        p[0] = p[1]

def p_parameter(p):
    '''parameter : ID
            | TUPLE'''
    p[0] = p[1]

def p_end(p):
    '''end : FRET
        | LABEL FRET'''
    pass

def p_statements(p):
    '''statements : LABEL expressions
                | expressions'''
    if len(p) == 3:
        p[0] = p[1] + p[2]
    else:
        p[0] = p[1]

def p_expressions(p):
    '''expressions : expression expressions
        | expression'''
    if len(p) == 3:
        p[0] = p[1] + p[2]
    else:
        p[0] = p[1]

def p_expression(p):
    '''expression : term
                | term expression
    '''

def p_conditional_overpass(p):
    '''term : CONDGO gamma COND gamma RPAREN ICONST
            | CONDGO gamma "<" gamma RPAREN ICONST
            | CONDGO gamma ">" gamma RPAREN ICONST
    '''

def p_conditional_operator(p):
    '''term : CONDOP gamma COND gamma RPAREN
            | CONDOP gamma "<" gamma RPAREN
            | CONDOP gamma ">" gamma RPAREN
    '''

def p_function_call(p):
    '''term : FCALL parameters RPAREN
            | FCALL parameters "/" parameters RPAREN
    '''

def p_overpass(p):
    ''' term : GOTO ICONST
            | ZEROGO ICONST
            | NONZERO ICONST
            | FAULTGO ICONST
            | ENUM LPAREN gamma RPAREN ICONST
            | EXIT ICONST
            | RETURN ICONST
    '''

def p_tao(p):
    '''term : alpha
            | gamma
            | term
    '''
    pass

def p_unary_operations(p):
    '''term : INC alpha
        | DEC alpha
        | "!"
        | NOT
        | WEIGHT
        | "/" gamma
        | ";" gamma
        | ":" gamma
        | TIMES gamma
        | PLUS gamma
        | MINUS gamma
        | LSHIFT gamma
        | RSHIFT gamma
        | "&" gamma
        | XOR gamma
        | OR gamma
    '''
    pass

def p_asminstruction(p):
    'expression : ASMINST'
    p[0] = 'CALL'

def p_term_gamma(p):
    '''gamma : ID
            | ICONST
            | element
            | CAPACITY
            | CARDINALITY
            | ONE
    '''
    p[0] = p[1]

def p_term_alpha(p):
    '''alpha : ID
        | element
    '''
    pass

# элемент комплекса
def p_tuple_element(p):
    '''element : TUPLE "." ICONST
                | TUPLE ID
                | TUPLE index
    '''
    p[0] = 'element'

def p_tuple_index(p):
    '''index : LPAREN term PLUS term RPAREN
                | LPAREN term MINUS term RPAREN
    '''
    p[0] = "(index)"

def p_error(p):
    print "Syntax error at '%s'" % p.value

yacc.yacc(debug=True)