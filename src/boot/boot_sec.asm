[org 0x7c00]

mov [BOOT_DRIVE], dl

mov bp, 0x9000
mov sp,bp

mov si, hello_string
call print_rm

mov si, init_message_rm
call print_rm
call load_kernel
call switch_to_pm

jmp $

load_kernel:
mov si,load_kernel_msg
call print_rm
mov bx ,KERNEL_OFFSET
mov dh ,15
mov dl, [BOOT_DRIVE]
call disk_load
ret

[BITS 32]
BEGIN_PM:
mov ebx ,init_message_pm
call print_pm
call KERNEL_OFFSET
jmp $

;include

%include "..\SO\src\boot\print_rm.asm"
%include "..\SO\src\boot\gdt.asm"
%include "..\SO\src\boot\print_pm.asm"
%include "..\SO\src\boot\switch_to_pm.asm"
%include "..\SO\src\boot\disk_load.asm"

;data

hello_string:
db "Hello Jarvis",0x0A,0

init_message_rm:
db 13,"Executing in Real Mode.",0x0A,0

init_message_pm:
db 13,"Executing in Protect Mode.",0x0A,0

load_kernel_msg:
db 13,"Loading kernel",0x0A,0

BOOT_DRIVE: 
db 0

KERNEL_OFFSET equ 0x1000

times 510 -($-$$) db 0 
dw 0xaa55
