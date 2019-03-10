print_rm:
pusha
mov ah,0x0e ;BIOS video device
.loop lodsb
cmp al,0
je .end
int 0x10 ;print interrup
jmp .loop
.end ;sair do loop
popa
ret