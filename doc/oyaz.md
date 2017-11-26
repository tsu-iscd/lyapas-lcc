# Описание однояза

Язык абстрактной одноадресной машины, каждый регистр общего назначения которой соответствует переменной ЛЯПАСа. Каждый вызов подпрограммы на ЛЯПАСе создает в машине новый набор регистров. Копирование входных и выходных параметров выполняется абстрактной машиной автоматически. Поскольку ЛЯПАС изначально создавался для компиляции в язык одноадресной машины Урал-1, этот язык является наиболее простым выходным языком для трансляции с ЛЯПАСа из всех следующих языков модульного транслятора.

## Операции передачи значения

### Запись в τ
```
load <readable_int>
load <memory_cell>
```
Записывает значение из операнда в τ.

### Запись в операнд
```
store <writable_int>
store <memory_cell>
```
Записывает значение из τ в операнд.

### Запись минимального значения
```
set_min <writable_int>
```
Записывает в операнд минимальное значение.

### Запись максимального значения
```
set_max <writable_int>
```
Записывает в операнд максимальное значение.

### Обмен значениями переменных
```
swap <variable>, <variable>
```

### Обмен значениями элементов комплекса
```
swap_comp_el <complex>, <int:1>, <int:2>
```
Обменивает значения элементов `<int:1>` и `<int:2>` комплекса `<complex>`.

## Операции генератора и времени

### Чтение времени таймера
```
get_time
```
Записывает значения из таймера компьютера в τ.

### Инициализация ГПСЧ
```
set_random
```
Устанавливает состояние генератора псевдослучайных чисел в состояние τ.

### Чтение псевдослучайного числа
```
get_random
```
Записывает значение из псевдослучайной последовательности в τ.

## Логические операции

### Номер правой единицы
```
get_number_least_1
```
Записывает в τ номер правой единицы в бинарном представлении τ.

### Отрицание
```
not
```
Записывает в τ результат отрицания бинарного представления τ.

### Вес булева вектора
```
weight
```
Записывает в τ вес булева вектора бинарного представления τ.

### Дизъюнкция
```
or <readable_int>
```
Записывает в τ результат дизъюнкции операнда и τ.

### Конъюнкция
```
and <readable_int>
```
Записывает в τ результат конъюнкции операнда и τ.

### Сложение по модулю 2
```
xor <readable_int>
```
Записывает в τ результат сложения по модулю 2 операнда и τ.

### Левый сдвиг
```
left_shift <readable_int>
```
Записывает в τ результат левого сдвига τ на операнд.

### Правый сдвиг
```
right_shift <readable_int>
```
Записывает в τ результат правого сдвига τ на операнд.

## Арифметические операции

### Инкрементация
```
inc <writable_int>
```
Записывает в τ и операнд результат инкрементации операнда.

### Декрементация
```
dec <writable_int>
```
Записывает в τ и операнд результат декрементации операнда.

### Сложение
```
add <readable_int>
```
Записывает в τ результат сложения операнда и τ.

### Вычитание
```
sub <readable_int>
```
Записывает в τ результат вычитания операнда из τ.

### Умножение
```
mul <readable_int>
```
Умножает τ на операнд, в τ записывается результат, а переполнение --- в Z.

### Деление
```
div <readable_int>
```
Делит τ на операнд, в τ записывается частное, а остаток --- в Z.

### Взятие остатка
```
mod <readable_int>
```
Делит τ на операнд, в τ записывается остаток, а частное --- в Z.

### Деление двойного числа
```
ddiv <readable_int>
```
Делит двойное число Zτ на операнд, в τ записывается частное, а остаток --- в Z.

## Операции Перехода

### Безусловный переход
```
jump <const>
```
Переходит на метку `<const>`.

### Переход по нулевому τ
```
jump_z <const>
```
Переходит на метку `<const>`, если τ равно 0.

### Переход по ненулевому τ
```
jump_z <const>
```
Переходит на метку `<const>`, если τ не равно 0.

### Сравнивание
```
compare <readable_int>
```
Вычитает операнд из τ. Устанавливает флаг нуля, если результат равен нулю. Устанавливает флаг заема, если для вычитания требуется заем.

### Переход по равенству
```
jump_eq <const>
```
Переходит на метку `<const>`, если установлен флаг нуля.

### Переход по неравенству
```
jump_neq <const>
```
Переходит на метку `<const>`, если не установлен флаг нуля.

