section .rodata

Q_WORD_LEN equ 8

section .text

global invertirQW_asm

; void invertirQW_asm(uint64_t* p)

invertirQW_asm:
	; rdi <- p
	; no necesito prologo ni epilogo. Por lo que ahorro en instrucciones al no trabajar con la pila.
	movq xmm1, [rdi]; Primer quadword
	movq xmm2, [rdi+Q_WORD_LEN]; Segundo quadword. Como el primer quadword ocupa a lo sumo 8 bytes, debo aumentar en 8 bytes la posicion de memoria dada.
	movq [rdi],xmm2
	movq [rdi+Q_WORD_LEN],xmm1
	ret
