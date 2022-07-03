    global main
    extern printf

section .data
msg: db 'result : %d', 10, 0
a dd 9
b dd 2
c dd 0
const_3 dd 1.000000
const_4 dd 2.000000
const_5 dd 1.000000
const_6 dd 3.000000
const_7 dd 4.000000
temp_8 dw 0
temp_9 dw 0
temp_10 dw 0
temp_11 dw 0
temp_12 dw 0
temp_13 dw 0
temp_14 dw 0

section .text
main:
; mov eax, dword [const_3]
; mov [a], eax
; mov [temp_8], eax
; mov eax, dword [const_4]
; mov [b], eax
; mov [temp_9], eax
; mov eax, [a]
; add eax, [b]
; mov [temp_10], eax
; mov eax, dword [temp_10]
; mov [c], eax
; mov [temp_11], eax
; mov eax, dword [const_5]
; mov [c], eax
; mov [temp_12], eax
; cmp dword [temp_12], 0
; je L7
; mov eax, dword [const_6]
; mov [c], eax
; mov [temp_13], eax
; L7:
; mov eax, dword [const_7]
; mov [c], eax
; mov [temp_14], eax

; multiply test edx:eax
; mov eax, [a]
; mul dword [a]

; mul float
; fld dword [a]
; fmul dword [b]
; fst dword [b]

mov eax, [a]
add eax, [b]

push eax
push msg
call printf
add esp, 8
ret

