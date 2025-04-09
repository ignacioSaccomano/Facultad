section .rodata

PIXEL_PER_ITER equ 4;
PIXEL_SIZE equ 4

COLOR_BLANCO: times 4 dd 0xffffffff
COLOR_NEGRO: times 4 dd 0xff000000
section .text

global Pintar_asm

;void Pintar_asm(unsigned char *src,
;              unsigned char *dst,
;              int width,
;              int height,
;              int src_row_size,
;              int dst_row_size);
;rdi<-src,rsi<-dst, rdx <- width, rcx <- height, r8<-src_row_size, r9<-dst_row_size 


Pintar_asm:
push r12
push r13
push r15
push r14;
push r11;
push r10;
; rax tendra el contador de filas
; 
xor rax, rax; contador de filas
mov r12, rdx
sub r12, PIXEL_PER_ITER; variable que determina si estamos en la ultima iteracion de una fila
mov r15, rsi; iterador de fila de imagen destino
mov r14, rsi; puntero de referencia de comienzo de fila(dst)
mov r11, rdx; tamano de ancho en pixeles
mov r10,rcx; otro comparador que marca las ultimas dos filas
sub r10,2; ultimas 2 filas




ciclo_imagen:
	xor r13,r13; iterador de columnas que seteo a 0 cada vez que empiezo una fila nueva
	cmp rax,rcx
	je fin_pintar
	
        cmp rax,1
        jle pintar_negro
        cmp rax, r10
        jge pintar_negro; estoy en ultimas dos columnas
		        
	;NO HAY SALTO A CICLO_FILA: Es siguiente instruccion




ciclo_fila:
	cmp r13, 0; ¿estoy en la primera columna de una fila?
	je primeras_columnas
        cmp r13, r12; ¿estoy en las 4 ultimas columnas de una fila?
        je ultimas_columnas
        ;NO HAY SALTO A COLUMNAS DEL MEDIO
        
        
        
        
 ;columnas_del_medio         
	movdqu xmm1, [COLOR_BLANCO]
	movdqu [r15],xmm1	
	add r15, PIXEL_SIZE*PIXEL_PER_ITER	
	add r13, PIXEL_PER_ITER
	jmp ciclo_fila
    


primeras_columnas:;setea las primeras dos columnas a negro, el resto a blanco
	movdqu xmm1,[COLOR_NEGRO]
	movdqu xmm2,[COLOR_BLANCO]
	blendps xmm1,xmm2,1100 ; detesto blend
	movdqu [r15],xmm1
	add r15, PIXEL_SIZE*PIXEL_PER_ITER	
	add r13,PIXEL_PER_ITER
	jmp ciclo_fila
	
	
        
ultimas_columnas:;setea la ultima columna a negro, el resto a blanco
	movdqu xmm1,[COLOR_BLANCO]
	movdqu xmm2,[COLOR_NEGRO]
	blendps xmm1,xmm2,1100 ; detesto blend
	movdqu [r15],xmm1
	add r14, r9
	mov r15, r14 ;pone los iteradores de fila al comienzo de la fila siguiente (si tal existe, si no, apunta a basura)
	inc rax; aumenta la fila actual
        jmp ciclo_imagen    



	
	


pintar_negro: ;pinta de a 4 pixeles toda la primera fila de negro
	cmp r13, r11
	je fin_pintar_negro
	movdqu xmm1, [COLOR_NEGRO]
	movdqu [r15],xmm1	
	add r15, PIXEL_SIZE*PIXEL_PER_ITER	
	add r13, PIXEL_PER_ITER
	jmp pintar_negro
	
	
fin_pintar_negro:	
	add r14, r9
	mov r15, r14 ;pone los iteradores de fila al comienzo de la fila siguiente (si tal existe, si no, apunta a basura)
	inc rax; aumenta la fila actual
        jmp ciclo_imagen
	



	
        

        
fin_pintar:
	pop r10
	pop r11
	pop r14
	pop r15	
	pop r13
	pop r12
	mov rax, rsi
	ret
	


