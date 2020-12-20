; Name: Harsh Kumar
; Roll No: 2019043

bits 16
ORG 0x7c00

CONSOLE EQU 0xb8000
COLOR EQU 0x07

start:
    cli                         ; Disable interrupts

    lgdt [gdtr]                 ; Load our GDT

    ; Setting protected mode flag
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE32_SEL:print      ; FAR JMP to set Code Segment

bits 32
print:
    mov esi, msg
    mov ebx, CONSOLE

    mov ch, 0
    mov cl, [0x451]
    mov eax, ecx                 ; EAX = cur_row

    mov cl, [0x44a]
    mul ecx                      ; EAX = cur_row * screen_width
    
    mov cl, [0x450]
    add eax, ecx                 ; EAX = cur_row * screen_width + cur_col
    lea edi, [ebx + eax * 2]     ; EDI = memory location of current screen cell

    mov ah, COLOR                ; Set color of text to be displayed
    jmp .getch
.repeat:
    stosw

.getch:
    lodsb                       ; Get character from string
    cmp al, 0
    jnz .repeat

.end:
    mov edx, cr0
    mov ah, COLOR
    mov ecx, 32

.loop:
    mov al, 48
    shl edx, 1
    adc eax, 0

    stosw

    sub ecx, 1
    cmp ecx, 0
    jz end_loop
    jmp .loop

end_loop:
    hlt
    jmp end_loop


msg: db "Hello World!     CR0: ", 0


align 8
gdt_start:
    dd 0
    dd 0
code:
    dw 0FFFFh
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0
data:
    dw 0FFFFh
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0
end_of_gdt:

gdtr:
    dw end_of_gdt - gdt_start - 1
    dd gdt_start

CODE32_SEL equ code - gdt_start
DATA32_SEL equ data - gdt_start

; add padding to make it 512 bytes, ending with 0xaa55
TIMES 510-($-$$) db  0
dw 0xaa55