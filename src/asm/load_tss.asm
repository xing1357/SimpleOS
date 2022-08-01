section .text
	global load_tss
	global get_eip

load_tss:
	mov ax, 0x2B
	ltr ax
	ret

get_eip:
	push eax
	jmp eax
	
