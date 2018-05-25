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

void appendWeight(Program &program)
{
        //
        // источник: http://www.hackersdelight.org/hdcodetxt/pop.c.txt
        // см. реализацию pop0
        //
        program.push_front(makeCmd(R"(
;;
;; in: rdi
;; out: rax
;;
__weight:
  mov rax, 0x5555555555555555
  mov rdx, rdi
  shr rdi, 1
  and rdx, rax
  and rdi, rax
  mov rcx, 0x0F0F0F0F0F0F0F0F
  lea rax, [rdx+rdi]
  mov rdi, 0x3333333333333333
  mov rdx, rax
  shr rax, 2
  and rdx, rdi
  and rax, rdi
  add rax, rdx
  mov rdx, rax
  shr rax, 4
  and rdx, rcx
  and rax, rcx
  mov rcx, 0x00FF00FF00FF00FF
  add rax, rdx
  mov rdx, rax
  shr rax, 8
  and rdx, rcx
  and rax, rcx
  mov rcx, 0x0000FFFF0000FFFF
  add rax, rdx
  mov rdx, rax
  shr rax, 16
  and rdx, rcx
  and rax, rcx
  add rax, rdx
  mov edx, eax
  shr rax, 32
  add rax, rdx
  ret
)"));

}

void appendEnumeration(Program &program)
{
        program.push_front(makeCmd(R"(
;;
;; in: rdi
;; out: rax - index of right 1
;;      rbx - input without right 1
;;
__enumeration:
  mov rax, rdi ; rax - input
  dec rdi
  and rdi, rax ; rdi - input without right 1
  mov rbx, rdi
  xor rdi, rax ; rdi - input's right 1
  jmp __tzcnt  ; rax - index of right 1
)"));
}
}

void appendBuiltinFunctions(Program &program)
{
    appendTrailingZerosCount(program);
    appendWeight(program);
    appendEnumeration(program);
    program.push_front(makeCmd("section .text"));
}

}
