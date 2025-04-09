

;########### ESTOS SON LOS OFFSETS Y TAMAÑO DE LOS STRUCTS
; Completar:
;NODO_LENGTH	EQU	 32 bytes
;LONGITUD_OFFSET	EQU	??
;Offset de next: 0 bytes 0x00
;Offset de categoria: 8 bytes 0x08
;Offset de arreglo: 16 bytes 0x10
;Offset de longitud: 24 bytes 0x18

;tamaño de lista = 8 bytes

;PACKED_NODO_LENGTH	EQU	21
;PACKED_LONGITUD_OFFSET	EQU	??
;Offset de next: 0 bytes 0x00
;Offset de categoria: 8 bytes 0x08
;Offset de arreglo: 9 bytes 0x09
;Offset de longitud: 17 bytes 0x11

;tamaño de packed_lista = 8 bytes


;########### SECCION DE DATOS
section .data

;########### SECCION DE TEXTO (PROGRAMA)
section .text

;########### LISTA DE FUNCIONES EXPORTADAS
global cantidad_total_de_elementos
global cantidad_total_de_elementos_packed

;########### DEFINICION DE FUNCIONES
;extern uint32_t cantidad_total_de_elementos(lista_t* lista);
;registros: lista[rdi]
cantidad_total_de_elementos:
	push rbp
	mov rbp , rsp ;alineada

	mov eax, 0
	cmp rdi, 0
	jz .fin
	cmp qword [rdi], 0
	mov rdi, [rdi]
	jz .fin

	.ciclo:
	add eax, [rdi + 0x18]
	mov rdi , [rdi]
	cmp rdi, 0
	jnz .ciclo
	
	.fin:
	pop rbp
	ret




;extern uint32_t cantidad_total_de_elementos_packed(packed_lista_t* lista);
;registros: lista[rdi]
cantidad_total_de_elementos_packed:
	push rbp
	mov rbp , rsp ;alineada

	mov eax, 0
	cmp rdi, 0
	jz .fin
	cmp qword [rdi], 0
	mov rdi, [rdi]
	jz .fin

	.ciclo_packed:
	add eax , [rdi + 0x11]
	mov rdi , [rdi]
	cmp rdi, 0
	jnz .ciclo_packed

	.fin:
	pop rbp
	ret



