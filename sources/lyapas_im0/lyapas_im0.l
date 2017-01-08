%{
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "print_json.h"

# define INT "int"
# define ID "id"
# define COMPLEX "complex"

typedef unsigned long long int uint64;
int first_time = 1;
int err_line_ptr = 1;
int err_column_ptr = 1;
const char *file_name_string;

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
uint64 BinToInt(const char *bin)
{   
    int i = 0;
    uint64 answ = 0;
    int digit = 0;
    uint64 pow2 = 0;
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
        
        pow2 = ((uint64)1 << (2 * (len - 1 - i)));
        answ += (digit * pow2);
        i++;
    }
    return answ;
}
uint64 OctToInt(const char *oct)
{   
    int i = 0;
    uint64 answ = 0;
    int digit = 0;
    uint64 pow8 = 0;
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
        
        pow8 = ((uint64)1 << (3 * (len - 1 - i)));
        answ += (digit * pow8);
        i++;
    }
    return answ;
}
uint64 HexToInt(const char *hex)
{   
    int i = 0;
    uint64 answ = 0;
    int digit = 0;
    uint64 pow16 = 0;
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
        
        pow16 = ((uint64)1 << (4 * (len - 1 - i)));
        answ += (digit * pow16);
        i++;
    }
    return answ;
}

