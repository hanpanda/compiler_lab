    global main
    extern printf

    section .data
a dd 0
b dd 0
c dd 0
const_3 dd 10
const_4 dd 0
const_5 dd 0
const_6 dd 1
const_7 dd 5
const_8 dd 1
temp_9 dd 0
temp_10 dd 0
temp_11 dd 0
temp_12 dd 0
temp_13 dd 0
temp_14 dd 0
temp_15 dd 0
temp_16 dd 0
temp_17 dd 0

    section .text
main:
L0:
mov eax, dword [const_3]
mov [a], eax
mov [temp_9], eax
L1:
mov eax, dword [const_4]
mov [b], eax
mov [temp_10], eax
L2:
mov eax, dword [const_5]
mov [c], eax
mov [temp_11], eax
L3:
mov eax, dword [a]
cmp eax, dword [b]
jg LL3
mov dword [temp_12], 0
jmp LLL3
LL3: mov dword [temp_12], 1
LLL3:
L4:
cmp dword [temp_12], 0
je L14
L5:
mov eax, [b]
add eax, [const_6]
mov [temp_13], eax
L6:
mov eax, dword [temp_13]
mov [b], eax
mov [temp_14], eax
L7:
mov eax, dword [b]
cmp eax, dword [const_7]
je LL7
mov dword [temp_15], 0
jmp LLL7
LL7: mov dword [temp_15], 1
LLL7:
L8:
cmp dword [temp_15], 0
je L11
L9:
jmp L3
L10:
jmp L11
L11:
mov eax, [c]
add eax, [const_8]
mov [temp_16], eax
L12:
mov eax, dword [temp_16]
mov [c], eax
mov [temp_17], eax
L13:
jmp L3
L14:
    ; testing
    push dword [c]
    push dword [b]
    push dword [a]
    push msg
    call printf
    add esp, 16

section .data
msg: db 'a: %d, b: %d, c: %d', 10, 0