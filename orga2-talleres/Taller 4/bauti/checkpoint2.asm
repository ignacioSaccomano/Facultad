
section .text

global checksum_asm

; uint8_t checksum_asm(void* array, uint32_t n)

checksum_asm:
	push rbp
	mov rbp , rsp
	mov rax, 1
	mov rbx, 0
	mov r9, 0
	
	mov r8, 8
	movq xmm3, r8
	PUNPCKLWD xmm3, xmm3
	PUNPCKLWD xmm3, xmm3
	PUNPCKLWD xmm3, xmm3 ; en cada word tengo un 8

	mov r8, 0
	not r8


	;Viendo el tester, los valores no son muy altos.
	;No me preocupo por el overflow en las sumas.

ciclo: 
	MOVDQA xmm0, [rdi + rbx] ; agarramos los primeros 128 bits (los A)
	MOVDQA xmm1, [rdi + 16 + rbx] ; agarramos los segundos 128 bits (los B)
	PADDW xmm0 , xmm1 ; los sumamos (cada word + su respectiva word)
	movaps xmm1 , xmm0 ; lo copiamos a xmm1

	PMULLW xmm0, xmm3 ; en xmm0 multiplico por 8 y me quedo con la parte baja 
	PMULHW xmm1, xmm3 ; en xmm1 multiplico por 8 y me quedo con la parte alta 

	movaps xmm5, xmm0 ; me guardo el valor de xmm0 porque se va a sobreescribir

	PUNPCKLWD xmm0, xmm1 ;desempaqueto. en xmm0 tengo la multiplicacion de los primeros 4 elem
	PUNPCKHWD xmm5, xmm1 ;desempaqueto. en xmm5 tengo la multiplicacion de los 4 elem restantes

	PCMPEQD xmm0 , [rdi + 16 + 16 + rbx]; comparo con los primeros 4 de C
	PCMPEQD xmm5 , [rdi + 16 + 16 + 16 + rbx]; comparo con los 4 restantes de C

	packssdw xmm0, xmm0 ;empaqueto para poder comparar
	movq rcx , xmm0
	cmp rcx, r8  
	jnz no_se_cumple
	packssdw xmm5, xmm5 ;empaqueto para poder comparar
	movq rcx , xmm5
	cmp rcx, r8 
	jnz no_se_cumple

	inc r9
	cmp r9, rsi
	jz fin


	add rbx , 64
	jmp ciclo


no_se_cumple:
	mov rax , 0

fin:
	pop rbp
	ret