void PrintJsonWithValueNull(const char * typeName, const char *yytext)
{   
    PutComma();
    printf("\n  {\"type\": \"%s\", \"value\": null}", typeName);
    err_column_ptr += strlen(yytext);
}
void PrintJsonWithValueString(const char *typeName, const char* value, int num_char_in_str)
{
    PutComma();
    printf("\n  {\"type\": \"%s\", \"value\": \"%s\"}", typeName, value);
    err_column_ptr += num_char_in_str;
}
void PrintJsonWithValueInt(const char *typeName, const char *yytext)
{
    PutComma();
    printf("\n  {\"type\": \"%s\", \"value\": %d}", typeName, atoi(yytext));
    err_column_ptr += strlen(yytext);
}
void PrintJsonWithValueHex(const char *typeName, const char* yytext)
{
    PutComma();
    unsigned long long hex = HexToInt(yytext);
    printf("\n  {\"type\": \"%s\", \"value\": %llu}", typeName, hex);
    err_column_ptr += strlen(yytext);
}
void PrintJsonWithValueOct(const char *typeName, const char* yytext)
{
    PutComma();
    unsigned long long oct = OctToInt(yytext);
    printf("\n  {\"type\": \"%s\", \"value\": %llu}", typeName, oct);
    err_column_ptr += strlen(yytext);
}
void PrintJsonWithValueBin(const char *typeName, const char* yytext)
{
    PutComma();
    unsigned long long bin = BinToInt(yytext);
    printf("\n  {\"type\": \"%s\", \"value\": %llu}", typeName, bin);
    err_column_ptr += strlen(yytext);
}
void PrintJsonWithLexicalError(const char *yytext, int err_line_ptr, int err_column_ptr)
{
    printf("\n  {\"type\": \"error\",\n\
   \"kind\": \"lexical\",\n\
   \"details\": \"Character \"%s\" are not valid.\",\n\
   \"location\": {\n\
     \"file\": \"%s\",\n\
     \"line\": %d,\n\
     \"column\": %d\n\
    }\n\
  }", yytext, file_name_string, err_line_ptr, err_column_ptr);
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
{VAR}           { PrintJsonWithValueString(ID, yytext, 1); }
    /*символьный комплекс*/
"F"             { PrintJsonWithValueString(COMPLEX, "symbol", 1); }
    /*логический комплекс*/
"L"             { PrintJsonWithValueString(COMPLEX, "logic", 1); }
    /*глобальный комплекс*/
"K"             { PrintJsonWithValueString(COMPLEX, "global", 1); }
    /*ёмкость комплекса*/
"S"             { PrintJsonWithValueNull("capacity", yytext); }
    /*мощность комплекса*/
"Q"             { PrintJsonWithValueNull("cardinality", yytext); }
    /*десятичная константа*/
{DCONST}        { PrintJsonWithValueInt(INT, yytext); }
    /*шестнадцатиричная константа*/
{HCONST}        { PrintJsonWithValueHex(INT, yytext); }
    /*восьмеричная константа*/
{OCONST}        { PrintJsonWithValueOct(INT, yytext); }
    /*двоичная константа*/
{BCONST}        { PrintJsonWithValueBin(INT, yytext); }
    /*символьная константа*/
{SCONST}        {
                    char *str = strndup(yytext, 1);
                    PrintJsonWithValueNull("quote", str);
                    int str_len = strlen(yytext) - 2;
                    str = strndup(yytext + 1, str_len);
                    PrintJsonWithValueString("sconst", str, str_len);
                    PrintJsonWithValueNull("quote", yytext + str_len);
                }
    /*Установить 1 в i-ую позицию*/
"I"             { PrintJsonWithValueNull("set_bit", yytext);}
    /*особая переменная*/
"Z"             { PrintJsonWithValueNull("spec_var", yytext); }

    /*Конец функции*/
"**"            { PrintJsonWithValueNull("fret", yytext); }
    /*Имя функции. Нельзя использовать имена состоящие из одного символа и совпадающие с комплексами.*/
([a-zA-Z][a-zA-Z_][a-zA-Z0-9_]*\()|([a-zA-Z_]{-}[FLK][a-zA-Z0-9_]*\()  {
                                    int str_len = strlen(yytext) - 1;
                                    char *str = strndup(yytext, str_len);
                                    PrintJsonWithValueString(ID, str, str_len);
                                    PrintJsonWithValueNull("lpar", yytext + str_len);
                                }
    /***** логические и арифметические операции *****/
    /*inc*/
"∆"              { PrintJsonWithValueNull("inc", yytext); }
    /*dec*/
"∇"              { PrintJsonWithValueNull("dec", yytext); }
    /*остаток от деления(в Z частное)*/
";"              { PrintJsonWithValueNull("mod", yytext); }
    /*деление двойного числа*/
":"              { PrintJsonWithValueNull("div", yytext); }
    /*дизъюнкция*/
"∨"              { PrintJsonWithValueNull("or", yytext); }
    /*конъюнкция*/
"&"              { PrintJsonWithValueNull("and", yytext); }
    /*сложение mod 2*/
"⊕"              { PrintJsonWithValueNull("xor", yytext); }
    /*отрицание*/
"¬"              { PrintJsonWithValueNull("not", yytext); }
    /*вычисление номера правой еденицы*/
"!"              { PrintJsonWithValueNull("num_right_one", yytext); }
    /*консоль*/
"C"              { PrintJsonWithValueNull("console", yytext); }

    /***** Многозначные символы  *****/
    /*угловые скобки: для разделения входных и выходные параметров;
      для сдвигов, для консоли, для вставки и удаления элемента в комплексах*/
"<"              { PrintJsonWithValueNull("l_ang_brack", yytext); }
">"              { PrintJsonWithValueNull("r_ang_brack", yytext); }
    /*частное от деления(В Z остаток) или для консоли*/
"/"              { PrintJsonWithValueNull("slash", yytext); }
    /*присвоить тао значение с выхода псевдослучайного генератора
      или  спец. символ в операции перечисления единиц */
"X"              { PrintJsonWithValueNull("letter_X", yytext); }
    /*вызов функции или умножение по mod 2^32(в Z переполнение)*/
"*"              { PrintJsonWithValueNull("star", yytext); }
"↑"              { PrintJsonWithValueNull("up_arrow", yytext); }
    /*операции над комплексами*/
    /*используется для копирования элементов комплекса*/
"#"              { PrintJsonWithValueNull("copy", yytext); }
    /*используется для операций над комплексами*/
"@"              {PrintJsonWithValueNull("at_sign", yytext); }
    /*сумма по mod 2^32(в Z переполнение) и создание комплекса*/
"+"              { PrintJsonWithValueNull("plus", yytext); }
    /*вычитаение по mod 2^32 и уничтожение компекса*/
"-"              { PrintJsonWithValueNull("minus", yytext); }
    /*вес булевого вектора и сокращение ёмкости комплекса*/
"%"              { PrintJsonWithValueNull("percent", yytext); }

    /*********************************/

    /*присвоение наименьшего значения(т.е. все нули)*/
"O"              { PrintJsonWithValueNull("set_min", yytext); }
    /*присвоение наибольшего значение(т.е. все единицы)*/
"⁻"              { PrintJsonWithValueNull("set_max", yytext); }
    /*присвоение*/
"⇒"              { PrintJsonWithValueNull("assign", yytext); }
    /*обмен значениями*/
"⇔"              { PrintJsonWithValueNull("swap", yytext); }
    /*присвоить тао значение с выхода таймера компьютера */
"T"              { PrintJsonWithValueNull("get_time", yytext); }
    /*метки*/
\§{DCONST}      {
                    //здесь и во многих других местах копируем 2 байта т.к. это юникод
                    char *str = strndup(yytext, 2);
                    PrintJsonWithValueNull("paragraph", str);
                    PrintJsonWithValueInt(INT, yytext + 2);
                }
    /*отношения сравнения*/
"="             { PrintJsonWithValueNull("je", yytext); }
"≠"             { PrintJsonWithValueNull("jne", yytext); }
"≥"             { PrintJsonWithValueNull("jge", yytext); }
"≤"             { PrintJsonWithValueNull("jle", yytext); }

    /*операции перехода*/
"→"             { PrintJsonWithValueNull("jmp", yytext); }
"↪"             { PrintJsonWithValueNull("jz", yytext); }
"↦"             { PrintJsonWithValueNull("jnz", yytext); }

   /*разделители*/ 
"("             { PrintJsonWithValueNull("lpar", yytext); }
")"             { PrintJsonWithValueNull("rpar", yytext); }
"["             { PrintJsonWithValueNull("lbrack", yytext); }
"]"             { PrintJsonWithValueNull("rbrack", yytext); }
"'"             { PrintJsonWithValueNull("quote", yytext); }
","             { PrintJsonWithValueNull("comma", yytext); }
"."             { PrintJsonWithValueNull("dot", yytext); }
\{[^\}]*\}      { 
                    char *str = strndup(yytext, 1);
                    PrintJsonWithValueNull("lbrace", str);
                    int str_len = strlen(yytext) - 2;
                    str = strndup(yytext + 1, str_len);
                    PrintJsonWithValueString("asm", str, str_len);
                    PrintJsonWithValueNull("rbrace", yytext + str_len);
                }
    /*ничего не делаем*/
[\n]            { ++err_line_ptr; err_column_ptr = 1; }
[ ]             { ++err_column_ptr; }
[\t ]           { err_column_ptr+=4; }
.               {
                    PrintJsonWithLexicalError(yytext, err_line_ptr, err_column_ptr);
                    ++err_column_ptr;
                }

%%

int main(int argc, char *argv[]) 
{
    json_error_t json_err;
    json_t *program;
    json_t *result;

    program = json_loadf(stdin, 0, &json_err);    
    if(!program)
    {
        fprintf(stderr,"Incorrect JSON\n");
        return 0;
    }
    
    json_t *source_value = json_object_get(program, "source");
    json_t *file_name_value = json_object_get(program, "file");
    
    file_name_string = json_string_value(file_name_value);
    const char *source_string = json_string_value(source_value);
    
    FILE * file = fopen("file.tmp", "w+");
    
    if(file != NULL)
    {
        fprintf(file, "%s", source_string);
        fseek(file, SEEK_SET, 0);

        yyin = file;
        printf("[");
        yylex();
        printf("]\n");
    }
    
    fclose(file);
    return 0;
}
