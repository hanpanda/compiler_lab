    global main
    extern printf

    section .data
msg: db 'result : %d', 10, 0
a dd 0
b dd 0
c dd 0
const_3 dd 1
const_4 dd 2
const_5 dd 1
const_6 dd 3
const_7 dd 5
const_8 dd 3
const_9 dd 4
const_10 dd 6
const_11 dd 3
temp_12 dw 0
temp_13 dw 0
temp_14 dw 0
temp_15 dw 0
temp_16 dw 0
temp_17 dw 0
temp_18 dw 0
temp_19 dw 0
temp_20 dw 0
temp_21 dw 0
temp_22 dw 0

    section .text
main:
mov eax, dword [const_3]
mov [a], eax
mov [temp_12], eax
mov eax, dword [const_4]
mov [b], eax
mov [temp_13], eax
mov eax, [a]
add eax, [b]
mov [temp_14], eax
mov eax, dword [temp_14]
mov [c], eax
mov [temp_15], eax
mov eax, dword [const_5]
mov [c], eax
mov [temp_16], eax
cmp dword [temp_16], 0
je L9
mov eax, [const_6]
imul dword [const_7]
; mov [temp_17], eax
; mov edx, 0
; mov eax, [temp_17]
; idiv dword [const_8]
; mov [temp_18], eax
; mov eax, dword [temp_18]
; mov [c], eax
; mov [temp_19], eax
jmp L10
L9:
mov eax, [const_9]
imul dword [const_10]
mov [temp_20], eax
mov edx, 0
mov eax, [temp_20]
idiv dword [const_11]
mov [temp_21], eax
mov eax, dword [temp_21]
mov [c], eax
mov [temp_22], eax
L10:

push dword [temp_16]
push msg
call printf
add esp, 8
ret