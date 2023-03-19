.CODE

tp_hypercall PROC
    push rcx
	push rbx
	push rdi
	xor rdi,rdi
    mov eax,86E4CA11h
    mov rbx,rcx
	mov rcx,rdx
    cpuid
	mov rax,rdi
	pop rdi
	pop rbx
	pop rcx
	ret
tp_hypercall ENDP

END