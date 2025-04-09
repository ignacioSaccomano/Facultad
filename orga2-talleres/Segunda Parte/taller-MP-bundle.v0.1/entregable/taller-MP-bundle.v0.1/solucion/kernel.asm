; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"
global start
extern pic_enable
extern pic_reset
extern idt_init
extern screen_draw_layout
extern GDT_DESC
extern IDT_DESC

extern tlb_flush
extern mmu_init_kernel_dir
extern copy_page
extern mmu_init_task_dir

extern page_fault_handler
; COMPLETAR - Agreguen declaraciones extern según vayan necesitando

extern A20_enable
extern A20_check
; COMPLETAR - Definan correctamente estas constantes cuando las necesiten
%define CS_RING_0_SEL 0x0008 ; = 0000 0000 0000 1000: (gdt[1]) << 3 | 0
%define DS_RING_0_SEL 0x0018 ; = 0000 0000 0001 1S000: (gdt[3]) << 3 | 0


BITS 16
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

stack_base equ 0x00025000
;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
	

BITS 16
start:
    ; COMPLETAR - Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font
    
    
    
    
    
    ; COMPLETAR - Imprimir mensaje de bienvenida - MODO REAL
     ; (revisar las funciones definidas en print.mac y los mensajes se encuentran en la
    ; sección de datos)

    
    print_text_rm start_rm_msg, start_rm_len, 0x3, 0, 0
    
    
   
    ; COMPLETAR - Habilitar A20
    ; (revisar las funciones definidas en a20.asm) 
    call A20_enable
    

;inicializacion de mmu

    ; COMPLETAR - Cargar la GDT
    
    lgdt [GDT_DESC]




    

     ;COMPLETAR - Setear el bit PE del registro CR0
    mov eax,cr0
    or eax,1 ; deja el ultimo bit en 1;inicializacion de mmu, al mover a cr0 nos deja en modo protegido ( nota, en cr0 esta tambien seteado el bit ET. Mantenemos este bit, con todo aquel que y estaba seteado en cr0)
    mov cr0,eax;
    ; COMPLETAR - Saltar a modo protegido (far jump)
    
    jmp CS_RING_0_SEL:modo_protegido
    ; (recuerden que un far jmp se especifica como jmp CS_selector:address)
    ; Pueden usar la constante CS_RING_0_SEL definida en este archivo

BITS 32
modo_protegido:
    ; COMPLETAR - A partir de aca, todo el codigo se va a ejectutar en modo protegido
    ; Establecer selectores de segmentos DS, ES, GS, FS y SS en el segmento de datos de nivel 0
    

    ; Pueden usar la constante DS_RING_0_SEL definida en este archivo
	mov ax,DS_RING_0_SEL
    mov ss,ax 
    mov ds,ax
    mov es,ax
    mov gs,ax
    mov fs,ax
    
   

    ; COMPLETAR - Establecer el tope y la base de la pila
    
    mov edx,stack_base
    mov esp,edx
    mov ebp,esp

    ; COMPLETAR - Imprimir mensaje de bienvenida - MODO PROTEGIDO
    print_text_pm start_pm_msg, start_pm_len, 0x9, 0x0, 0x0
    ; COMPLETAR - Inicializar pantalla
    


    
    call screen_draw_layout

    



    ; Cargo la IDT
interrupciones:
     
     lidt [IDT_DESC]
     call idt_init

post_idt:
   ; Inicializo los pic
     call pic_reset
     call pic_enable

paginacion_init:
    
    call mmu_init_kernel_dir
    
    ;inicializacion de mmu
    
    mov cr3,eax
    



    mov eax,cr0
    or eax, 0x80010000
    mov cr0,eax
    


    

post_pic:
     sti
     int 0x20 
     int 0x21
     int 88
     int 98


paginacion_test:
    mov ebx,0x500000
    mov eax,0x00001000
    push eax
    push ebx
    call copy_page
    pop ebx
    pop eax


task_init:    
    mov eax, 0x18000
    push eax
    call mmu_init_task_dir 
    mov cr3,eax
    call tlb_flush;

    ;test del page handler
    mov byte [0x07000002],999
    mov byte [0x07000003],999


    ;check de fuera de rango
    ;mov byte [0x03000003],1
    


    


timer:

    ;mov al, 0x00
    ;out 0x40,al
    ;mov al, 0x80
    ;out 0x40,al
    
    mov al, 0xAE; 
    out 0x40,al
    mov al, 0x71; pongo 29102 como valor maximo del contador. sabiendo que 29102*41 es el contador por segundo
    out 0x40,al
    
    

    ; Ciclar infinitamente 
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"


