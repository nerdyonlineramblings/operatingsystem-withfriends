[org 0x7c00]

KERNEL_LOCATION equ 0x7e00
BOOT_DISK: db 0
NUMBER_OF_SECTORS: db 0
    mov [BOOT_DISK], dl
    mov [NUMBER_OF_SECTORS], byte 20

    push diskReadMsg
    call printf
    pop ax
    mov al, [NUMBER_OF_SECTORS]
    mov ah, 0
    push ax
    call print_hex
    pop ax
    push newline
    call printf
    pop ax


    mov ax, 0
    mov es, ax ; extra segment
    mov ds, ax 
    mov bp, 0x8000 ; setting stack
    mov sp, bp

    mov bx, KERNEL_LOCATION ; segment offset
    mov al, [NUMBER_OF_SECTORS] ; number of sectors to read

    mov ah, 2 ; bios code

    mov ch, 0 ; cylinder number
    mov dh, 0 ; head number
    mov cl, 2 ; sector number (starts at 1 instead of 0)

    mov dl, [BOOT_DISK]
    int 0x13

    jc failed_to_read_disk
    cmp al, [NUMBER_OF_SECTORS]
    jne failed_to_read_disk


    mov ah, 0x0
    mov al, 0x3
    int 0x10


    jmp enter_protected_mode

    hlt

failed_to_read_disk:
    push diskReadError
    call printf
    pop di
    mov ah, 0
    push ax
    call print_hex
    pop ax

    hlt


print_hex: ; takes integer
    push bp
    mov bp, sp
    mov ch, 3
hex_printer_loop:
    mov bx, [bp + 4]

    mov al, 4
    mul ch
    mov cl, al

    shr bx, cl
    and bx, 0x000f
    cmp bl, 10
    jge hex_greater_then_ten
    mov al, 48
    jmp hex_step_print
hex_greater_then_ten:
    mov al, 55
hex_step_print:
    add al, bl
    mov ah, 0x0e
    int 0x10

    dec ch
    cmp ch, 0
    jl hex_printer_end
    jmp hex_printer_loop

hex_printer_end:
    mov sp, bp
    pop bp
    ret


printf:
    push bp
    mov bp, sp

    mov bx, [bp+4]
    mov ah, 0x0e
printf_loop:
    mov al, [bx]

    cmp al, 0
    je printf_end

    cmp al, '\'
    jne backslash_end
    inc bx
    mov al, [bx]
    cmp al, 'n'
    jne backslash_end
    mov al, 0x0a
    int 0x10
    mov al, 0x0d
backslash_end:

    int 0x10 ; print the value in al

    inc bx
    jmp printf_loop

printf_end:
    mov sp, bp
    pop bp
    ret

diskReadError:
    db "Critical Error: Failed to read disk. Sectors read:\n", 0
diskReadMsg:
    db "Reading disk. Number of sectore being read:\n", 0
newline:
    db "\n", 0



; Entering protected mode
GDT_Start:
    null_descriptor:
        dd 0
        dd 0
    code_descriptor:
        dw 0xffff ; first 16 bits of the limit
        dw 0 ; first 24 bits for the base
        db 0
        db 0b10011010 ; pres, priv, type, type flags
        db 0b11001111 ; other flags + last 4 bits of limit
        db 0 ; last 8 bits of the base
    data_descriptor:
        dw 0xffff ; first 16 bits of the limit
        dw 0 ; first 24 bits for the base
        db 0
        db 0b10010010 ; pres, priv, type, type flags
        db 0b11001111 ; other flags + last 4 bits of limit
        db 0 ; last 8 bits of the base
GDT_End:

GDT_Descriptor:
    dw GDT_End - GDT_Start - 1 ; size
    dd GDT_Start ; Start pointer

CODE_SEG equ code_descriptor - GDT_Start
DATA_SEG equ data_descriptor - GDT_Start

enter_protected_mode:
    cli
    lgdt [GDT_Descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:start_protected_mode

[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp

    jmp CODE_SEG:KERNEL_LOCATION 

    hlt

times 510-($-$$) db 0
dw 0xaa55
