/**********************************
*** Не реализованы восмиричные и двоичные константы.
*** Не реализовано деление двойного числа(операция ":").
*** Не обрабатываются ассемблерные вставки. 
**********************************/
%{
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

# define INT "int"
# define ID "id"
# define COMPLEX "complex"
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
unsigned long long int GetPow16(unsigned short int i)
{
    if(i > 15)
    {
        return 0;
    }
    unsigned long long int pow = ((unsigned long long int)1 << (4 * i));
    return pow;
}
unsigned long long int HexToInt(const char *hex)
{   
    int i = 0;
    unsigned long long int answ = 0;
    int digit = 0;
    unsigned long long int pow16 = 0;
    int len = 0;

    while(hex[len] != '\0')
    {
        len++;
    }

    if(len > 1 && hex[len - 1] == 'h')
    {
        len--;
    }
    else
    {
        puts("Неправильный формат 16-ого числа:");
        puts("Используйте h в конце.");
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
        pow16 = GetPow16(len - 1 - i);
        if(pow16 == 0)
        {
            puts("число должно быть меньше 16 цифр");
            return 0;
        }
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
"S"             { PrintJsonWithValueNull("cap"); }
    /*мощность комплекса*/
"Q"             { PrintJsonWithValueNull("card"); }
    /*десятичная константа*/
{DCONST}        { PrintJsonWithValueInt(INT, atoi(yytext)); }
    /*шестнадцатиричная константа*/
{HCONST}        { PrintJsonWithValueHex(INT, yytext); }

    /*символьная константа*/
{SCONST}        { PrintJsonWithValueString("sconst", yytext); }
    /*Установить 1 в i-ую позицию*/
"I"             { PrintJsonWithValueNull("setBit");}
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
    /*дизъюнкция*/
"∨"              { PrintJsonWithValueNull("or"); }
    /*конъюнкция*/
"&"              { PrintJsonWithValueNull("and");       }
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
">"              { PrintJsonWithValueNull("r_abrack"); }
    /*частное от деления(В Z остаток) или для консоли*/
"/"              { PrintJsonWithValueNull("slash"); }
    /*присвоить тао значение с выхода псевдослучайного генератора
      или  спец. символ в операции перечисления единиц */
"X"              { PrintJsonWithValueNull("get_rand"); }
    /*вызов функции или умножение по mod 2^32(в Z переполнение)*/
"*"              { PrintJsonWithValueNull("star"); }

    /*********************************/

    /*присвоение наименьшего значения(т.е. все нули)*/
"O"              { PrintJsonWithValueNull("set_min"); }
    /*присвоение наибольшего значение(т.е. все единицы)*/
"⁻"              { PrintJsonWithValueNull("set_max"); }
    /*присвоение*/
"⇒"              { PrintJsonWithValueNull("assign"); }
    /*обмен значениями*/
"⇔"              { PrintJsonWithValueNull("exchange"); }
    /*присвоить тао значение с выхода таймера компьютера */
"T"              { PrintJsonWithValueNull("get_time"); }
    /*метки*/
\§{DCONST}      {
                    PrintJsonWithValueNull("lable"); 
                    PrintJsonWithValueInt(INT, atoi(yytext + 2));
                }
    /*отношения сравнения*/
"="             { PrintJsonWithValueNull("je"); }
"≠"             { PrintJsonWithValueNull("jne"); }
"≥"             { PrintJsonWithValueNull("jge"); }
"≤"             { PrintJsonWithValueNull("jle"); }

    /*операции перехода*/
"→"             { PrintJsonWithValueNull("goto"); }
"↪"             { PrintJsonWithValueNull("jz"); }
"↦"             { PrintJsonWithValueNull("jnz"); }
"↑"             { PrintJsonWithValueNull("goto_relation"); }

   /*разделители*/ 
"("             { PrintJsonWithValueNull("lpar"); }
")"             { PrintJsonWithValueNull("rpar"); }
"["             { PrintJsonWithValueNull("lbrack"); }
"]"             { PrintJsonWithValueNull("rbrack"); }
"'"             { PrintJsonWithValueNull("quote"); }
","             { PrintJsonWithValueNull("comma"); }
"."             { PrintJsonWithValueNull("dot"); }
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
