    global main
    extern printf

    section .data
a dd 0
b dd 0
c dd 0
d dd 0
const_4 dd 1
const_5 dd 3
const_6 dd 0
const_7 dd 1
const_8 dd 3
const_9 dd 10
const_10 dd 100
const_11 dd 1
const_12 dd 1
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
temp_29 dd 0

    section .text
main:
L0:
mov eax, dword [const_4]
mov [a], eax
mov [temp_13], eax
L1:
mov eax, dword [const_5]
mov [b], eax
mov [temp_14], eax
L2:
mov eax, [a]
add eax, [b]
mov [temp_15], eax
L3:
mov eax, dword [temp_15]
mov [c], eax
mov [temp_16], eax
L4:
mov eax, dword [const_6]
mov [d], eax
mov [temp_17], eax
L5:
mov eax, dword [c]
cmp eax, dword [const_7]
jg LL5
mov dword [temp_18], 0
jmp LL6
LL5: mov dword [temp_18], 1
LL6:
L6:
cmp dword [temp_18], 0
je L21
L7:
mov eax, [a]
imul dword [const_8]
mov [temp_19], eax
L8:
mov eax, [b]
sub eax, [temp_19]
mov [temp_20], eax
L9:
cmp dword [temp_20], 0
je L13
L10:
mov eax, [d]
add eax, [const_9]
mov [temp_21], eax
L11:
mov eax, dword [temp_21]
mov [d], eax
mov [temp_22], eax
L12:
jmp L15
L13:
mov eax, [d]
add eax, [const_10]
mov [temp_23], eax
L14:
mov eax, dword [temp_23]
mov [d], eax
mov [temp_24], eax
L15:
mov eax, 0
sub eax, [const_11]
mov [temp_25], eax
L16:
mov eax, [temp_25]
add eax, [b]
mov [temp_26], eax
L17:
mov eax, dword [temp_26]
mov [b], eax
mov [temp_27], eax
L18:
mov eax, [c]
sub eax, [const_12]
mov [temp_28], eax
L19:
mov eax, dword [temp_28]
mov [c], eax
mov [temp_29], eax
L20:
jmp L5
L21:

    push  dword [c]
	push	message
	call	printf
	add	esp, 8
	ret

section .data
message: db 'res : %d', 10, 0