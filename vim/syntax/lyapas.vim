if exists('b:current_syntax') | finish|  endif

syntax match LYAPAS_vars /[a-z]/
syntax match LYAPAS_ddigit /[0-9]/
syntax match LYAPAS_sting /(\\.^\\|[^\\'\n]|''|\\\\)+/
syntax match LYAPAS_complex /[FLK]/
syntax match LYAPAS_inc /∆/
