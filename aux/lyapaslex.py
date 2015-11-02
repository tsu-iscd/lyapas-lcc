# -*- coding: utf-8 -*-

# ----------------------------------------------------------------------
# lyapaslex.py
#
# A lexer for LYaPAS.
# ----------------------------------------------------------------------

import sys
import ply.lex as lex

sys.path.insert(0,"../..")

tokens = (
    # Literals (identifier, integer constant, string constant)
    'ID', 'FNAME', 'ICONST', 'SCONST',
    'TUPLE', 'ONE', 'CAPACITY', 'CARDINALITY',
    'ASMINST', 'FRET', 'FCALL',

    # Operators
    'PLUS', 'MINUS', 'WEIGHT', 'LSHIFT',
    'RSHIFT', 'TIMES',
    'XOR', 'OR', 'NOT',

    # Increment/decrement
    'INC', 'DEC',

    # Conditional operator
    'CONDOP', 'COND',

    # Assignments
    'ASSIGN', 'EXCHANGE', 'MAX',

    # Labels
    'LABEL', 'GOTO',
    'ZEROGO', 'NONZERO',
    'CONDGO', 'FAULTGO',
    'ENUM',
    'EXIT', 'RETURN',

    # Delimeters ( ) [ ] ' ' ,
    'LPAREN', 'RPAREN',
    'LBRACKET', 'RBRACKET',
    'QUOTE', 'COMMA',
)

literals = [
    ':',';', '!', '&', '/',
    '@', '>', '<', 'c', 'C',
    'X', 'O', 'Z', 'T'
]

# Literals
t_ID = r'[a-z^c]'
t_TUPLE = r'[LF]\d+'
t_CAPACITY = r'[S]\d+'
t_CARDINALITY = r'[Q]\d+'
t_ICONST = r'\d+[A-F]*[hb]*'
t_SCONST = '\'([^\\\n]|(\\.))*?\''
t_ONE = 'I'
t_FRET = r'\*{2}'

def t_FNAME(t):
    r'[a-zA-Z_][a-zA-Z0-9_]*\('
    t.value = t.value[:-1]
    return t

# Operators
t_PLUS = r'\+'
t_MINUS = r'\-'
t_WEIGHT = r'%'
t_TIMES = r'\*'
t_XOR = ur'⊕'
t_OR = ur'∨'
t_NOT = ur'¬'
t_ASMINST = r'\{[^\}]*\}'

def t_FCALL(t):
    r'\*[a-zA-Z_][a-zA-Z0-9_]*\('
    t.value = t.value[1:-1]
    return t

# Increment/decrement
t_INC = ur'∆'
t_DEC = ur'∇'

# Conditional operator (?)
def t_CONDOP(t):
    r'\?\('
    t.value = t.value[:-1]
    return t

t_COND = ur'[=≠≥≤]'

# Assignments
t_ASSIGN = ur'⇒'
t_EXCHANGE = ur'⇔'
t_MAX = ur'⁻'

# Labels
t_LABEL = ur'§\d+'
t_GOTO = ur'→'
t_ZEROGO = ur'↪'
t_NONZERO = ur'↦'
t_FAULTGO = ur'↑>'
t_ENUM = ur'↑'
t_EXIT = ur'↣'
t_RETURN = ur'↢'

def t_CONDGO(t):
    ur'↑\('
    t.value = t.value[:-1]
    return t

# Delimeters  ( ) [ ] { } ' , .
t_LPAREN = r'\('
t_RPAREN = r'\)'
t_LBRACKET = r'\['
t_RBRACKET = r'\]'
t_QUOTE = r'\''
t_COMMA = r','

t_ignore = ' \t'

def t_comment(t):
    r'[*]{3}[^\n]*'
    pass

def t_newline(t):
    r'\n+'
    t.lexer.lineno += t.value.count('\n')

def t_error(t):
    print "Illegal character '%s'" % t.value[0]
    t.lexer.skip(1)

# Build the lexer
lex.lex()

data = u'''
prikrapok(L1,q/L2)
   Oi
§1 *sokramat(L1,q/L1)
§2 *minsto(L1,q/d) *maxstro(L1,d,q/a)
   L1a⇒L2i ∆i L1a¬&q⇒q↦1 i⇒Q2 **

maxstro(L1,d,q/a)
   Oa Ob*Oi
§1 L1i&Id↪2 L1i&q%⇒c ↑(c≤b)1 i⇒a c⇒b
§2 ∆i⊕Q1↦1 **

minsto(L1,q/d)
   ⁻b
§1 ↑X4qj Ok Os 	*** j — текущий столбец, s — его вес
§2 L1i&Ij↪3  ∆s
§3 ∆i⊕Q1↦2 ↑(s≥b)1 s⇒b j⇒d→1
§4 **

sokramat(L1,q/L1)
   Oc ⁻b
§1 ∆b⊕Q1↪4 b⇒i
§2 ∆i⊕Q1↪3 L1i¬&L1b&q↦2 L1i⊕L1b&q↦1 →2
§3 L1b⇒L1c ∆c→1
§4 c⇒Q1 **
'''

lex.input(data)

# Tokenize
while True:
    tok = lex.token()
    if not tok: break
    print u'%s' % tok.type + u'\u0009' + tok.value