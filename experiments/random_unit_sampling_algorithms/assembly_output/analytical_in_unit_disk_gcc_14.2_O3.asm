.LC9:
        .string " "
main:
        push    r14
        xor     edx, edx
        push    r13
        push    r12
        mov     r12d, 1
        push    rbp
        push    rbx
        sub     rsp, 5056
        mov     QWORD PTR [rsp+32], 0
        lea     rbx, [rsp+32]
.L4:
        mov     rax, rdx
        shr     rax, 30
        xor     rax, rdx
        imul    rax, rax, 1812433253
        lea     edx, [rax+r12]
        mov     QWORD PTR [rbx+r12*8], rdx
        add     r12, 1
        cmp     r12, 624
        jne     .L4
        mov     eax, 2147483647
        mov     ebp, 10000000
        movq    xmm3, rax
        punpcklqdq      xmm3, xmm3
        jmp     .L5
.L44:
        divsd   xmm6, QWORD PTR [rsp+8]
        comisd  xmm6, QWORD PTR .LC1[rip]
        jnb     .L30
        pxor    xmm0, xmm0
        addsd   xmm6, xmm0
        ucomisd xmm0, xmm6
        ja      .L42
        sqrtsd  xmm6, xmm6
.L16:
        mov     rax, QWORD PTR .LC1[rip]
        mov     ecx, 2
        pxor    xmm7, xmm7
        mov     QWORD PTR [rsp+8], rax
        cmp     r12, 624
        je      .L17
.L46:
        mov     rdx, QWORD PTR [rsp+32+r12*8]
        add     r12, 1
.L18:
        mov     rax, rdx
        shr     rax, 11
        mov     eax, eax
        xor     rax, rdx
        mov     rdx, rax
        sal     rdx, 7
        and     edx, 2636928640
        xor     rax, rdx
        mov     rdx, rax
        sal     rdx, 15
        and     edx, 4022730752
        xor     rax, rdx
        mov     rdx, rax
        shr     rdx, 18
        xor     rax, rdx
        js      .L21
        pxor    xmm0, xmm0
        cvtsi2sd        xmm0, rax
.L22:
        mulsd   xmm0, QWORD PTR [rsp+8]
        fld     DWORD PTR .LC7[rip]
        fmul    QWORD PTR [rsp+8]
        addsd   xmm7, xmm0
        fstp    QWORD PTR [rsp+8]
        cmp     ecx, 1
        jne     .L31
        divsd   xmm7, QWORD PTR [rsp+8]
        comisd  xmm7, QWORD PTR .LC1[rip]
        jnb     .L32
        pxor    xmm0, xmm0
        lea     rdi, [rsp+24]
        lea     rsi, [rsp+16]
        movsd   QWORD PTR [rsp+8], xmm6
        addsd   xmm7, xmm0
        mulsd   xmm7, QWORD PTR .LC8[rip]
        addsd   xmm0, xmm7
        call    sincos
        movsd   xmm6, QWORD PTR [rsp+8]
        movsd   xmm0, QWORD PTR [rsp+16]
        movsd   xmm1, QWORD PTR [rsp+24]
        mulsd   xmm0, xmm6
.L24:
        mulsd   xmm6, xmm1
        mov     edi, OFFSET FLAT:std::cout
        movq    r14, xmm6
        call    std::basic_ostream<char, std::char_traits<char> >& std::basic_ostream<char, std::char_traits<char> >::_M_insert<double>(double)
        mov     edx, 1
        mov     esi, OFFSET FLAT:.LC9
        mov     r13, rax
        mov     rdi, rax
        call    std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)
        movq    xmm0, r14
        mov     rdi, r13
        call    std::basic_ostream<char, std::char_traits<char> >& std::basic_ostream<char, std::char_traits<char> >::_M_insert<double>(double)
        mov     r13, rax
        mov     rax, QWORD PTR [rax]
        mov     rax, QWORD PTR [rax-24]
        mov     r14, QWORD PTR [r13+240+rax]
        test    r14, r14
        je      .L41
        cmp     BYTE PTR [r14+56], 0
        je      .L26
        movsx   esi, BYTE PTR [r14+67]
.L27:
        mov     rdi, r13
        call    std::basic_ostream<char, std::char_traits<char> >::put(char)
        mov     rdi, rax
        call    std::basic_ostream<char, std::char_traits<char> >::flush()
        sub     ebp, 1
        movdqa  xmm3, XMMWORD PTR .LC4[rip]
        je      .L28
