
extern malloc
extern free
extern fprintf
extern stack_snooper
extern stack_snooper_n
section .data

section .text

global strCmp
global strClone
global strDelete
global strPrint
global strLen

; ** String **

; int32_t strCmp(char* a, char* b)
;a : rdi, b : rsi
strCmp:
	push rbp
	mov rbp,rsp	
ciclo:	
	cmp rdi, 0
	jz finish
	cmp byte [rdi],0; ve si a termino
	je aFinish
	cmp byte [rsi],0; ve si b termino
	je bFinish
	mov r10b, [rsi]
	cmp byte [rdi], r10b ; compara los chars en a y en b
	jl menor
	jg mayor
	inc rdi
	inc rsi
	jmp ciclo
	
aFinish:
	cmp byte [rsi],0; Dado que a finalizó, chequeo si b finalizó
	je iguales
	jmp menor; a finalizo y b no, por el invariante, hasta este punto a y b eran iguales, por lo que a es menor
bFinish:
	cmp byte [rdi],0; Dado que b finalizó, chequeo si a finalizó
	je iguales
	jmp mayor; misma logica que para aFinish, solo que a es mayor
menor:
	mov rax,1
	jmp finish

mayor:
	mov rax,-1
	jmp finish

iguales:
	mov rax, 0
	jmp finish
	
finish:

	pop rbp 	
	ret

; char* strClone(char* a)
; a : rdi
strClone:
        ;prologo
	push rbp
	mov rbp, rsp
	
	;ej 4b
	
	push r15
	xor rax, rax
	cmp rdi, 0
	jz nullException
	mov r15, rdi

    sub rsp, 0x8
	call strLen; mide la longitud del string

	inc rax;no olvidarse del byte 0
	mov rcx, rax; rcx ahora tiene la longitud del string
	mov rdi,rax 
	call malloc wrt ..plt; reserva memoria para longitud de string bytes, el resto es magia negra para que linkee bien
	mov rdi, r15
	add rsp, 0x8
	
	
	mov rdx, rax; rdx tendra una copia al puntero inicial del string sobre el que vamos iterando.
	
cicloClone:
	cmp rcx,0; Veo si termino el string original
	je fin
	mov r10b,[rdi]
	mov [rdx],r10b
	inc rdx
	inc rdi
	dec rcx
	jmp cicloClone
	
fin:	
	pop r15
	pop rbp
	ret


nullException:
	mov rax,0		
	jmp fin


; void strDelete(char* a)
strDelete:
	;prologo
	push rbp
	mov rbp, rsp
	
	call free wrt ..plt; free borra toda memoria reservada a partir de un puntero, es decir, borra todo el bloque de memoria reservada a partir de una posicion dada, por lo que solo hace falta llamarla una vez. De aqui una pregunta: como sabe free cuando termina un bloque de memoria, es decir, como me puedo asegurar de que free no borre dos estructuras que se encuentran por mala suerte de manera contigua. ¿Malloc asegura que la posicion de memoria guardada no sea contigua a otra ya reservada?
	
	pop rbp
	ret

; void strPrint(char* a, FILE* pFile); a: rdi, pFile: rsi
strPrint: 
	push rbp
	mov rbp,rsp
	cmp rdi, 0
	jz nullWrite
	cmp byte [rdi],0
	jz nullWrite

	
cicloPrint: 
	mov r10, [rdi]
	mov [rsi], r10
	inc rsi
	inc rdi
	cmp byte [rdi], 0; el primer caracter es el caracter vacio.
	jz finishPrint
	jmp cicloPrint

nullWrite:
	mov byte [rsi], 'N'
	inc rsi
	mov byte [rsi], 'U'
	inc rsi
	mov byte [rsi], 'L'
	inc rsi
	mov byte [rsi], 'L'
	inc rsi
	jmp finishPrint

finishPrint:
	mov byte [rsi], 0
	pop rbp
	ret

	
; uint32_t strLen(char* a)
; registros: a[rdi]
strLen:
	push rbp
	mov rbp, rsp
	
	;ej 4b
	push 11037
	 
	xor rax,rax
	
	;ej 4b
	push rdi

	cmp rdi, 0
	jz nullExc
	
	mov rdi,1
	; call stack_snooper_n
	; inc rdi
	; call stack_snooper_n
	pop rdi
    pop r9; popea el dummy
	
	cmp byte [rdi], 0
	jz fin2
	
ciclo_Len:
	add rax, 1
	add rdi, 1
	cmp byte [rdi], 0
	jnz ciclo_Len
	jz fin2
	
nullExc:
	pop rdi
	pop r9
fin2:		
	
	pop rbp 
	ret
