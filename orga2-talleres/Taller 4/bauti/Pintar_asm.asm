
global Pintar_asm

;void Pintar_asm(unsigned char *src, <---------RDI
;              unsigned char *dst, <---------RSI
;              int width,       <---------RDX
;              int height,      <---------RCX
;              int src_row_size,   <---------R8
;              int dst_row_size);  <---------R9



Pintar_asm:
	push rbp
	mov rbp , rsp
	push r12
	push r13
	push r14
	push r15
	push r10
	push r9

	;en rax voy a guardar la direccion de memoria donde tengo que ir a buscar los pixeles
	;en r15 voy a guardar la direccion de memoria donde voy a poner los pixeles

	mov rax , rdi
	mov r15 , rsi

	mov rbx , rcx
	sub rbx , 3

	mov r9 , rdx
	sub r9 , 4




	mov r12, 0x00ffffff
	movq xmm3, r12
	PUNPCKLDQ xmm3, xmm3
	PUNPCKLDQ xmm3, xmm3 ; mascara para poner los 4 pixeles en negro (no se toca A)

	mov r12, 0xff000000
	movq xmm4, r12
	PUNPCKLDQ xmm4, xmm4
	PUNPCKLDQ xmm4, xmm4 ; mascara para poder aumentar A 

	mov r12, 0x00ffffff
	movq xmm5, r12
	PUNPCKLDQ xmm5, xmm5 ; mascara para middle left ( xmm5 =0x0000000000000000ffffff00ffffff00  )



	mov r12 , 0 ;iterador de filas
	mov r13 , 0 ;iterador de columnas
	mov r14 , 0 ;r14 es el valor que tengo que sumar para pasar a la siguiente fila. aumenta a la par que las filas  FUENTE
	mov r10 , 0 ;r1O es el valor que tengo que sumar para pasar a la siguiente fila. aumenta a la par que las filas  DESTINO
ciclo:
	;me quede sin columnas
	cmp r13 , rdx 
	jz aumentar_fila 

	;me quede sin filas
	cmp r12 , rcx 
	jz fin

	;estoy en el tope
	cmp r12 , 2
	jl top_bottom

	;estoy en el fondo
	cmp r12 , rbx
	jg top_bottom

	jmp middle

aumentar_fila:
	inc r12 ;aumento iterador filas
	mov r13 , 0 ; columnas en 0

	add r14 , r8; aumento r14
	add r10 , [rbp - 0x30]

	mov rax , rdi
	add rax , r14

	mov r15 , rsi
	add r15 , r10


	jmp ciclo


middle: ;si estas en el medio hay 3 casos: estar a la izq, estar en el centro o estar en la der
	;estoy en la izq
	cmp r13 , 0
	jz middle_right ;estas llamadas tendriasn que estar al reves, pero asi funcionan 

	;estoy en la der
	cmp r13 , r9
	jz middle_left ;estas llamadas tendriasn que estar al reves, pero asi funcionan 

	;estoy en el centro (no hace falta saltar)
	
middle_middle:
	MOVDQU xmm0, [rax] ;tomo los 4 pixeles

	PADDUSB xmm0, xmm4 ;aumento la saturacion
	PADDUSB xmm0 , xmm3 ;seteo en blanco 

	MOVDQU [r15] , xmm0 ;Mando los pixeles a su respectiva pos en dest

	add r13 , 4 ;aumento columna
	add rax , 16
	add r15 , 16
	jmp ciclo


middle_left:
	MOVDQU xmm0, [rax] ;tomo los 4 pixeles

	PADDUSB xmm0, xmm4 ;aumento la saturacion

	PSUBUSB xmm0 , xmm3 ;seteo en negro 
	PADDUSB xmm0 , xmm5 ; seteo los 2 de la der en blanco

	MOVDQU [r15] , xmm0 ;Mando los pixeles a su respectiva pos en dest

	add r13 , 4 ;aumento columna
	add rax , 16
	add r15 , 16
	jmp ciclo
middle_right:
	MOVDQU xmm0, [rax] ; tomo los 4 pixeles

	PADDUSB xmm0, xmm4 ;aumento la saturacion

	PADDUSB xmm0 , xmm3 ;seteo en blanco 
	PSUBUSB xmm0 , xmm5 ; seteo los 2 de la der en negro


	MOVDQU [r15] , xmm0 ;Mando los pixeles a su respectiva pos en dest

	add r13 , 4 ;aumento columna
	add rax , 16
	add r15 , 16
	jmp ciclo
	

top_bottom:
	MOVDQU xmm0, [rax] ; tomo los 4 pixeles

	PADDUSB xmm0, xmm4 ;aumento la saturacion
	PSUBUSB xmm0 , xmm3 ;seteo en negro 

	MOVDQU [r15] , xmm0 ;Mando los pixeles a su respectiva pos en dest

	add r13 , 4 ;aumento columna
	add rax , 16
	add r15 , 16
	jmp ciclo


fin:
	pop r9
	pop r10
	pop r15
	pop r14
	pop r13
	pop r12 
	pop rbp
	ret
	


