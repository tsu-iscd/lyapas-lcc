# Описание комплексояза

В комплексояз добавляются операции захвата и освобождения памяти. Адресная арифметика в данном языке запрещена. Операции с комплексами производятся в явном виде, в отличие от предыдущих языков, в которых абстрактная машина имела элементарные операции для работы с комплексами.

Память для комплексов захватывается из кучи и хранится в двух структурах. Первая структура содержит служебную информацию: мощность, ёмкость и указатель на вторую структуру, которая содержит элементы комплекса. Служебную структуру можно расширять с целью хранения информации, которая необходима для мофицикаций языка ЛЯПАС.

Для трансляции операций комплексояза будем использовать вспомогательные переменные `cyaz_ti`, где i --- положительное число. Для нумерации вспомогательных меток используются числа, не занятые в метках транслируемой подпрограммы.

## Объявления

### Объявление строки
```
definition_string <string>, <writable_int>
```
Размещает в памяти строку из первого операнда и записывает её адрес во второй операнд.

## Операции чтения и записи символов

### Чтение символа с консоли
```
read_char <writable_int>
```
Считывает с консоли символ и записывает его в операнд.

### Вывод строки на консоль
```
write_string <readable_int:1>, <readable_int:2>
```
Выводит на консоль строку длины `<readable_int:2>`, лежащую по адресу `<readable_int:1>`.

## Операции обработки ошибок
### Аварийное завершение программы
```
error <string>
```
Выводит на консоль сообщение и завершает выполнение программы.

## Операции захвата и освобождения памяти

### Захват памяти
```
alloc <readable_int>, <writable_int>
```
Захватывает память размером, равным первому операнду, и записывает во второй операнд адрес захваченной памяти. Если первый операнд равен нулю, то во второй операнд записывается нулевой адрес. Приложение завершается аварийной остановкой, если захватить память не удалось.

### Захват памяти с запасом
```
alloc_at_least <readable_int>, <writable_int>
```
Захватывает память размером не менее значения первого операнда и записывает во второй операнд адрес захваченной памяти. Если первый операнд равен нулю, то во второй операнд записывается нулевой адрес. Приложение завершается аварийной остановкой, если захватить память не удалось.

### Перезахват памяти
```
realloc <readable_int>, <writable_int>
```
Первый операнд --- требуемый размеры захваченной памяти, а второй операнд --- адрес захваченной памяти. Если требуемый размер больше, чем текущий, то адрес захваченной памяти может измениться. В таком случае память полностью копируется из старого местоположения в новое, а во второй операнд записывается адрес захваченной памяти. Если новый размер равен нулю, то адрес захваченной памяти приравнивается к нулю. Приложение завершается аварийной остановкой, если захватить память не удалось.

### Освобождение памяти
```
dealloc <readable_int>
```
Освобождает память по адресу, который хранится в операнде.

## Обозначения

Введем обознчения для описания трансляции операций в комплексояз:
`<complex_struct>` ::= `<complex>_struct`

`<complex_cardinality>` ::= `8byte <complex>_struct[0]`

`<complex_capacity>` ::= `8byte <complex>_struct[1]`

`<complex_buffer>` ::= `8byte <complex>_struct[2]`

`<complex_buffer_var>` ::= вспомогательная переменная, в которой лежит значение `<complex_buffer>`

`<complex_element_size>` ::= `1byte` для символьных комплексов и `8byte` для логических.

`<complex_cell>(i)` ::= `<complex_element_size> <complex_buffer_var>[i]`.

`<string_len>` --- длина строки `<string>`.


## Операции над комплексами

### Создание комплекса
```
create_complex <complex>, <int>
=>
alloc 24, <complex_struct> 
move <complex_cardinality>, 0 
move <complex_capacity>, <int> 
move cyaz_t1, <int>
mul cyaz_t1, <complex_element_size>
alloc_at_least cyaz_t1, <complex_buffer> 
move <complex_buffer_var>, <complex_buffer>
```

### Уничтожение комплекса
```
remove_complex <complex>
=>
dealloc <complex_buffer> 
dealloc <complex_struct>
```

### Сокращение ёмкости
```
reduce_complex <complex>
=>
move cyaz_t1, <complex_cardinality>
mul cyaz_t1, <complex_element_size>
realloc cyaz_t1, <complex_buffer> 
move <complex_buffer_var>, <complex_buffer>
move <complex_capacity>, <complex_cardinality>
```

### Очистка комплекса
```
clear_complex <complex>
=>
move cyaz_t1, 0 
label 1 
compare <complex_cardinality>, cyaz_t1 
jump_eq 2 
move <complex_cell>(cyaz_t1), 0 
inc cyaz_t1 
jump 1 
label 2
```

### Запись строки в комплекс
```
insert_string_in_complex <symbol_complex>, <string>
=>
definition_string <string>, cyaz_t1 
compare <complex_capacity>, <string_len> 
jump_geq 1 
# ru: "Длина строки больше ёмкости комплекса" 
error "Length of string greater than capacity of complex"
label 1 
move <complex_cardinality>, <string_len> 
move cyaz_t2, 0 
label 2 
compare <complex_cardinality>, cyaz_t2 
jump_geq 3 
# индексация по символам
move <complex_cell>(cyaz_t2), 1byte cyaz_t1[cyaz_t2]
inc cyaz_t2 
jump 2 
label 3
```

