### Общие обозначения для описания операций промежуточных языков:

`<const>` --- целочисленная неотрицательная константа, 32 бита.

`<variable>` --- целочисленная неотрицательная переменная, 32 бита. Обозначаются буквами `a, b, ..., z, Z`. Переменная `Z` используется в специальных целях и некоторые операции могут неявно изменять ее значение.

`<string>` --- набор символов `Unicode`, обрамленных двойными кавычками.

`<name>` --- набор символов английского алфавита и цифр, за исключением ключевых слов языка ЛЯПАС.

`<int>` ::= `<const>` | `<variable>`

`<symbol_complex>` ::= `F<const>`, где `<const>` --- номер комплекса.

`<number_complex>` ::= `L<const>`, где `<const>` --- номер комплекса.

`<complex>` ::= `<symbol_complex>` | `<number_complex>`

`<complex_cell>` ::= `<complex>[<int>]`

`<complex_cardinality>` ::= `Q<const>`, где `<const>` --- номер комплекса.

`<complex_capacity>` ::= `S<const>`, где `<const>` --- номер комплекса.

`<complex_attribute>` ::= `<complex_cardinality>` | `<complex_capacity>`

`<memory>` ::= `G1` | `G4`

`<memory_cell>` ::= `<memory>[<int>]`

`<writable_int>` ::= `<complex_cell>` | `<complex_attribut>` | `<variable>`

`<readable_int>` ::= `<writable_int>` | `<const>`

`<args>` --- набор аргументов, каждый из которых может быть как `<int>`, так и `<complex>`.

`<params>` --- набор параметров, каждый из которых может быть как `<variable>`, так и `<complex>`.
