; ** por compatibilidad se omiten tildes **
; ==============================================================================
; System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
;
; Definicion de rutinas de atencion de interrupciones

%include "print.mac"
%define CS_RING_0_SEL    (1 << 3)
%define SHARED_TICK_COUNT 0x1D000

BITS 32

;; PIC
extern pic_finish1
extern kernel_exception
extern page_fault_handler
extern process_scancode
sched_task_offset:     dd 0xFFFFFFFF
sched_task_selector:   dw 0xFFFF


;; Tasks
extern tasks_tick
extern tasks_screen_update
extern tasks_syscall_draw
extern tasks_input_process

;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISRc 1
    push DWORD %1
    ; Stack State:
    ; [ INTERRUPT #] esp
    ; [ ERROR CODE ] esp + 0x04
    ; [ EIP        ] esp + 0x08
    ; [ CS         ] esp + 0x0c
    ; [ EFLAGS     ] esp + 0x10
    ; [ ESP        ] esp + 0x14 (if DPL(cs) == 3)
    ; [ SS         ] esp + 0x18 (if DPL(cs) == 3)

    ; GREGS
    pushad
    ; Check for privilege change before anything else.
    mov edx, [esp + (8*4 + 3*4)]

    ; SREGS
    xor eax, eax
    mov ax, ss
    push eax
    mov ax, gs
    push eax
    mov ax, fs
    push eax
    mov ax, es
    push eax
    mov ax, ds
    push eax
    push eax ; cs

    ; CREGS
    mov eax, cr4
    push eax
    mov eax, cr3
    push eax
    mov eax, cr2
    push eax
    mov eax, cr0
    push eax

    cmp edx, CS_RING_0_SEL
    je .ring0_exception


      ; COMPLETAR (opcional):
    ;   Si caemos acá es porque una tarea causó una excepción
    ;   En lugar de frenar el sistema podríamos matar la tarea (o reiniciarla)
    ;   ¿Cómo harían eso?
    call kernel_exception
    add esp, 10*4
    popad


     xchg bx, bx
    jmp $

    ;call ring3_exception
    jmp $


.ring0_exception:
    call kernel_exception
    add esp, 10*4
    popad

    xchg bx, bx
    jmp $

%endmacro

; ISR that pushes an exception code.
%macro ISRE 1
global _isr%1

_isr%1:
  ISRc %1
%endmacro

; ISR That doesn't push an exception code.
%macro ISRNE 1
global _isr%1

_isr%1:
  push DWORD 0x0
 ISRc %1
%endmacro

;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISRNE 0
ISRNE 1
ISRNE 2
ISRNE 3
ISRNE 4
ISRNE 5
ISRNE 6
ISRNE 7
ISRE 8
ISRNE 9
ISRE 10
ISRE 11
ISRE 12
ISRE 13
;ISRE 14
ISRNE 15
ISRNE 16
ISRE 17
ISRNE 18
ISRNE 19
ISRNE 20






;; Rutina de atención de Page Fault(Catedra)
;global _isr14

;_isr14:
    ; Estamos en un page fault.
 ;   pushad
    ; COMPLETAR: llamar rutina de atención de page fault, pasandole la dirección que se intentó acceder
  ;  .ring0_exception:
    ; Si llegamos hasta aca es que cometimos un page fault fuera del area compartida.
   ; call kernel_exception
    ;jmp $

 ;   .fin:
  ;  popad
  ;  add esp, 4 ; error code
   ; iret



global _isr14
_isr14:
    pushad
    mov eax,cr2
    push eax
    
    call page_fault_handler

    cmp BYTE al,0x0 ; valor de retorno si no pudo atender el page fault
    je page_fault_not_res

    pop eax
    popad
    add esp, 4 ; quito el codigo de error
    iret

page_fault_not_res:
    pop eax
    popad
    add esp, 4
    jmp page_fault_halt

page_fault_halt:
    jmp page_fault_halt



;; Rutina de atención del RELOJ (catedra)
;; -------------------------------------------------------------------------- ;;
;global _isr32
; COMPLETAR: Implementar la rutina
;_isr32:
 ;   pushad
    ; 1. Le decimos al PIC que vamos a atender la interrupción
 ;   call pic_finish1
  ;  call next_clock
    ; 2. Realizamos el cambio de tareas en caso de ser necesario
   ; call sched_next_task
   ; cmp ax, 0
   ; je .fin

    ;str bx
    ;cmp ax, bx
    ;je .fin

    ;mov word [sched_task_selector], ax
    ;jmp far [sched_task_offset]

    ;.fin:
    ; 3. Actualizamos las estructuras compartidas ante el tick del reloj
    ;call tasks_tick
    ; 4. Actualizamos la "interfaz" del sistema en pantalla
    ;call tasks_screen_update
    ;popad
    ;iret


;; Rutina de atención del RELOJ (pre-tasks)
;; -------------------------------------------------------------------------- ;;



global _isr32




clockNumber:           dd 41
; COMPLETAR: Implementar la rutina
_isr32:
    pushad
    dec DWORD [clockNumber]
    mov ebx, [clockNumber]
    cmp ebx, 0
    je secondPassed
    jmp fin_clock
    
    
secondPassed:
    
    mov DWORD [clockNumber],41
    call next_clock
    ;jmp fin_clock
fin_clock:

    call pic_finish1
    popad
    iret

;; Rutina de atención del TECLADO
;; -----------------------------  IDT_ENTRY3(88); 
  ;IDT_ENTRY3(98); --------------------------------------------- ;;
global _isr33
; COMPLETAR: Implementar la rutina





_isr33:
    pushad
    xor eax,eax
    in al,0x60
    push eax
    

    call process_scancode


    
    pop eax

    popad
    iret




;; Rutinas de atención de las SYSCALLS(catedra)
;; -------------------------------------------------------------------------- ;;

;global _isr88
; Syscall para que una tarea dibuje en su pantalla
;_isr88:
;  pushad
;  push eax
;  call tasks_syscall_draw
;  add esp, 4
;  popad
;  iret

;global _isr98
;_isr98:
;  mov eax, 0x62
;  iret

; PushAD Order
%define offset_EAX 28
%define offset_ECX 24
%define offset_EDX 20
%define offset_EBX 16
%define offset_ESP 12
%define offset_EBP 8
%define offset_ESI 4
%define offset_EDI 0

;; Rutinas de atención de las SYSCALLS(pre tasks)
;; -------------------------------------------------------------------------- ;;

global _isr88
; COMPLETAR: Implementar la rutina
_isr88:
    pushad
    
    mov eax,0x58


    popad
    iret

global _isr98
; COMPLETAR: Implementar la rutina
_isr98:
    pushad
    mov eax,0x62
    popad
    iret

; PushAD Order
%define offset_EAX 28
%define offset_ECX 24
%define offset_EDX 20
%define offset_EBX 16
%define offset_ESP 12
%define offset_EBP 8
%define offset_ESI 4
%define offset_EDI 0


;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
next_clock:
        pushad	
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0B, 49, 79
                popad
        ret