.L5:
        mov     rax, QWORD PTR .LC1[rip]
        mov     ecx, 2
        pxor    xmm6, xmm6
        mov     QWORD PTR [rsp+8], rax
        cmp     r12, 624
        je      .L6
.L45:
        mov     rdx, QWORD PTR [rsp+32+r12*8]
        add     r12, 1
.L7:
        mov     rax, rdx
        shr     rax, 11
        mov     eax, eax
        xor     rax, rdx
        mov     rdx, rax
        sal     rdx, 7
        and     edx, 2636928640
        xor     rax, rdx
        mov     rdx, rax
        sal     rdx, 15
        and     edx, 4022730752
        xor     rax, rdx
        mov     rdx, rax
        shr     rdx, 18
        xor     rax, rdx
        js      .L10
        pxor    xmm0, xmm0
        cvtsi2sd        xmm0, rax
.L11:
        mulsd   xmm0, QWORD PTR [rsp+8]
        fld     DWORD PTR .LC7[rip]
        fmul    QWORD PTR [rsp+8]
        addsd   xmm6, xmm0
        fstp    QWORD PTR [rsp+8]
        cmp     ecx, 1
        je      .L44
        mov     ecx, 1
        cmp     r12, 624
        jne     .L45
.L6:
        mov     rdi, -2147483648
        lea     rdx, [rsp+1840]
        mov     rax, rbx
        movq    xmm5, rdi
        mov     edi, 1
        movq    xmm4, rdi
        punpcklqdq      xmm5, xmm5
        punpcklqdq      xmm4, xmm4
.L8:
        movdqu  xmm1, XMMWORD PTR [rax+8]
        movdqa  xmm0, XMMWORD PTR [rax]
        add     rax, 16
        movdqu  xmm7, XMMWORD PTR [rax+3160]
        pand    xmm0, xmm5
        pand    xmm1, xmm3
        por     xmm1, xmm0
        movdqa  xmm2, xmm1
        pand    xmm1, xmm4
        movdqa  xmm0, xmm1
        psrlq   xmm2, 1
        psllq   xmm0, 3
        pxor    xmm2, xmm7
        paddq   xmm0, xmm1
        psllq   xmm0, 9
        paddq   xmm0, xmm1
        psllq   xmm0, 5
        paddq   xmm0, xmm1
        psllq   xmm0, 2
        psubq   xmm0, xmm1
        psllq   xmm0, 3
        psubq   xmm0, xmm1
        movdqa  xmm7, xmm0
        psllq   xmm7, 4
        paddq   xmm0, xmm7
        psllq   xmm0, 5
        psubq   xmm0, xmm1
        pxor    xmm2, xmm0
        movaps  XMMWORD PTR [rax-16], xmm2
        cmp     rdx, rax
        jne     .L8
        mov     rdx, QWORD PTR [rsp+1840]
        mov     rax, QWORD PTR [rsp+1848]
        mov     rdi, -2147483648
        movq    xmm7, rdi
        mov     edi, 1
        and     rdx, -2147483648
        and     eax, 2147483647
        movq    xmm5, rdi
        or      rax, rdx
        punpcklqdq      xmm7, xmm7
        punpcklqdq      xmm5, xmm5
        mov     rdx, rax
        and     eax, 1
        neg     rax
        shr     rdx
        xor     rdx, QWORD PTR [rsp+5016]
        and     eax, 2567483615
        xor     rax, rdx
        lea     rdx, [rbx+4984]
        mov     QWORD PTR [rsp+1840], rax
        lea     rax, [rsp+1848]
.L9:
        movdqa  xmm2, XMMWORD PTR [rax+8]
        movdqu  xmm0, XMMWORD PTR [rax]
        add     rax, 16
        pand    xmm0, xmm7
        pand    xmm2, xmm3
        por     xmm2, xmm0
        movdqa  xmm4, xmm2
        pand    xmm2, xmm5
        movdqa  xmm1, xmm2
        psrlq   xmm4, 1
        pxor    xmm4, XMMWORD PTR [rax-1832]
        psllq   xmm1, 3
        paddq   xmm1, xmm2
        psllq   xmm1, 9
        paddq   xmm1, xmm2
        psllq   xmm1, 5
        paddq   xmm1, xmm2
        psllq   xmm1, 2
        psubq   xmm1, xmm2
        psllq   xmm1, 3
        psubq   xmm1, xmm2
        movdqa  xmm0, xmm1
        psllq   xmm0, 4
        paddq   xmm1, xmm0
        psllq   xmm1, 5
        psubq   xmm1, xmm2
        pxor    xmm4, xmm1
        movups  XMMWORD PTR [rax-16], xmm4
        cmp     rax, rdx
        jne     .L9
        mov     rdx, QWORD PTR [rsp+32]
        mov     rax, QWORD PTR [rsp+5016]
        mov     r12d, 1
        mov     rsi, rdx
        and     rax, -2147483648
        and     esi, 2147483647
        or      rax, rsi
        mov     rsi, rax
        and     eax, 1
        neg     rax
        shr     rsi
        xor     rsi, QWORD PTR [rsp+3200]
        and     eax, 2567483615
        xor     rax, rsi
        mov     QWORD PTR [rsp+5016], rax
        jmp     .L7