### Переход по условию больше
```
jump_gt <const>
```
Переходит на метку `<const>`, если не установлены флаги заема и нуля.

### Переход по условию больше или равно
```
jump_geq <const>
```
Переходит на метку `<const>`, если не установлен флаг заема.

### Переход по условию меньше
```
jump_lt <const>
```
Переходит на метку `<const>`, если установлен флаг заема.

### Переход по условию меньше или равно
```
jump_leq <const>
```
Переходит на метку `<const>`, если установлены флаги заема или нуля.

### Перечисление единиц
```
enumeration_1 <const>, <readable_int:1>, <readable_int:2>
```
Если `<readable_int:1>` равен 0, то в `<readable_int:2>` записывается 0 и осуществляется переход на метку `<const>`, в противном случае правая 1 в `<readable_int:1>` заменяется на 0, а её номер записывается в `<readable_int:2>`.

## Операции над комплексами

### Создание комплекса
```
create_complex <complex>, <int>
```
Создается комплекс ёмкости `<int>` и мощности 0.

### Уничтожение комплекса
```
rmv_complex <complex>
```
Освобождается память, выделенная под комплекс. Комплекс помечается как несозданный.

### Сокращение ёмкости
```
reduce_complex <complex>
```
Ёмкость комплекса сокращается до его мощности.

### Очистка комплекса
```
clear_complex <complex>
```
Заполняет комплекс нулями в пределах его мощности.

### Запись строки в комплекс
```
insert_string_in_complex <symbol_complex>, <string>
```
Содержимое комплекса заменяется строкой. Мощность равна длине строки, а ёмкость равна по крайней мере мощности.

### Вставка в комплекс
```
insert_in_complex <complex>, <int>, <readable_int>
```
Третий операнд записывается в позицию перед элементом с индексом `<int>`. Если элемента с таким индексом нет, то добавляем третий операнд после последнего элемента. Мощность комплекса увеличивается на 1. Приложение завершается аварийной остановкой, если для вставки не хватает ёмкости.

### Удаление элемента из комплекса
```
rmv_from_complex <complex>, <int>, <writable_int>
```
Удаляется элемент с индексом `<int>` из комплекса. Если элемента с таким индексом нет, то удаляем последний элемент. Удаленный элемент записывается в третий операнд, а мощность комплекса уменьшается на 1. Приложение завершается аварийной остановкой, если комплекс имеет нулевую мощность перед выполнением операции.

### Копирование элементов комплексов
```
copy_complex <complex:1>, <complex:2>, <int:1>, <int:2>, <int:3>
```
`<int:1>` элементов комплекса `<complex:1>`, начиная с `<int:2>`-го элемента (в отсутствие `<int:2>` --- первые `<int:1>` элементов `<complex:1>`), копируются в `<complex:2>`, начиная с `<int:3>`-го элемента (в отсутствие `<int:3>` --- в конец `<complex:2>`). Мощности `<complex:1>` и `<complex:2>` не изменяются. Приложение завершается аварийной остановкой, если обозначенные выше интервалы выходят за пределы мощности комплексов.

## Операции ввода-вывода

### Вывод символьного комплекса на консоль
```
write_complex <symbol_complex>
```
Выводит символы комплекса в пределах мощности на консоль.

### Вывод строки на консоль
```
write_string <string>
```

### Ввод строки с консоли
```
read_complex <symbol_complex>
```
Символы, введенные с клавиатуры, добавляются к символьному комплексу, мощность комплекса увеличивается с очередным прочитанным символом. Операция завершается, если мощность становится равна ёмкости, либо считан символ перевода строки.

## Прочее

### Объявление подпрограммы
```
definition <name>, <params:1>, /, <params:2>
```
Объявление подпрограммы с именем `<name>`, где `<params:1>` --- входные параметры, а `<params:2>` --- выходные параметры.

### Вызов подпрограммы
```
call <name>, <args:1>, /, <args:2>
```
Вызов подпрограммы с именем `<name>`, `<args:1>` --- входные аргументы, `<args:2>` --- выходные аргументы.

### Ассемблерная вставка
```
asm <string>
```
Выполняется код ассемблера из `<string>`.

### Метка
```
label <const>
```
Объявление метки с номером `<const>`. Метка доступна только в подпрограмме, в которой она объявлена.