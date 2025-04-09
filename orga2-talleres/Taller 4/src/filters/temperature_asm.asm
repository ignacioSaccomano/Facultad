global temperature_asm

section .rodata
PIXEL_SIZE equ 4
PIXEL_PER_IT equ 2

masc_transparencia: times 4 dd 0x000000ff
turbio: times 2 dq 0x00ff000000000000

int_max: times 4 dd 0xffffffff
low_word_mask: times 4 dd 0x00ffff00
tres_flotante: times 4 dd 0x00000003
int_128: times 4 dd 128
int_32: times 4 dd 32
int_224: times 4 dd 224
int_160: times 4 dd 160
int_96: times 4 dd 96
int_255: times 4 dd 255
permut_correcta: times 4 dd 0x00030201;

section .text
;void temperature_asm(unsigned char *src,
;              unsigned char *dst,
;              int width,
;              int height,
;              int src_row_size,
;              int dst_row_size);
;rdi<- src, rsi <- dst, rdx<- width, rcx <- height, r8<-src_row_size, r9<-dst_row_size
temperature_asm:
push rbp
mov rbp,rsp
push r10
push r11
push r12
push r13
push r14; pila desapilada

movdqu xmm2,[masc_transparencia]
movdqu xmm3, [tres_flotante]
movdqu xmm9,[turbio]
movdqu xmm11, [int_255]
movdqu xmm12,[permut_correcta]
movdqu xmm13,[int_max]

cvtdq2ps xmm3,xmm3; convierte cada dword 3 en valores de simple precision

xor rax,rax; iterador de filas
mov r10, rdi; puntero a principio de fila(origen)
mov r11, rsi; puntero a principio de fila(destino)


mov r13,r10; avanzo en las filas origen y destino para sacar, evaluar y insertar pixeles r13 tiene origen, r14 tiene destino
mov r14,r11

ciclo_imagen:	
	cmp rax,rcx
	je fin_imagen
	xor r12,r12; iterador de columnas
	
	; va a ciclo_fila sin saltar
ciclo_fila:
	cmp r12,rdx
	je fin_fila
	pmovzxbw xmm0,[r13] ;extiendo cada atributo de dos pixeles a tener el doble de tamaño (evito overflows posteriores)
	
	por xmm0,xmm9; setea transparencia maxima a cada pixel
	phaddw  xmm0, xmm0; suma horizontalmente las words de xmm0 con si mismo: van a quedar words con las sumas, y van a repetirse en la parte alta y baja del registro
	
	phaddw xmm0,xmm0; lo mismo, ahora tenemos parte baja y alta con mismo contenido : suma pixel1|suma pixel2|suma pixel 1|suma pixel 2. Cada suma es un word
	pmovzxwd xmm0,xmm0; extiende cada word a dw
	
	psubw xmm0,xmm11; a cada temperatura resto 255
	
	
	cvtdq2ps xmm0,xmm0; convierte cada dword en valores de simple precision
	
	divps xmm0, xmm3
	
	cvttps2dq xmm0,xmm0; trunca el resultado de la division para cada dword. Hace conversion de cada valor ps a un double word
	
	
	
	;Actualmente, cada dword tiene el valor de temperatura de cada pixel, repetido 1 vez en cada qword. Todavia no agregamos la transparencia
	
	movdqu xmm4,xmm0;registro temporario donde guardamos el valor de los 2 pixeles.
	
	pshufd xmm0,xmm4, 10101010b; mete en xmm0 4 repeticiones del pixel1
	
	push rdx; pila apilada. Si no hago esto rdx varia poquito entre llamadas. Un horror
        call definir_color	
        
        movdqu xmm7,xmm0
        
        pshufd xmm0,xmm4 ,01010101b; otro pixel
	
	call definir_color
	pop rdx;
	
	pblendw xmm0,xmm7,00110011b ; Si esto esta bien blendeado, deberian estar, en cada qword, el primer pixel en la parte alta y el segundo en la baja
	
	
	

	
	movq [r14], xmm0; Mueve 2 pixeles (la mitad del registro r0) a la posicion de memoria donde corresponde
	
		
		
	add r13, PIXEL_PER_IT*PIXEL_SIZE
	add r14, PIXEL_PER_IT*PIXEL_SIZE
	add r12, PIXEL_PER_IT
	jmp ciclo_fila
		

fin_fila:
	inc rax
	add r10,r8; apunto a la siguiente fila en origen y destino
	add r11,r9
	mov r13, r10; punteros de columna reinicializados
	mov r14, r11;
	jmp ciclo_imagen


fin_imagen:
	mov rax, rsi
	
	
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop rbp
	
       ret
       
       
       
definir_color: ; dado el valor en xmm0 (la temperatura como un numero entero que ocupa un dword), lo modifica segun el criterio de temperatura. Devuelve el pixel modificado en xmm0 (4 copias empaquetadas)

	
	
	movd edx, xmm0 ; la temperatura es un valor de 8 bytes. No tiene sentido comparar con operaciones multimedia
	
	
	cmp edx,32
	jl caso_1
	
	cmp edx,96
	jl caso_2
	
	cmp edx,160
	jl caso_3
	
	cmp edx,224
	jl caso_4
	
	jmp caso_5
	
	
caso_1:
	
	pslld xmm0,2
	movdqu xmm10,[int_128]
	paddd xmm0,xmm10
	jmp fin_definir
caso_2:
	movdqu xmm10,[int_32]
	psubd xmm0,xmm10
	pslld xmm0,10; multiplico por 4 (<<2) y shifteo por un byte
	por xmm0,xmm2
	jmp fin_definir
caso_3:
	movdqu xmm10,[int_96]
	psubd xmm0,xmm10
	pslld xmm0,2
	movdqu xmm8,xmm0; guardo registro temporal con valor (t-96)*4 en cada dw
	pslld xmm0,8
	por xmm0,xmm2; ultimo byte tiene 255
	pslld xmm0,8; seteo byte del medio a 255
	por xmm0,xmm2; seteo ultimo byte a 255
	psubd xmm0,xmm8; ultimo byte tendra 255-(t-96)*4
	jmp fin_definir
caso_4:
	movdqu xmm10,[int_160]
	psubd xmm0,xmm10; resto 160 a temperatura
	pslld xmm0,2; multiplico por 4
	movdqu xmm8,xmm2; registro temporal con 255
	psubd xmm8,xmm0; resto (t-160)*4 a 255
	por xmm0,xmm2; seteo toda dw a 255
	pslld xmm0,8; byte menos significativo vacio
	por xmm0,xmm8; byte menos significativo tiene 255-(t-160)*4, el del medio tiene 255
	pslld xmm0,8; shifteo un byte mas y tengo lo deseado
	jmp fin_definir
caso_5:
	movdqu xmm10,[int_224]
	psubd xmm0,xmm10
	pslld xmm0,2
	movdqu xmm8,xmm2
	psubd xmm8,xmm0; no hay operacion simd para negar, asi que tengo que hacer esta suciedad
	movdqu xmm0,xmm8 
	pslld xmm0,16
	jmp fin_definir
		

	
fin_definir:
	pslld xmm0,8; deja vacio el espacio de la transparencia
	por xmm0,xmm2; agrega transparencia
	pshufb xmm0,xmm12; porque little endian vs big endian es confuso, y se penso todo al reves	
	ret
