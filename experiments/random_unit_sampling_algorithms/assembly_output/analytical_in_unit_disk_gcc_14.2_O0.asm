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
RNG::analytical_in_unit_disk():                         ; 49 instructions
        push    rbp
        mov     rbp, rsp
        sub     rsp, 64
        mov     QWORD PTR [rbp-56], rdi
        movsd   xmm0, QWORD PTR .LC1[rip]
        mov     rax, QWORD PTR [rbp-56]
        movapd  xmm1, xmm0
        mov     rdx, QWORD PTR .LC2[rip]
        movq    xmm0, rdx
        mov     rdi, rax
        call    RNG::num(double, double)                ; 19 instructions
        movq    rax, xmm0
        movq    xmm0, rax
        call    sqrt
        movq    rax, xmm0
        mov     QWORD PTR [rbp-8], rax
        movsd   xmm0, QWORD PTR .LC3[rip]
        mov     rax, QWORD PTR [rbp-56]
        movapd  xmm1, xmm0
        mov     rdx, QWORD PTR .LC2[rip]
        movq    xmm0, rdx
        mov     rdi, rax
        call    RNG::num(double, double)                ; 19 instructions
        movq    rax, xmm0
        mov     QWORD PTR [rbp-16], rax
        mov     rax, QWORD PTR [rbp-16]
        movq    xmm0, rax
        call    cos
        movsd   xmm1, QWORD PTR [rbp-8]
        mulsd   xmm0, xmm1
        movsd   QWORD PTR [rbp-24], xmm0
        mov     rax, QWORD PTR [rbp-16]
        movq    xmm0, rax
        call    sin
        movsd   xmm1, QWORD PTR [rbp-8]
        mulsd   xmm0, xmm1
        movsd   QWORD PTR [rbp-32], xmm0
        movsd   xmm0, QWORD PTR [rbp-32]
        mov     rdx, QWORD PTR [rbp-24]
        lea     rax, [rbp-48]
        movapd  xmm1, xmm0
        movq    xmm0, rdx
        mov     rdi, rax
        call    Vec2::Vec2(double, double) [complete object constructor]        ; 14 instructions
        mov     rax, QWORD PTR [rbp-48]
        mov     rdx, QWORD PTR [rbp-40]
        movq    xmm0, rax
        movq    xmm1, rdx
        leave
        ret 

--------

    The above method:

    (49 - 3) + 19 + 19 + 14: 98 instrunctions  (albiet 6 calls)
