    global main
    extern printf

    section .data
a dd 0
b dd 0
const_2 dd 10
const_3 dd 4
const_4 dd 1
const_5 dd 1
temp_6 dd 0
temp_7 dd 0
temp_8 dd 0
temp_9 dd 0
temp_10 dd 0

    section .text
main:
L0:
mov eax, dword [const_2]
mov [a], eax
mov [temp_6], eax
L1:
mov eax, dword [const_3]
mov [b], eax
mov [temp_7], eax
L2:
cmp dword [const_4], 0
je L10
L3:
mov eax, dword [a]
cmp eax, dword [b]
je LL3
mov dword [temp_8], 0
jmp LLL3
LL3: mov dword [temp_8], 1
LLL3:
L4:
cmp dword [temp_8], 0
je L7
L5:
jmp L10
L6:
jmp L7
L7:
mov eax, [a]
sub eax, [const_5]
mov [temp_9], eax
L8:
mov eax, dword [temp_9]
mov [a], eax
mov [temp_10], eax
L9:
jmp L2
L10:

    ; testing
    push dword [b]
    push dword [a]
    push msg
    call printf
    add esp, 12

section .data
msg: db 'a: %d, b: %d', 10, 0