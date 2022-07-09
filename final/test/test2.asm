    global main
    extern printf

    section .data
var_a dd 0
var_b dd 0
var_c dd 0
const_3 dd 7
const_4 dd 4
const_5 dd 1
const_6 dd 2
const_7 dd 3
const_8 dd 4
temp_9 dd 0
temp_10 dd 0
temp_11 dd 0
temp_12 dd 0
temp_13 dd 0
temp_14 dd 0
temp_15 dd 0
temp_16 dd 0
temp_17 dd 0
temp_18 dd 0
temp_19 dd 0
temp_20 dd 0
temp_21 dd 0
temp_22 dd 0
temp_23 dd 0
temp_24 dd 0
temp_25 dd 0
temp_26 dd 0
temp_27 dd 0
temp_28 dd 0

    section .text
main:
L0:
mov eax, dword [const_3]
mov [var_a], eax
mov [temp_9], eax
L1:
mov eax, dword [const_4]
mov [var_b], eax
mov [temp_10], eax
L2:
mov eax, [var_a]
sub eax, [var_b]
mov [temp_11], eax
L3:
mov edx, 0
mov eax, [var_a]
idiv dword [temp_11]
mov [temp_12], edx
L4:
mov eax, dword [temp_12]
mov [var_c], eax
mov [temp_13], eax
    
    ; test
    push dword [var_c]
    push dword [var_b]
    push dword [var_a]
    push msg
    call printf
    add esp, 16

L5:
mov eax, dword [var_c]
cmp eax, dword [var_b]
jl LL5
mov dword [temp_14], 0
jmp LLL5
LL5: mov dword [temp_14], 1
LLL5:
L6:
cmp dword [temp_14], 0
je L9
L7:
mov eax, dword [const_5]
mov [var_a], eax
mov [temp_15], eax
L8:
jmp L9
L9:
    ; test
    push dword [var_c]
    push dword [var_b]
    push dword [var_a]
    push msg
    call printf
    add esp, 16

mov eax, dword [var_c]
cmp eax, dword [var_b]
jl LL9
mov dword [temp_16], 0
jmp LLL9
LL9: mov dword [temp_16], 1
LLL9:
L10:
mov eax, [temp_16]
cmp eax, 0
je LL10
mov dword [temp_17], 0
jmp LLL10
LL10:
mov dword [temp_17], 1
LLL10:
L11:
cmp dword [temp_17], 0
je L14
L12:
mov eax, dword [const_6]
mov [var_a], eax
mov [temp_18], eax
L13:
jmp L14
L14:
    ; test
    push dword [var_c]
    push dword [var_b]
    push dword [var_a]
    push msg
    call printf
    add esp, 16

mov eax, dword [var_c]
cmp eax, dword [var_b]
jl LL14
mov dword [temp_19], 0
jmp LLL14
LL14: mov dword [temp_19], 1
LLL14:
L15:
mov eax, [temp_19]
cmp eax, 0
je LL15
mov dword [temp_20], 0
jmp LLL15
LL15:
mov dword [temp_20], 1
LLL15:
L16:
mov eax, dword [var_c]
cmp eax, dword [var_b]
jl LL16
mov dword [temp_21], 0
jmp LLL16
LL16: mov dword [temp_21], 1
LLL16:
L17:
mov eax, [temp_20]
or eax, dword [temp_21]
mov [temp_22], eax
L18:
cmp dword [temp_22], 0
je L21
L19:
mov eax, dword [const_7]
mov [var_a], eax
mov [temp_23], eax
L20:
jmp L21
L21:
    ; test
    push dword [var_c]
    push dword [var_b]
    push dword [var_a]
    push msg
    call printf
    add esp, 16

mov eax, dword [var_c]
cmp eax, dword [var_b]
jl LL21
mov dword [temp_24], 0
jmp LLL21
LL21: mov dword [temp_24], 1
LLL21:
L22:
mov eax, [temp_24]
cmp eax, 0
je LL22
mov dword [temp_25], 0
jmp LLL22
LL22:
mov dword [temp_25], 1
LLL22:
L23:
mov eax, dword [var_c]
cmp eax, dword [var_b]
jl LL23
mov dword [temp_26], 0
jmp LLL23
LL23: mov dword [temp_26], 1
LLL23:
L24:
mov eax, [temp_25]
and eax, dword [temp_26]
mov [temp_27], eax
L25:
cmp dword [temp_27], 0
je L28
L26:
mov eax, dword [const_8]
mov [var_a], eax
mov [temp_28], eax
L27:
jmp L28
L28:
    ; test
    push dword [var_c]
    push dword [var_b]
    push dword [var_a]
    push msg
    call printf
    add esp, 16
section .data
msg: db 'var_a, var_b, var_c value: %d, %d, %d', 10, 0