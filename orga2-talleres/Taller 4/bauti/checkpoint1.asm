
section .text

global invertirQW_asm

; void invertirQW_asm(uint64_t* p)

invertirQW_asm:
	push rbp
	mov rbp, rsp

	mov rax , [rdi] 
	mov rbx , [rdi + 8]  
	mov [rdi], rbx
	mov [rdi + 8] , rax

	pop rbp
	ret
