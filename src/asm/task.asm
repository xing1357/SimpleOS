global write_port
extern timer_handler_main

write_port:
	mov   edx, [esp + 4]    
	mov   al, [esp + 4 + 4]  
	out   dx, al  
	ret

global timer_handler
timer_handler:
    ;cli ; no more interrupts

    pushad ; save all registers

    push esp ; send the ESP to timer_handler_main
    push ebp ; send the EBP to timer_handler_main

    call timer_handler_main
    ;hlt ; we should not get here, halt the cpu

global switch_stack_and_jump
switch_stack_and_jump:
    mov ebx, [esp + 8] ; second argument, the function pointer
    mov eax, [esp + 4] ; first argument, a location to build a stack
    mov esp, eax ; set the stack pointer to the location
    mov ebp, esp ; by setting the EBP to the ESP, we configure a new stack
    sti ; enable interrupts
    jmp ebx ; jump to the function pointer

global switch_stack
switch_stack:
    mov ebx, [esp + 8] ; second argument, the stack frame pointer
    mov eax, [esp + 4] ; first argument, the stack pointer

    mov ebp, ebx ; restore the stack frame
    mov esp, eax ; restore the top of the stack

    popad ; restore all registers

    sti ; OK for interupts at this point
    iretd ; perform a return from interrupt since the last task was interrupted

