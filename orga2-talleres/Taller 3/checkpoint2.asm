extern sumar_c
extern restar_c
;########### SECCION DE DATOS
section .data

;########### SECCION DE TEXTO (PROGRAMA)
section .text

;########### LISTA DE FUNCIONES EXPORTADAS

global alternate_sum_4
global alternate_sum_4_simplified
global alternate_sum_8
global product_2_f
global alternate_sum_4_using_c
global product_9_f

;########### DEFINICION DE FUNCIONES
; uint32_t alternate_sum_4(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4);
; registros: x1[rdi], x2[rsi], x3[rdx], x4[rcx]
alternate_sum_4:
	push rbp
	mov rbp , rsp ;alineada

	sub rdi, rsi
	sub rdx, rcx
	add rdi, rdx

	mov rax, rdi 
	;recordar que si la pila estaba alineada a 16 al hacer la llamada
	;con el push de RIP como efecto del CALL queda alineada a 8
	;epilogo
	pop rbp
	ret

; uint32_t alternate_sum_4_using_c(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4);
; registros: x1[rdi], x2[rsi], x3[rdx], x4[rcx]
alternate_sum_4_using_c:
	;prologo
	push rbp ; alineado a 16
	mov rbp,rsp
	push rbx ; desalineado???
	sub rsp, 0x8

	call restar_c
	mov rbx, rax
	mov rdi , rdx
	mov rsi , rcx
	call restar_c
	mov rdi , rbx
	mov rsi , rax
	call sumar_c

	;epilogo
	add rsp, 0x8
	pop rbx

	;suponiendo que la operacion sumar_c no desalinea la pila, a partir de aqui tendria que estar alineada
	pop rbp
	ret

; uint32_t alternate_sum_4_simplified(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4);
; registros: x1[rdi], x2[rsi], x3[rdx], x4[rcx]
alternate_sum_4_simplified:
	sub rdi, rsi
	sub rdx, rcx
	add rdi, rdx
	mov rax, rdi 

	ret
	;por lo que entendi, para esta funcion como no usa la pila no hace falya hacer el epilogo ni prologo


; uint32_t alternate_sum_8(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t x5, uint32_t x6, uint32_t x7, uint32_t x8);
; registros y pila: x1[rdi], x2[rsi], x3[rdx], x4[rcx], x5[r8], x6[r9], x7[RBP + 0x10], x8[RBP + 0x18]
;si los datos son de 8 bits, porque se le suma 0x10????
alternate_sum_8:
	;prologo
	push rbp
	mov rbp , rsp ;alineada
	push rbx 
	sub rsp, 0x8

	call alternate_sum_4_simplified ;esta funcion no usa la pila
	mov rbx, rax
	mov rdi, r8
	mov rsi, r9
	mov rdx,[rbp + 0x10]
	mov rcx,[rbp + 0x18]
	call alternate_sum_4_simplified ;esta funcion no usa la pila
	add rax , rbx 

	;epilogo
	add rsp, 0x8
	pop rbx
	pop rbp
	ret


; SUGERENCIA: investigar uso de instrucciones para convertir enteros a floats y viceversa
;void product_2_f(uint32_t * destination, uint32_t x1, float f1);
;registros: destination[rdi], x1[rsi], f1[XMM0]
product_2_f:
	push rbp
	mov rbp , rsp

	cvtsi2ss xmm1, rsi ;convertir el uint_32 en float
	mulss xmm0, xmm1 ;multiplicaacion entre floats
	cvttss2si eax, xmm0 ;elimino decimales y almaceno en rax

	mov [rdi], eax

	pop rbp
	ret


;extern void product_9_f(uint32_t * destination
;, uint32_t x1, float f1, uint32_t x2, float f2, uint32_t x3, float f3, uint32_t x4, float f4
;, uint32_t x5, float f5, uint32_t x6, float f6, uint32_t x7, float f7, uint32_t x8, float f8
;, uint32_t x9, float f9);
;registros y pila: destination[rdi], x1[rsi], f1[XMM0], x2[rdx], f2[XMM1], x3[rcx], f3[XMM2], x4[r8], f4[XMM3]
;	, x5[r9], f5[XMM4], x6[RBP + 0x10], f6[XMM5], x7[RBP + 0x18], f7[XMM6], x8[RBP + 0x26], f8[XMM7],
;	, x9[RBP + 0x34], f9[RBP + 0x42]
product_9_f:
	;prologo
	push rbp
	mov rbp, rsp

	;convertimos los flotantes de cada registro xmm en doubles
	cvtps2pd XMM0, XMM0
	cvtps2pd XMM1, XMM1
	cvtps2pd XMM2, XMM2
	cvtps2pd XMM3, XMM3
	cvtps2pd XMM4, XMM4
	cvtps2pd XMM5, XMM5
	cvtps2pd XMM6, XMM6
	cvtps2pd XMM7, XMM7

	;multiplicamos los doubles en xmm0 <- xmm0 * xmm1, xmmo * xmm2 , ...
	mulsd XMM0, XMM1
	mulsd XMM0, XMM2
	mulsd XMM0, XMM3
	mulsd XMM0, XMM4
	mulsd XMM0, XMM5
	mulsd XMM0, XMM6
	mulsd XMM0, XMM7
	
	;no olvidarse del ultimo float

	movss  XMM1, [RBP + 0x30]
	cvtps2pd XMM1, XMM1
	mulsd XMM0, XMM1



	; convertimos los enteros en doubles y los multiplicamos por xmm0.
	cvtsi2sd XMM1, rsi
	mulsd XMM0, XMM1
	cvtsi2sd XMM1, rdx
	mulsd XMM0, XMM1
	cvtsi2sd XMM1, rcx
	mulsd XMM0, XMM1
	cvtsi2sd XMM1, r8
	mulsd XMM0, XMM1
	cvtsi2sd XMM1, r9
	mulsd XMM0, XMM1
	cvtsi2sd XMM1, [RBP + 0x10]
	mulsd XMM0, XMM1
	cvtsi2sd XMM1, [RBP + 0x18]
	mulsd XMM0, XMM1
	cvtsi2sd XMM1, [RBP + 0x20]
	mulsd XMM0, XMM1
	cvtsi2sd XMM1, [RBP + 0x28]
	mulsd XMM0, XMM1
	
	movsd [rdi] , XMM0 

	; epilogo
	pop rbp
	ret

