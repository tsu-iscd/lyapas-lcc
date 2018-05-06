# Описание двуяза

Отличается от предыдущего языка тем, что представляет собой язык абстрактной двухадресной машины. Переход от одноадресной к двухадресной машине облегчает явную работу со стеком на следующих этапах трансляции и помогает более эффективно производить оптимизацию получаемой программы.

## Операции передачи значения

### Запись в τ
```
load <writable_int> => move acc, <writable_int>
load <memory_cell> => move acc, <memory_cell>
```

### Запись в операнд
```
store <writable_int> => move <writable_int>, acc
store <memory_cell> => move <memory_cell>, acc
```

### Обмен значениями переменных
```
swap <variable:1>, <variable:2>
=>
move swaptemp, <variable:1>
move <variable:1>, <variable:2>
move <variable:2>, swaptemp
```

### Обмен значениями элементов комплекса
```
swap_comp_el <complex>, <int:1>, <int:2>
=>
move swaptemp, <complex_cell>(<int:1>)
move <complex_cell>(<int:1>), <complex_cell>(<int:2>)
move <complex_cell>(<int:2>), swaptemp
```
, где `<complex_cell>(i)`, это элемент с индексом `i` в комплексе `<complex>`.

### Запись значения
```
move <writable_int>, <writable_int>
move <writable_int>, <memory_cell>
move <memory_cell>, <memory_cell>
move <memory_cell>, <writable_int>
```
Записывает значение из второго операнда в первый.

[comment]: # (операции set_min и set_max из однояза остаются без изменений)

## Операции генератора и времени

### Чтение времени таймера
```
get_time => get_time acc
```
```
get_time <writable_int>
```
Записывает значения из таймера компьютера в операнд.

### Инициализация ГПСЧ
```
set_random => set_random acc
```
```
set_random <readable_int>
```
Устанавливает состояние генератора псевдослучайных чисел в состояние значения операнда.

### Чтение псевдослучайного числа
```
get_random => get_random acc
```
```
get_random <writable_int>
```
Записывает значение из псевдослучайной последовательности в операнд.

## Логические операции

### Номер правой единицы
```
get_number_least_1 => get_number_least_1 acc
```
```
get_number_least_1 <writable_int>
```
Записывает в операнд номер правой единицы в бинарном представлении операнда.

### Отрицание
```
not => not acc
```
```
not <writable_int>
```
Записывает в операнд результат отрицания бинарного представления операнда.

### Вес булева вектора
```
weight => weight acc
```
```
weight <writable_int>
```
Записывает в операнд вес булева вектора бинарного представления операнда.

### Дизъюнкция
```
or <readable_int> => or acc, <readable_int>
```
```
or <writable_int>, <readable_int>
```
Записывает в первый операнд результат дизъюнкции операндов.

### Конъюнкция
```
and <readable_int> => and acc, <readable_int>
```
```
and <writable_int>, <readable_int>
```
Записывает в первый операнд результат конъюнкции операндов.

### Сложение по модулю 2
```
xor <readable_int> => xor acc, <readable_int>
```
```
xor <writable_int>, <readable_int>
```
Записывает в первый операнд результат сложения по модулю 2 операндов.

### Левый сдвиг
```
left_shift <readable_int> => left_shift acc, <readable_int>
```
```
left_shift <writable_int>, <readable_int>
```
Записывает в первый операнд результат левого сдвига первого операнда на второй.

### Правый сдвиг
```
right_shift <readable_int> => right_shift acc, <readable_int>
```
```
right_shift <writable_int>, <readable_int>
```
Записывает в первый операнд результат правого сдвига первого операнда на второй.

## Арифметические операции

### Инкрементация
```
inc <writable_int>
=>
inc <writable_int>
move acc, <writable_int>
```
```
inc <writable_int>
```
Увеличивает операнд на единицу.

### Декрементация
```
dec <writable_int>
=>
dec <writable_int>
move acc, <writable_int>
```
```
dec <writable_int>
```
Уменьшает операнд на единицу.

