#include "builtin_functions.h"
#include "make_cmd.h"

namespace ayaz {
namespace {

void appendTrailingZerosCount(Program &program)
{
    //
    // источник: http://www.hackersdelight.org/hdcodetxt/ntz.c.txt
    // см. реализацию ntz8
    //
    program.push_front(makeCmd(R"(
;;
;; in: rdi
;; out: rax
;;
__tzcnt:
  mov eax, edi
  neg eax
  and eax, edi
  imul eax, eax, 72416175
  shr eax, 26
  movzx rax, BYTE [__tzcnt__table + eax]
  cmp rax, 32
  je __tzcnt_1
  rep ret
__tzcnt_1:
  shr rdi, 32
  mov eax, edi
  neg eax
  and edi, eax
  imul edi, edi, 72416175
  shr edi, 26
  movzx rax, BYTE [__tzcnt__table + eax]
  add rax, 32
  ret
__tzcnt__table:
  db 32, 0 , 1 , 12, 2 , 6 , 99, 13
  db 3 , 99, 7 , 99, 99, 99, 99, 14
  db 10, 4 , 99, 99, 8 , 99, 99, 25
  db 99, 99, 99, 99, 99, 21, 27, 15
  db 31, 11, 5 , 99, 99, 99, 99, 99
  db 9 , 99, 99, 24, 99, 99, 20, 26
  db 30, 99, 99, 99, 99, 23, 99, 19
  db 29, 99, 22, 18, 28, 17, 16, 99
)"));

}
}

void appendBuiltinFunctions(Program &program)
{
    appendTrailingZerosCount(program);
}

}
