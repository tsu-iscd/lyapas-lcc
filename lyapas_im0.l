%{
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

# define INT "int"
# define ID "id"
# define COMPLEX "complex"
typedef unsigned long long int uint64_t;
int first_time = 1;

void PutComma() 
{
    if (!first_time) 
    {   
        printf(",");
    } 
    else 
    {
        first_time = 0;
    }
}
uint64_t BinToInt(const char *bin)
{   
    int i = 0;
    uint64_t answ = 0;
    int digit = 0;
    uint64_t pow2 = 0;
    int len = strlen(bin);
    if(len > 1 && len <= 32 && bin[len - 1] == 'b')
    {
        len--;
    }
    else
    {
        puts("Неправильный формат 2-ого числа:");
        puts("Используйте b в конце.");
        puts("число должно быть не более 31 цифры");
        return -1;
    }

    while(i < len)
    {
        if(bin[i] >= '0' && bin[i] <= '1')
        {
            digit = bin[i] - '0';
        }
        else
        {
            puts("Неправильный формат 2-ого числа:");
            puts("используйте только цифры 0 и 1");
            return 0;
        }
        
        pow2 = ((uint64_t)1 << (2 * (len - 1 - i)));
        answ += (digit * pow2);
        i++;
    }
    return answ;
}
uint64_t OctToInt(const char *oct)
{   
    int i = 0;
    uint64_t answ = 0;
    int digit = 0;
    uint64_t pow8 = 0;
    int len = strlen(oct);
    if(len > 1 && len <= 22 && oct[len - 1] == 'o')
    {
        len--;
    }
    else
    {
        puts("Неправильный формат 8-ого числа:");
        puts("Используйте o в конце.");
        puts("число должно быть не более 21 цифры");
        return -1;
    }

    while(i < len)
    {
        if(oct[i] >= '0' && oct[i] <= '7')
        {
            digit = oct[i] - '0';
        }
        else
        {
            puts("Неправильный формат 8-ого числа:");
            puts("используйте только цифры 0-7");
            return 0;
        }
        
        pow8 = ((uint64_t)1 << (3 * (len - 1 - i)));
        answ += (digit * pow8);
        i++;
    }
    return answ;
}
uint64_t HexToInt(const char *hex)
{   
    int i = 0;
    uint64_t answ = 0;
    int digit = 0;
    uint64_t pow16 = 0;
    int len = strlen(hex);

    if(len > 1 && len <= 16 && hex[len - 1] == 'h')
    {
        len--;
    }
    else
    {
        puts("Неправильный формат 16-ого числа:");
        puts("Используйте h в конце.");
        puts("число должно быть не более 15 цифр");
        return -1;
    }
    
    while(i < len)
    {
        if(hex[i] >= 'A' && hex[i] <= 'F')
        {
            digit = hex[i] - 'A' + 10;
        }
        else if(hex[i] >= '0' && hex[i] <= '9')
        {
            digit = hex[i] - '0';
        }
        else
        {
            puts("Неправильный формат 16-ого числа:");
            puts("используйте только цифры 0-9 и буквы A-F");
            return 0;
        }
        
        pow16 = ((uint64_t)1 << (4 * (len - 1 - i)));
        answ += (digit * pow16);
        i++;
    }
    return answ;
}

void PrintJsonWithValueNull(const char * typeName)
{   
    PutComma();
    printf("\n  {\"type\": \"%s\", \"value\": null}", typeName);
}
void PrintJsonWithValueString(const char *typeName, const char* value)
{
    PutComma();
    printf("\n  {\"type\": \"%s\", \"value\": \"%s\"}", typeName, value);
}
void PrintJsonWithValueInt(const char *typeName, int yytext)
{
    PutComma();
    printf("\n  {\"type\": \"%s\", \"value\": %d}", typeName, yytext);
}
void PrintJsonWithValueHex(const char *typeName, const char* yytext)
{
    PutComma();
    unsigned long long hex = HexToInt(yytext);
    printf("\n  {\"type\": \"%s\", \"value\": %llu}", typeName, hex);
}
void PrintJsonWithValueOct(const char *typeName, const char* yytext)
{
    PutComma();
    unsigned long long oct = OctToInt(yytext);
    printf("\n  {\"type\": \"%s\", \"value\": %llu}", typeName, oct);
}
void PrintJsonWithValueBin(const char *typeName, const char* yytext)
{
    PutComma();
    unsigned long long bin = BinToInt(yytext);
    printf("\n  {\"type\": \"%s\", \"value\": %llu}", typeName, bin);
}
%}
VAR     [a-z]
DDIGIT  [0-9]
DCONST  {DDIGIT}+
HDIGIT  [0-9A-F]
HCONST  {HDIGIT}+[h]
ODIGIT  [0-7]
OCONST  {ODIGIT}+[o]
BDIGIT  [01]
BCONST  [BDIGIT]+[b]
SYM_COMPLEX [F][0-9]+
LOG_COMPLEX [L][0-9]+
SCONST '(\\.^\\|[^\\'\n]|''|\\\\)+' 

%%
    /*переменная*/
{VAR}           { PrintJsonWithValueString(ID, yytext); }
    /*символьный комплекс*/
"F"             { PrintJsonWithValueString(COMPLEX, "symbol"); }
    /*логический комплекс*/
"L"             { PrintJsonWithValueString(COMPLEX, "logic"); }
    /*глобальный комплекс*/
"K"             { PrintJsonWithValueString(COMPLEX, "global"); }
    /*ёмкость комплекса*/
"S"             { PrintJsonWithValueNull("capacity"); }
    /*мощность комплекса*/