.L31:
        mov     ecx, 1
        cmp     r12, 624
        jne     .L46
.L17:
        mov     rdi, -2147483648
        lea     rdx, [rsp+1840]
        mov     rax, rbx
        movq    xmm5, rdi
        mov     edi, 1
        movq    xmm4, rdi
        punpcklqdq      xmm5, xmm5
        punpcklqdq      xmm4, xmm4
.L19:
        movdqu  xmm1, XMMWORD PTR [rax+8]
        movdqa  xmm0, XMMWORD PTR [rax]
        add     rax, 16
        pand    xmm0, xmm5
        pand    xmm1, xmm3
        por     xmm1, xmm0
        movdqu  xmm0, XMMWORD PTR [rax+3160]
        movdqa  xmm2, xmm1
        pand    xmm1, xmm4
        psrlq   xmm2, 1
        pxor    xmm2, xmm0
        movdqa  xmm0, xmm1
        psllq   xmm0, 3
        paddq   xmm0, xmm1
        psllq   xmm0, 9
        paddq   xmm0, xmm1
        psllq   xmm0, 5
        paddq   xmm0, xmm1
        psllq   xmm0, 2
        psubq   xmm0, xmm1
        psllq   xmm0, 3
        psubq   xmm0, xmm1
        movdqa  xmm8, xmm0
        psllq   xmm8, 4
        paddq   xmm0, xmm8
        psllq   xmm0, 5
        psubq   xmm0, xmm1
        pxor    xmm2, xmm0
        movaps  XMMWORD PTR [rax-16], xmm2
        cmp     rdx, rax
        jne     .L19
        mov     rdx, QWORD PTR [rsp+1840]
        mov     rax, QWORD PTR [rsp+1848]
        mov     rdi, -2147483648
        movq    xmm5, rdi
        mov     edi, 1
        and     rdx, -2147483648
        and     eax, 2147483647
        movq    xmm4, rdi
        or      rax, rdx
        punpcklqdq      xmm5, xmm5
        punpcklqdq      xmm4, xmm4
        mov     rdx, rax
        and     eax, 1
        neg     rax
        shr     rdx
        xor     rdx, QWORD PTR [rsp+5016]
        and     eax, 2567483615
        xor     rax, rdx
        lea     rdx, [rbx+4984]
        mov     QWORD PTR [rsp+1840], rax
        lea     rax, [rsp+1848]
.L20:
        movdqa  xmm1, XMMWORD PTR [rax+8]
        movdqu  xmm0, XMMWORD PTR [rax]
        add     rax, 16
        pand    xmm0, xmm5
        pand    xmm1, xmm3
        por     xmm1, xmm0
        movdqa  xmm2, xmm1
        pand    xmm1, xmm4
        movdqa  xmm0, xmm1
        psrlq   xmm2, 1
        pxor    xmm2, XMMWORD PTR [rax-1832]
        psllq   xmm0, 3
        paddq   xmm0, xmm1
        psllq   xmm0, 9
        paddq   xmm0, xmm1
        psllq   xmm0, 5
        paddq   xmm0, xmm1
        psllq   xmm0, 2
        psubq   xmm0, xmm1
        psllq   xmm0, 3
        psubq   xmm0, xmm1
        movdqa  xmm8, xmm0
        psllq   xmm8, 4
        paddq   xmm0, xmm8
        psllq   xmm0, 5
        psubq   xmm0, xmm1
        pxor    xmm2, xmm0
        movups  XMMWORD PTR [rax-16], xmm2
        cmp     rax, rdx
        jne     .L20
        mov     rdx, QWORD PTR [rsp+32]
        mov     rax, QWORD PTR [rsp+5016]
        mov     r12d, 1
        mov     rsi, rdx
        and     rax, -2147483648
        and     esi, 2147483647
        or      rax, rsi
        mov     rsi, rax
        and     eax, 1
        neg     rax
        shr     rsi
        xor     rsi, QWORD PTR [rsp+3200]
        and     eax, 2567483615
        xor     rax, rsi
        mov     QWORD PTR [rsp+5016], rax
        jmp     .L18
