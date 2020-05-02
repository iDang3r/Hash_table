//
//  asm_code.s
//  Hash_table
//
//  Created by Александр on 29.04.2020.
//  Copyright © 2020 Alex. All rights reserved.
//

.intel_syntax noprefix

.text

.globl _myStrlen
.globl _myStrcmp

_myStrlen:

    //push rcx
    xor rcx, rcx

    mov rax, rdi
repeat:
    
    mov cl, [rax]
    inc rax
    
    cmp cl, 0
    jne repeat
    
    sub rax, rdi
    dec rax
    //pop rcx

    ret

_myStrcmp:
    
    //push rcx
    xor rcx, rcx

repeat_:
    
    mov ch, [rdi]
    mov cl, [rsi]

    cmp cl, 0
    je repeat_break_

    inc rdi
    inc rsi

    cmp ch, cl
    je repeat_

    dec rdi
    dec rsi

repeat_break_:

    xor rax, rax
    mov al, [rdi]
    sub al, [rsi]

    //pop rcx
    
    ret
