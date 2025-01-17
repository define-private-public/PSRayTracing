Vec2::Vec2(double, double) [base object constructor]:   ; 14 instructions
        push    rbp
        mov     rbp, rsp
        mov     QWORD PTR [rbp-8], rdi
        movsd   QWORD PTR [rbp-16], xmm0
        movsd   QWORD PTR [rbp-24], xmm1
        mov     rax, QWORD PTR [rbp-8]
        movsd   xmm0, QWORD PTR [rbp-16]
        movsd   QWORD PTR [rax], xmm0
        mov     rax, QWORD PTR [rbp-8]
        movsd   xmm0, QWORD PTR [rbp-24]
        movsd   QWORD PTR [rax+8], xmm0
        nop
        pop     rbp
        ret
Vec2::length_squared() const:                           ; 16 instructions
        push    rbp
        mov     rbp, rsp
        mov     QWORD PTR [rbp-8], rdi
        mov     rax, QWORD PTR [rbp-8]
        movsd   xmm1, QWORD PTR [rax]
        mov     rax, QWORD PTR [rbp-8]
        movsd   xmm0, QWORD PTR [rax]
        mulsd   xmm1, xmm0
        mov     rax, QWORD PTR [rbp-8]
        movsd   xmm2, QWORD PTR [rax+8]
        mov     rax, QWORD PTR [rbp-8]
        movsd   xmm0, QWORD PTR [rax+8]
        mulsd   xmm0, xmm2
        addsd   xmm0, xmm1
        pop     rbp
        ret
RNG::num(double, double):                               ; 19 instructions
        push    rbp
        mov     rbp, rsp
        sub     rsp, 48
        mov     QWORD PTR [rbp-24], rdi
        movsd   QWORD PTR [rbp-32], xmm0
        movsd   QWORD PTR [rbp-40], xmm1
        mov     rax, QWORD PTR [rbp-24]
        lea     rdx, [rax+5000]
        mov     rax, QWORD PTR [rbp-24]
        mov     rsi, rax
        mov     rdi, rdx
        call    double std::uniform_real_distribution<double>::operator()<std::mersenne_twister_engine<unsigned long, 32ul, 624ul, 397ul, 31ul, 2567483615ul, 11ul, 4294967295ul, 7ul, 2636928640ul, 15ul, 4022730752ul, 18ul, 1812433253ul> >(std::mersenne_twister_engine<unsigned long, 32ul, 624ul, 397ul, 31ul, 2567483615ul, 11ul, 4294967295ul, 7ul, 2636928640ul, 15ul, 4022730752ul, 18ul, 1812433253ul>&)
        movq    rax, xmm0
        mov     QWORD PTR [rbp-8], rax
        movsd   xmm0, QWORD PTR [rbp-40]
        subsd   xmm0, QWORD PTR [rbp-32]
        mulsd   xmm0, QWORD PTR [rbp-8]
        addsd   xmm0, QWORD PTR [rbp-32]
        leave
        ret
RNG::rejection_in_unit_disk():                          ; 41 instructions
        push    rbp
        mov     rbp, rsp
        push    rbx
        sub     rsp, 40
        mov     QWORD PTR [rbp-40], rdi
.L12:
        movsd   xmm0, QWORD PTR .LC1[rip]
        mov     rdx, QWORD PTR .LC3[rip]
        mov     rax, QWORD PTR [rbp-40]
        movapd  xmm1, xmm0
        movq    xmm0, rdx
        mov     rdi, rax
        call    RNG::num(double, double)                ; 19 instructions
        movq    rbx, xmm0
        movsd   xmm0, QWORD PTR .LC1[rip]
        mov     rdx, QWORD PTR .LC3[rip]
        mov     rax, QWORD PTR [rbp-40]
        movapd  xmm1, xmm0
        movq    xmm0, rdx
        mov     rdi, rax
        call    RNG::num(double, double)                ; 19 instructions
        movq    rax, xmm0
        lea     rdx, [rbp-32]
        movq    xmm1, rbx
        movq    xmm0, rax
        mov     rdi, rdx
        call    Vec2::Vec2(double, double) [complete object constructor]        ; 14 instructions
        lea     rax, [rbp-32]
        mov     rdi, rax
        call    Vec2::length_squared() const                                    ; 16 instructions
        movsd   xmm1, QWORD PTR .LC1[rip]
        comisd  xmm1, xmm0
        seta    al
        test    al, al
        je      .L12                                                            ; Possible jump to repeat the above     (29 instructions)
        mov     rax, QWORD PTR [rbp-32]
        mov     rdx, QWORD PTR [rbp-24]
        movq    xmm0, rax
        movq    xmm1, rdx
        mov     rbx, QWORD PTR [rbp-8]
        leave
        ret

--------

    This needs to traverse quite a few instructions, perform calls (and can loop infinately).  Let's say we had success on our first go:
        41 + 19 + 19 + 14 + 16: 109 instructions (w/ 4 calls)
    
    If we had one failure but then a success, so the loop is run twice:
        5 + 2 * (29 + 19 + 19 + 14 + 16) + 7: 207 instructions (w/ 8 calls)
    
    I may be a little off on the math
