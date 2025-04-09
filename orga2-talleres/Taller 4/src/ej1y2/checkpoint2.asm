section .rodata

WORD_LEN equ 2
DWORD_LEN equ 4
 
section .text

global checksum_asm

; uint8_t checksum_asm(void* array, uint32_t n)

checksum_asm:
	;rdi <- array, rsi <- n
	;nota: pila desalineada
	;Para cada set A,B,C necesitaré por lo menos 6 registros XMM : Como el resultado de A+B no es garantizado de ser de menos de 17 bits, y mucho menos eso multiplicado por 8, debo guardar estos como resultados de 32 bits. Ahora bien, las packed sum empaquetan en el operando destino la suma de los operandos paquete de tanto operando fuente como operando destino, es decir, que tengo que guardar tanto los Ai como los Bi como paquetes de 32 bits. Esto nos fuerza a usar 4 registros de 128. Dos para enpaquetar 4 valores de 32 bits de Ai cada uno,  y otros dos para B. Los dos restantes empaquetan los valores de C, que ya de por si son de 32 bits	
	
	xor eax,eax;setea eax a 0 (devuelve un int de 8 bits, uso eax)
	
	
ciclo:
	cmp esi,0; n es de 32 bits. Uso esi
	jz iguales
		
		
		
	pmovzxwd xmm0,[rdi]; Extiende los primeros 4 Ai y los empaqueta como valores de 32 bits. Extension sin signo 
	add rdi, WORD_LEN*4
	pmovzxwd xmm1,[rdi]; Extiende los otros 4 Ai y los empaqueta como valores de 32 bits. Extension sin signo 
	add rdi, WORD_LEN*4
	pmovzxwd xmm2,[rdi]; Lo mismo para B 
	add rdi, WORD_LEN*4
	pmovzxwd xmm3,[rdi];  
	add rdi, WORD_LEN*4
	
	
	movdqu xmm4,[rdi]; los Ci altos de un set
	add rdi, DWORD_LEN*4
	movdqu xmm5,[rdi]; los Ci bajos de un set
	add rdi, DWORD_LEN*4
	
	
	
	paddd xmm0,xmm2; Suma los primeros 4 ai con los primeros 4 bi
	
	pslld xmm0,3; multiplica por 8 (shiftea 3 veces a izquierda cada paquete
	
	pcmpeqd xmm0,xmm4; compara el resultado de los ai,bi altos con los ci altos
	packssdw xmm0,xmm0; comprime los valores para poder compararlo y ver el flag de cero
	movq r9,xmm0
	not r9;
	cmp r9,0
	jnz desiguales; salto temprano, los valores altos son desiguales
	
	
	
	
	paddd xmm1,xmm3; suma los restantes	
	pslld xmm1,3
	pcmpeqd xmm1,xmm5; compara el resultado de los ai,bi bajos con los ci bajos. Misma historia
	packssdw xmm0,xmm0; comprime los valores para poder compararlo y ver el flag de cero
	movq r9,xmm0
	not r9;
	cmp r9,0
	jnz desiguales
	
	
	
	dec rsi; dado que los valores son iguales por ahora, decremento n y sigo iterando		
	jmp ciclo


desiguales:
	jmp fin_ciclo; eax ya esta seteado a 0		
	
iguales:
	inc eax;eax estaba en 0, por ende, eax sera 1
	jmp fin_ciclo	
	
fin_ciclo:
	ret