"Q"             { PrintJsonWithValueNull("cardinality"); }
    /*десятичная константа*/
{DCONST}        { PrintJsonWithValueInt(INT, atoi(yytext)); }
    /*шестнадцатиричная константа*/
{HCONST}        { PrintJsonWithValueHex(INT, yytext); }
    /*восьмеричная константа*/
{OCONST}        { PrintJsonWithValueOct(INT, yytext); }
    /*двоичная константа*/
{BCONST}        { PrintJsonWithValueBin(INT, yytext); }
    /*символьная константа*/
{SCONST}        { 
                    PrintJsonWithValueNull("quote");
                    char *str = strndup(yytext + 1, strlen(yytext) - 2);
                    PrintJsonWithValueString("sconst", str);
                    PrintJsonWithValueNull("quote");
                }
    /*Установить 1 в i-ую позицию*/
"I"             { PrintJsonWithValueNull("set_bit");}
    /*особая переменная*/
"Z"             { PrintJsonWithValueNull("spec_var"); }

    /*Конец функции*/
"**"            { PrintJsonWithValueNull("fret"); }
    /*Имя функции*/
[a-zA-Z_][a-zA-Z0-9_]*\(        {
                                    char *str = strndup(yytext,strlen(yytext) - 1);
                                    PrintJsonWithValueString(ID, str);
                                    PrintJsonWithValueNull("lpar");                        
                                }
    /***** логические и арифметические операции *****/
    /*inc*/
"∆"              { PrintJsonWithValueNull("inc"); }
    /*dec*/
"∇"              { PrintJsonWithValueNull("dec"); }
    /*сумма по mod 2^32(в Z переполнение)*/
"+"              { PrintJsonWithValueNull("add"); }
    /*вычитаение по mod 2^32*/
"-"              { PrintJsonWithValueNull("sub"); }
    /*вес булевого вектора*/
"%"              { PrintJsonWithValueNull("weight"); }
    /*остаток от деления(в Z частное)*/
";"              { PrintJsonWithValueNull("mod"); }
    /*деление двойного числа*/
":"              { PrintJsonWithValueNull("div"); }
    /*дизъюнкция*/
"∨"              { PrintJsonWithValueNull("or"); }
    /*конъюнкция*/
"&"              { PrintJsonWithValueNull("and"); }
    /*сложение mod 2*/
"⊕"              { PrintJsonWithValueNull("xor"); }
    /*отрицание*/
"¬"              { PrintJsonWithValueNull("not"); }
    /*вычисление номера правой еденицы*/
"!"              { PrintJsonWithValueNull("num_right_one"); }   
    /*консоль*/
"С"              { PrintJsonWithValueNull("console"); }

    /***** Многозначные символы  *****/
    /*угловые скобки: для разделения входных и выходные параметров;
      для сдвигов, для консоли*/
"<"              { PrintJsonWithValueNull("l_ang_brack"); }   
">"              { PrintJsonWithValueNull("r_ang_brack"); }
    /*частное от деления(В Z остаток) или для консоли*/
"/"              { PrintJsonWithValueNull("slash"); }
    /*присвоить тао значение с выхода псевдослучайного генератора
      или  спец. символ в операции перечисления единиц */
"X"              { PrintJsonWithValueNull("letter_X"); }
    /*вызов функции или умножение по mod 2^32(в Z переполнение)*/
"*"              { PrintJsonWithValueNull("star"); }
"↑"              { PrintJsonWithValueNull("up_arrow"); }

    /*********************************/

    /*присвоение наименьшего значения(т.е. все нули)*/
"O"              { PrintJsonWithValueNull("set_min"); }
    /*присвоение наибольшего значение(т.е. все единицы)*/
"⁻"              { PrintJsonWithValueNull("set_max"); }
    /*присвоение*/
"⇒"              { PrintJsonWithValueNull("assign"); }
    /*обмен значениями*/
"⇔"              { PrintJsonWithValueNull("swap"); }
    /*присвоить тао значение с выхода таймера компьютера */
"T"              { PrintJsonWithValueNull("get_time"); }
    /*метки*/
\§{DCONST}      {
                    PrintJsonWithValueNull("paragraph"); 
                    PrintJsonWithValueInt(INT, atoi(yytext + 2));
                }
    /*отношения сравнения*/
"="             { PrintJsonWithValueNull("je"); }
"≠"             { PrintJsonWithValueNull("jne"); }
"≥"             { PrintJsonWithValueNull("jge"); }
"≤"             { PrintJsonWithValueNull("jle"); }

    /*операции перехода*/
"→"             { PrintJsonWithValueNull("jmp"); }
"↪"             { PrintJsonWithValueNull("jz"); }
"↦"             { PrintJsonWithValueNull("jnz"); }

   /*разделители*/ 
"("             { PrintJsonWithValueNull("lpar"); }
")"             { PrintJsonWithValueNull("rpar"); }
"["             { PrintJsonWithValueNull("lbrack"); }
"]"             { PrintJsonWithValueNull("rbrack"); }
"'"             { PrintJsonWithValueNull("quote"); }
","             { PrintJsonWithValueNull("comma"); }
"."             { PrintJsonWithValueNull("dot"); }
\{[^\}]*\}      { 
                    PrintJsonWithValueNull("lbrace");
                    char *str = strndup(yytext + 1, strlen(yytext) - 2);
                    PrintJsonWithValueString("asm", str);
                    PrintJsonWithValueNull("rbrace");
                }
    /*ничего не делаем*/
[\n\t ]         {   ;   }

%%

int main(int argc, char *argv[]) {
    yyin = stdin;
    printf("[");
    yylex();
    printf("]\n");
    return 0;
}
