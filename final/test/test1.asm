    global main
    extern printf

    section .data
a dd 0
b dd 0
c dd 0
d dd 0
const_4 dd 1
const_5 dd 3
const_6 dd 2
const_7 dd 0
const_8 dd 0
const_9 dd 3
const_10 dd 0
const_11 dd 10
const_12 dd 100
const_13 dd 1
const_14 dd 1
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
temp_30 dd 0
temp_31 dd 0
temp_32 dd 0
temp_33 dd 0

    section .text
main:
L0:
mov eax, dword [const_4]
mov [a], eax
mov [temp_15], eax
L1:
mov eax, dword [const_5]
mov [b], eax
mov [temp_16], eax
L2:
mov eax, [b]
imul dword [const_6]
mov [temp_17], eax
L3:
mov eax, [a]
add eax, [temp_17]
mov [temp_18], eax
L4:
mov eax, dword [temp_18]
mov [c], eax
mov [temp_19], eax
L5:
mov eax, dword [const_7]
mov [d], eax
mov [temp_20], eax
L6:
mov eax, dword [c]
cmp eax, dword [const_8]
jg LL6
mov dword [temp_21], 0
jmp LL7
LL6: mov dword [temp_21], 1
LL7:
L7:
cmp dword [temp_21], 0
je L23
L8:
mov eax, [a]
imul dword [const_9]
mov [temp_22], eax
L9:
mov eax, [b]
sub eax, [temp_22]
mov [temp_23], eax
L10:
mov eax, dword [temp_23]
cmp eax, dword [const_10]
je LL10
mov dword [temp_24], 0
jmp LL11
LL10: mov dword [temp_24], 1
LL11:
L11:
cmp dword [temp_24], 0
je L15
L12:
mov eax, [d]
add eax, [const_11]
mov [temp_25], eax
L13:
mov eax, dword [temp_25]
mov [d], eax
mov [temp_26], eax
L14:
jmp L17
L15:
mov eax, [d]
add eax, [const_12]
mov [temp_27], eax
L16:
mov eax, dword [temp_27]
mov [d], eax
mov [temp_28], eax
L17:
mov eax, 0
sub eax, [const_13]
mov [temp_29], eax
L18:
mov eax, [temp_29]
add eax, [b]
mov [temp_30], eax
L19:
mov eax, dword [temp_30]
mov [b], eax
mov [temp_31], eax
L20:
mov eax, [c]
sub eax, [const_14]
mov [temp_32], eax
L21:
mov eax, dword [temp_32]
mov [c], eax
mov [temp_33], eax
L22:
jmp L6
L23:
    ; 手动加上的代码
    push  dword [a]
    push    'a'
	push	message
	call	printf
	add	esp, 12

    push  dword [b]
    push    'b'
	push	message
	call	printf
	add	esp, 12

    push  dword [c]
    push    'c'
	push	message
	call	printf
	add	esp, 12

    push  dword [d]
    push    'd'
	push	message
	call	printf
	add	esp, 12
	ret
	
section .data
message: db '%c value: %d', 10, 0