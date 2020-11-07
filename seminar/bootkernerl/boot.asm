    ; MBR for 16 bit Intel 8086
    bits 16

start:
    mov ax, 0x07C0              ; 0x07C00 is where we are
    add ax, 0x20                ; add 0x20 (32) (when shifted 512)
    mov ss, ax                  ; set the stack segment
    mov sp, 0x1000              ; set the stack pointer

    mov ax, 0x07C0              ; set data segment
    mov ds, ax                  ; more about this later

    mov si, msg                 ; pointer to the message in SI (Source Index)
    mov ah, 0x0E                ; print char BIOS procedure

.next:
    lodsb                      ; next byte to AL, increment SI
    cmp al, 0                   ; if the byte is zero
    je .done                    ; jump to done
    int 0x10                    ; invoke the BIOS system call INT
    jmp .next                   ; loop

.done:
    jmp $                       ; loop forever

msg:
    db 'Hello', 0               ; the string we want to print
    times 510-($-$$) db 0       ; calucalte how many bytes left from here and fill upto 510 with zeros
    dw 0xAA55                   ; master boot record signature