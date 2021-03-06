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

void appendDiv128To64(Program &program)
{
    //
    // источник: https://codereview.stackexchange.com/questions/67962/mostly-portable-128-by-64-bit-division
    //
    program.push_front(makeCmd(R"(
;;
;; in: rdi - lo
;;     rsi - hi
;;     rdx - denominator
;;
;; out: rdi - quotient
;;      rsi - remainder
;;
__div_128_64:
  lea rax, [rdi+rdi]
  sub rsp, 8
  shr rdi, 63
  mov ecx, 64
__div_128_64__1:
  mov r8, rsi
  add rsi, rsi
  or rsi, rdi
  mov rdi, rax
  shr r8, 63
  shr rdi, 63
  add rax, rax
  cmp rdx, rsi
  jbe __div_128_64__5
  test r8, r8
  je __div_128_64__2
__div_128_64__5:
  sub rsi, rdx
  or rax, 1
__div_128_64__2:
  sub ecx, 1
  jne __div_128_64__1
  mov rdi, rax
  add rsp, 8
  ret
)"));
}

void appendCopy(Program &program)
{
    program.push_front(makeCmd(R"(
;;
;; in: rdi - src
;;     rsi - dst
;;     rdx - len
;;
__copy:
  test rdx, rdx
  je __copy_1
  xor rax, rax
__copy_2:
  movzx ecx, BYTE [rdi+rax]
  mov BYTE [rsi+rax], cl
  add rax, 1
  cmp rdx, rax
  jne __copy_2
__copy_1:
  rep ret
)"));
}
}

void appendBuiltinFunctions(Program &program)
{
    appendTrailingZerosCount(program);
    appendWeight(program);
    appendEnumeration(program);
    appendDiv128To64(program);
    appendCopy(program);
    program.push_front(makeCmd("section .text"));
}

}
