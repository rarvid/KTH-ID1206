bits 32

global  start
extern  myos

section .text

start:
    cli
    mov esp, stack
    call myos
    hlt

section .bss

resb 8192

stack: