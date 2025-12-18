.intel_syntax noprefix
.global switch_proc
.type switch_proc, @function
switch_proc:
    # rdi = old, rsi = new

    # Save old context

    # Callee saved registers
    mov [rdi + 0x00], rbx
    mov [rdi + 0x08], rbp
    mov [rdi + 0x10], r12
    mov [rdi + 0x18], r13
    mov [rdi + 0x20], r14
    mov [rdi + 0x28], r15

    # Save stack pointer
    mov [rdi + 0x30], rsp

    # Load new context

    # Preserve pointer
    mov rax, rsi

    # Callee saved registers
    mov rbx, [rax + 0x00]
    mov rbp, [rax + 0x08]
    mov r12, [rax + 0x10]
    mov r13, [rax + 0x18]
    mov r14, [rax + 0x20]
    mov r15, [rax + 0x28]

    # Load stack
    mov rsp, [rax + 0x30]

    ret