.L10:
        mov     rdx, rax
        and     eax, 1
        pxor    xmm0, xmm0
        shr     rdx
        or      rdx, rax
        cvtsi2sd        xmm0, rdx
        addsd   xmm0, xmm0
        jmp     .L11
.L21:
        mov     rdx, rax
        and     eax, 1
        pxor    xmm0, xmm0
        shr     rdx
        or      rdx, rax
        cvtsi2sd        xmm0, rdx
        addsd   xmm0, xmm0
        jmp     .L22
.L26:
        mov     rdi, r14
        call    std::ctype<char>::_M_widen_init() const
        mov     rax, QWORD PTR [r14]
        mov     esi, 10
        mov     rax, QWORD PTR [rax+48]
        cmp     rax, OFFSET FLAT:_ZNKSt5ctypeIcE8do_widenEc
        je      .L27
        mov     rdi, r14
        call    rax
        movsx   esi, al
        jmp     .L27
.L32:
        movsd   xmm1, QWORD PTR .LC3[rip]
        movapd  xmm0, xmm6
        jmp     .L24
.L30:
        movsd   xmm6, QWORD PTR .LC2[rip]
        jmp     .L16
.L28:
        add     rsp, 5056
        xor     eax, eax
        pop     rbx
        pop     rbp
        pop     r12
        pop     r13
        pop     r14
        ret
.L42:
        movapd  xmm0, xmm6
        call    sqrt
        movdqa  xmm3, XMMWORD PTR .LC4[rip]
        movapd  xmm6, xmm0
        jmp     .L16
main.cold:
.LC1:
        .long   0
        .long   1072693248
.LC2:
        .long   -1
        .long   1072693247
.LC3:
        .long   -1933240574
        .long   -1126930039
.LC4:
        .quad   2147483647
        .quad   2147483647
.LC7:
        .long   1333788672
.LC8:
        .long   1413754136
        .long   1075388923


--------


Possible method:

.L44:
        divsd   xmm6, QWORD PTR [rsp+8]
        comisd  xmm6, QWORD PTR .LC1[rip]
        jnb     .L30
        pxor    xmm0, xmm0
        addsd   xmm6, xmm0
        ucomisd xmm0, xmm6
        ja      .L42
        sqrtsd  xmm6, xmm6
.L16:
        mov     rax, QWORD PTR .LC1[rip]
        mov     ecx, 2
        pxor    xmm7, xmm7
        mov     QWORD PTR [rsp+8], rax
        cmp     r12, 624
        je      .L17
.L46:
        mov     rdx, QWORD PTR [rsp+32+r12*8]
        add     r12, 1
.L18:
        mov     rax, rdx
        shr     rax, 11
        mov     eax, eax
        xor     rax, rdx
        mov     rdx, rax
        sal     rdx, 7
        and     edx, 2636928640
        xor     rax, rdx
        mov     rdx, rax
        sal     rdx, 15
        and     edx, 4022730752
        xor     rax, rdx
        mov     rdx, rax
        shr     rdx, 18
        xor     rax, rdx
        js      .L21
        pxor    xmm0, xmm0
        cvtsi2sd        xmm0, rax
.L22:
        mulsd   xmm0, QWORD PTR [rsp+8]
        fld     DWORD PTR .LC7[rip]
        fmul    QWORD PTR [rsp+8]
        addsd   xmm7, xmm0
        fstp    QWORD PTR [rsp+8]
        cmp     ecx, 1
        jne     .L31
        divsd   xmm7, QWORD PTR [rsp+8]
        comisd  xmm7, QWORD PTR .LC1[rip]
        jnb     .L32
        pxor    xmm0, xmm0
        lea     rdi, [rsp+24]
        lea     rsi, [rsp+16]
        movsd   QWORD PTR [rsp+8], xmm6
        addsd   xmm7, xmm0
        mulsd   xmm7, QWORD PTR .LC8[rip]
        addsd   xmm0, xmm7
        call    sincos
        movsd   xmm6, QWORD PTR [rsp+8]
        movsd   xmm0, QWORD PTR [rsp+16]
        movsd   xmm1, QWORD PTR [rsp+24]
        mulsd   xmm0, xmm6

--------

    55(?) instructions + two calls (to sqrt and sincos)