### Сложение
```
add <readable_int> => add acc, <readable_int>
```
```
add <writable_int>, <readable_int>
```
Записывает в первый операнд результат сложения операндов.

### Вычитание
```
sub <readable_int> => sub acc, <readable_int>
```
```
sub <writable_int>, <readable_int>
```
Записывает в первый операнд результат вычитания второго операнда из первого.

### Умножение
```
mul <readable_int> => mul acc, <readable_int>, Z
```
```
mul <writable_int>, <readable_int>, <writable_int>
```
Умножает первые два операнда, в первый операнд записывается результат, а переполнение --- в третий.

### Деление
```
div <readable_int> => div acc, <readable_int>, Z
```
```
div <writable_int>, <readable_int>, <writable_int>
```
Делит второй операнд на первый, в первый операнд записывается частное, а остаток --- в третий.

### Взятие остатка
```
mod <readable_int> => mod acc, <readable_int>, Z
```
```
mod <writable_int>, <readable_int>, <writable_int>
```
Делит второй операнд на первый, в первый операнд записывается остаток, а частное --- в третий.

### Деление двойного числа
```
ddiv <readable_int> => ddiv acc, <readable_int>, Z
```
```
ddiv <writable_int>, <readable_int>, <writable_int>
```
Делит двойное число на второй операнд, в первый операнд записывается частное, а остаток --- в третий. Двойное число состоит из первого операнда в младшей части и третьего операнда в старшей части.

## Операции перехода
[comment]: # (операции jump <const>, jump_z <const>, jump_nz <const>, jump_eq <const>, jump_neq <const>, jump_gt <const>, jump_geq <const>, jump_lt <const>, jump_leq <const>, enumeration_1 <const> <readable_int:1> <readable_int:2> из однояза остаются без изменений)

### Сравнивание
```
compare <readable_int> => compare acc, <readable_int>
```
```
compare <readable_int>, <readable_int>
```
Вычитает второй операнд из первого. Устанавливает флаг нуля, если результат равен нулю. Устанавливает флаг заема, если для вычитания требуется заем.

## Операции над комплексами
[comment]: # (операции create_complex <complex> <int>, rmv_complex <complex>, reduce_complex <complex>, clear_complex <complex>, insert_string_in_complex, copy_complex из однояза остаются без изменений)

### Вставка в комплекс
```
insert_element_in_complex <complex>, <int>
=>
insert_element_in_complex <complex>, <int>, acc
```
```
insert_element_in_complex <complex>, <int>, <readable_int>
```
Третий операнд записывается в позицию перед элементом с индексом `<int>`. Мощность комплекса увеличивается на 1. Приложение завершается аварийной остановкой, если для вставки не хватает ёмкости или индекс выходит за границы комплекса.

### Вставка в конец комплекса
```
push_back_element_to_complex <complex>
=>
push_back_element_to_complex <complex>, acc
```
```
push_back_element_to_complex <complex>, <readable_int>
```
Второй операнд записывается в конец комплекса. Мощность комплекса увеличивается на 1. Приложение завершается аварийной остановкой, если для вставки не хватает ёмкости.

### Удаление элемента из комплекса
```
remove_element_from_complex <complex>, <int>
=>
remove_element_from_complex <complex>, <int>, acc
```
```
remove_element_from_complex <complex>, <int>, <writable_int>
```
Удаляется элемент с индексом `<int>` из комплекса. Удаленный элемент записывается в третий операнд, а мощность комплекса уменьшается на 1. Приложение завершается аварийной остановкой, если комплекс имеет нулевую мощность перед выполнением операции или элемента с таким индексом нет.

### Удаление элемента из конца комплекса
```
pop_back_element_from_complex <complex>, 
=>
pop_back_element_from_complex <complex>, acc
```
```
pop_back_element_from_complex <complex>, <writable_int>
```
Удаляется элемент из конца комплекса. Удаленный элемент записывается во второй операнд, а мощность комплекса уменьшается на 1. Приложение завершается аварийной остановкой, если комплекс имеет нулевую мощность перед выполнением операции.

[comment]: # ( Блок 'прочее'  из однояза остается без изменений)