### Вставка в комплекс
```
insert_element_in_complex <complex>, <int>, <readable_int>
=>
compare <complex_cardinality>, <complex_capacity> 
jump_lt 1 
error "Capacity is too small for inserting" 
label 1 
move cyaz_t1, <int> 
compare cyaz_t1, <complex_cardinality> 
jump_leq 2 
error "Index out of range" 
label 2 
inc <complex_cardinality> 
# копирование
move cyaz_t2, cyaz_t1 
inc cyaz_t2 
label 3 
compare <complex_cardinality>, cyaz_t2 
jump_geq 4 
move <complex_cell>(cyaz_t2), <complex_cell>(cyaz_t2-1) 
inc cyaz_t2 
jump 3 
label 4 
move <complex_cell>(cyaz_t1), <readable_int>
```

### Вставка в конец комплекса
```
push_back_element_to_complex <complex>, <readable_int>
=>
compare <complex_cardinality>, <complex_capacity> 
jump_lt 1 
error "Capacity is too small for inserting" 
label 1 
move cyaz_t1, <complex_cardinality> 
inc <complex_cardinality> 
# копирование
move cyaz_t2, cyaz_t1 
inc cyaz_t2 
label 3 
compare <complex_cardinality>, cyaz_t2 
jump_geq 4 
move <complex_cell>(cyaz_t2), <complex_cell>(cyaz_t2-1) 
inc cyaz_t2 
jump 3 
label 4 
move <complex_cell>(cyaz_t1), <readable_int>
```

### Удаление элемента из комплекса
```
remove_element_from_complex <complex>, <int>, <writable_int>
=>
compare <complex_cardinality>, 0 
jump_neq 1 
error "Cardinality is too small for removing" 
label 1 
move cyaz_t1, <int> 
compare cyaz_t1, <complex_cardinality> 
jump_lt 2 
error "Index out of range" 
label 2 
move <writable_int>, <complex_cell>(cyaz_t1) 
%% копирование
label 3 
inc cyaz_t1 
compare <complex_cardinality>, cyaz_t1 
jump_geq 4 
move <complex_cell>(cyaz_t1-1), <complex_cell>(cyaz_t1) 
jump 3 
label 4 
dec <complex_cardinality>
```

### Удаление элемента из конца комплекса
```
pop_back_element_from_complex <complex>, <writable_int>
=>
compare <complex_cardinality>, 0 
jump_neq 1 
error "Cardinality is too small for removing" 
label 1 
move cyaz_t1, <complex_cardinality> 
dec cyaz_t1 
move <writable_int>, <complex_cell>(cyaz_t1) 
%% копирование
label 3 
inc cyaz_t1 
compare <complex_cardinality>, cyaz_t1 
jump_geq 4 
move <complex_cell>(cyaz_t1-1), <complex_cell>(cyaz_t1) 
jump 3 
label 4 
dec <complex_cardinality>
```

### Копирование элементов комплексов
```
copy_complex <complex:1>, <complex:2>, <int:1>, <int:2>, <int:3>
=>
# move p1, <int:2>
# move p2, <int:3>
move cyaz_t1, <int:2> 
move cyaz_t2, <int:3> 
# if (p1 >= Q1) p1 = 0;
compare cyaz_t1, <complex_cardinality:1> 
jump_lt 1 
move cyaz_t1, 0 
label 1 
# if (<int:1> > Q1 - p1) error;
move cyaz_t3, <complex_cardinality:1> 
sub cyaz_t3, cyaz_t1 
compare <int:1>, cyaz_t3 
jump_leq 2 
error "Cardinality of first complex is too small" 
label 2 
# if (p2 >= Q2) {
#   if (Q2 >= <int:1>) p2 = Q2 - <int:1> else error;
# } else {
#   if (<int:1> > Q2 - p2) error
# }
# if
compare cyaz_t2, <complex_cardinality:2> 
jump_lt 3 
compare <complex_cardinality:2>, <int:1> 
jump_geq 4 
error "Cardinality of second complex is too small" 
label 4 
move cyaz_t2, <complex_cardinality:2> 
sub cyaz_t2, <int:1> 
jump 5 
label 3 
# else
move cyaz_t3, <complex_cardinality:2> 
sub cyaz_t3, cyaz_t2 
compare <int:1>, cyaz_t3, 
jump_leq 5 
error "Cardinality of second complex is too small" 
# копирование
label 5 
compare <complex_cardinality:1>, cyaz_t1 
jump_geq 6 
move <complex_cell:2>(cyaz_t2), <complex_cell:1>(cyaz_t1) 
inc cyaz_t1 
inc cyaz_t2 
jump 5 
label 6
```

## Операции ввода-вывода

### Вывод символьного комплекса на консоль
```
write_complex <symbol_complex>
=>
write_string <symbol_complex>, <complex_cardinality> 
```

### Вывод строки на консоль
```
write_string <string>
=>
definition_string <string>, cyaz_t1 
write_string cyaz_t1, <string_len>
```

### Ввод строки с консоли
```
read_complex <symbol_complex>
=>
move cyaz_t1, <complex_cardinality> 
label 1 
compare cyaz_t1, <complex_capacity> 
jump_geq 3 
read_char <complex_cell>(cyaz_t1) 
# символ переноса строки '\n' == 10
compare <complex_cell>(cyaz_t1), 10 
jump_eq 2 
inc cyaz_t1 
jump 1 
label 2 
inc cyaz_t1 
label 3 
move <complex_cardinality>, cyaz_t1
```
