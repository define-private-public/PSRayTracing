.LC8:
        .string " "
main:
        push    rbx
        xor     ecx, ecx
        mov     eax, 1
        sub     rsp, 5040
        mov     QWORD PTR [rsp+16], 0
        lea     rsi, [rsp+16]
.L2:
        mov     rdx, rcx
        shr     rdx, 30
        xor     rdx, rcx
        imul    rdx, rdx, 1812433253
        lea     ecx, [rax+rdx]
        mov     QWORD PTR [rsi+rax*8], rcx
        add     rax, 1
        cmp     rax, 624
        jne     .L2
        mov     ebx, 2147483647
        fld     DWORD PTR .LC7[rip]
        movsd   xmm8, QWORD PTR .LC1[rip]
        lea     rdx, [rsp+1824]
        movq    xmm5, rbx
        mov     rbx, -2147483648
        movq    xmm4, rbx
        mov     ebx, 1
        punpcklqdq      xmm5, xmm5
        movq    xmm3, rbx
        punpcklqdq      xmm4, xmm4
        punpcklqdq      xmm3, xmm3
        jmp     .L35
.L38:
        divsd   xmm6, QWORD PTR [rsp+8]
        comisd  xmm6, xmm8
        jnb     .L26
        pxor    xmm0, xmm0
        addsd   xmm6, xmm0
        addsd   xmm6, xmm6
        subsd   xmm6, xmm8
        movapd  xmm9, xmm6
        mulsd   xmm9, xmm6
.L10:
        mov     edi, 2
        pxor    xmm7, xmm7
        movsd   QWORD PTR [rsp+8], xmm8
        cmp     rax, 624
        je      .L11
.L40:
        mov     r8, QWORD PTR [rsp+16+rax*8]
        add     rax, 1
.L12:
        mov     rcx, r8
        shr     rcx, 11
        mov     ecx, ecx
        xor     rcx, r8
        mov     r8, rcx
        sal     r8, 7
        and     r8d, 2636928640
        xor     rcx, r8
        mov     r8, rcx
        sal     r8, 15
        and     r8d, 4022730752
        xor     rcx, r8
        mov     r8, rcx
        shr     r8, 18
        xor     rcx, r8
        js      .L15
        pxor    xmm0, xmm0
        cvtsi2sd        xmm0, rcx
.L16:
        mulsd   xmm0, QWORD PTR [rsp+8]
        fld     st(0)
        fmul    QWORD PTR [rsp+8]
        addsd   xmm7, xmm0
        fstp    QWORD PTR [rsp+8]
        cmp     edi, 1
        jne     .L27
        divsd   xmm7, QWORD PTR [rsp+8]
        comisd  xmm7, xmm8
        jnb     .L18
        pxor    xmm0, xmm0
        addsd   xmm7, xmm0
        addsd   xmm7, xmm7
        subsd   xmm7, xmm8
        movapd  xmm0, xmm7
        mulsd   xmm0, xmm7
        addsd   xmm0, xmm9
        comisd  xmm8, xmm0
        ja      .L41
.L35:
        mov     edi, 2
        pxor    xmm6, xmm6
        movsd   QWORD PTR [rsp+8], xmm8
        cmp     rax, 624
        je      .L3
.L39:
        mov     r8, QWORD PTR [rsp+16+rax*8]
        add     rax, 1
.L4:
        mov     rcx, r8
        shr     rcx, 11
        mov     ecx, ecx
        xor     rcx, r8
        mov     r8, rcx
        sal     r8, 7
        and     r8d, 2636928640
        xor     rcx, r8
        mov     r8, rcx
        sal     r8, 15
        and     r8d, 4022730752
        xor     rcx, r8
        mov     r8, rcx
        shr     r8, 18
        xor     rcx, r8
        js      .L7
        pxor    xmm0, xmm0
        cvtsi2sd        xmm0, rcx
.L8:
        mulsd   xmm0, QWORD PTR [rsp+8]
        fld     st(0)
        fmul    QWORD PTR [rsp+8]
        addsd   xmm6, xmm0
        fstp    QWORD PTR [rsp+8]
        cmp     edi, 1
        je      .L38
        mov     edi, 1
        cmp     rax, 624
        jne     .L39
.L3:
        mov     rax, rsi
.L5:
        movdqu  xmm1, XMMWORD PTR [rax+8]
        movdqa  xmm0, XMMWORD PTR [rax]
        add     rax, 16
        movdqu  xmm7, XMMWORD PTR [rax+3160]
        pand    xmm0, xmm4
        pand    xmm1, xmm5
        por     xmm1, xmm0
        movdqa  xmm2, xmm1
        pand    xmm1, xmm3
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
        jne     .L5
        mov     rcx, QWORD PTR [rsp+1824]
        mov     rax, QWORD PTR [rsp+1832]
        and     rcx, -2147483648
        and     eax, 2147483647
        or      rax, rcx
        mov     rcx, rax
        and     eax, 1
        neg     rax
        shr     rcx
        xor     rcx, QWORD PTR [rsp+5000]
        and     eax, 2567483615
        xor     rax, rcx
        lea     rcx, [rsi+4984]
        mov     QWORD PTR [rsp+1824], rax
        lea     rax, [rsp+1832]
.L6:
        movdqa  xmm1, XMMWORD PTR [rax+8]
        movdqu  xmm0, XMMWORD PTR [rax]
        add     rax, 16
        pand    xmm0, xmm4
        pand    xmm1, xmm5
        por     xmm1, xmm0
        movdqa  xmm2, xmm1
        pand    xmm1, xmm3
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
        movdqa  xmm7, xmm0
        psllq   xmm7, 4
        paddq   xmm0, xmm7
        psllq   xmm0, 5
        psubq   xmm0, xmm1
        pxor    xmm2, xmm0
        movups  XMMWORD PTR [rax-16], xmm2
        cmp     rax, rcx
        jne     .L6
        mov     r8, QWORD PTR [rsp+16]
        mov     rax, QWORD PTR [rsp+5000]
        mov     rcx, r8
        and     rax, -2147483648
        and     ecx, 2147483647
        or      rax, rcx
        mov     rcx, rax
        and     eax, 1
        neg     rax
        shr     rcx
        xor     rcx, QWORD PTR [rsp+3184]
        and     eax, 2567483615
        xor     rax, rcx
        mov     QWORD PTR [rsp+5000], rax
        mov     eax, 1
        jmp     .L4
.L27:
        mov     edi, 1
        cmp     rax, 624
        jne     .L40
.L11:
        mov     rax, rsi
.L13:
        movdqu  xmm1, XMMWORD PTR [rax+8]
        movdqa  xmm0, XMMWORD PTR [rax]
        add     rax, 16
        pand    xmm0, xmm4
        pand    xmm1, xmm5
        por     xmm1, xmm0
        movdqu  xmm0, XMMWORD PTR [rax+3160]
        movdqa  xmm2, xmm1
        pand    xmm1, xmm3
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
        movdqa  xmm10, xmm0
        psllq   xmm10, 4
        paddq   xmm0, xmm10
        psllq   xmm0, 5
        psubq   xmm0, xmm1
        pxor    xmm2, xmm0
        movaps  XMMWORD PTR [rax-16], xmm2
        cmp     rdx, rax
        jne     .L13
        mov     rcx, QWORD PTR [rsp+1824]
        mov     rax, QWORD PTR [rsp+1832]
        and     rcx, -2147483648
        and     eax, 2147483647
        or      rax, rcx
        mov     rcx, rax
        and     eax, 1
        neg     rax
        shr     rcx
        xor     rcx, QWORD PTR [rsp+5000]
        and     eax, 2567483615
        xor     rax, rcx
        lea     rcx, [rsi+4984]
        mov     QWORD PTR [rsp+1824], rax
        lea     rax, [rsp+1832]
.L14:
        movdqa  xmm1, XMMWORD PTR [rax+8]
        movdqu  xmm0, XMMWORD PTR [rax]
        add     rax, 16
        pand    xmm0, xmm4
        pand    xmm1, xmm5
        por     xmm1, xmm0
        movdqa  xmm2, xmm1
        pand    xmm1, xmm3
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
        movdqa  xmm10, xmm0
        psllq   xmm10, 4
        paddq   xmm0, xmm10
        psllq   xmm0, 5
        psubq   xmm0, xmm1
        pxor    xmm2, xmm0
        movups  XMMWORD PTR [rax-16], xmm2
        cmp     rax, rcx
        jne     .L14
        mov     r8, QWORD PTR [rsp+16]
        mov     rax, QWORD PTR [rsp+5000]
        mov     rcx, r8
        and     rax, -2147483648
        and     ecx, 2147483647
        or      rax, rcx
        mov     rcx, rax
        and     eax, 1
        neg     rax
        shr     rcx
        xor     rcx, QWORD PTR [rsp+3184]
        and     eax, 2567483615
        xor     rax, rcx
        mov     QWORD PTR [rsp+5000], rax
        mov     eax, 1
        jmp     .L12
.L15:
        mov     r8, rcx
        and     ecx, 1
        pxor    xmm0, xmm0
        shr     r8
        or      r8, rcx
        cvtsi2sd        xmm0, r8
        addsd   xmm0, xmm0
        jmp     .L16
.L7:
        mov     r8, rcx
        and     ecx, 1
        pxor    xmm0, xmm0
        shr     r8
        or      r8, rcx
        cvtsi2sd        xmm0, r8
        addsd   xmm0, xmm0
        jmp     .L8
.L18:
        addsd   xmm9, QWORD PTR .LC2[rip]
        comisd  xmm8, xmm9
        jbe     .L35
        fstp    st(0)
        movsd   xmm7, QWORD PTR .LC3[rip]
        jmp     .L19
.L41:
        fstp    st(0)
.L19:
        movapd  xmm0, xmm7
        mov     edi, OFFSET FLAT:std::cout
        movsd   QWORD PTR [rsp+8], xmm6
        call    std::basic_ostream<char, std::char_traits<char> >& std::basic_ostream<char, std::char_traits<char> >::_M_insert<double>(double)
        mov     edx, 1
        mov     esi, OFFSET FLAT:.LC8
        mov     rdi, rax
        mov     rbx, rax
        call    std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)
        movsd   xmm6, QWORD PTR [rsp+8]
        mov     rdi, rbx
        movapd  xmm0, xmm6
        call    std::basic_ostream<char, std::char_traits<char> >& std::basic_ostream<char, std::char_traits<char> >::_M_insert<double>(double)
        mov     rbx, rax
        mov     rax, QWORD PTR [rax]
        mov     rax, QWORD PTR [rax-24]
        mov     rdi, QWORD PTR [rbx+240+rax]
        test    rdi, rdi
        je      .L34
        cmp     BYTE PTR [rdi+56], 0
        je      .L23
        movzx   eax, BYTE PTR [rdi+67]
.L24:
        mov     rdi, rbx
        movsx   esi, al
        call    std::basic_ostream<char, std::char_traits<char> >::put(char)
        mov     rdi, rax
        call    std::basic_ostream<char, std::char_traits<char> >::flush()
        add     rsp, 5040
        xor     eax, eax
        pop     rbx
        ret
.L26:
        movsd   xmm9, QWORD PTR .LC2[rip]
        movsd   xmm6, QWORD PTR .LC3[rip]
        jmp     .L10
.L23:
        mov     QWORD PTR [rsp+8], rdi
        call    std::ctype<char>::_M_widen_init() const
        mov     rdi, QWORD PTR [rsp+8]
        mov     esi, 10
        mov     rax, QWORD PTR [rdi]
        call    [QWORD PTR [rax+48]]
        jmp     .L24
main.cold:
.LC1:
        .long   0
        .long   1072693248
.LC2:
        .long   -4
        .long   1072693247
.LC3:
        .long   -2
        .long   1072693247
.LC7:
        .long   1333788672




--------



    Possible loop?
.L16:
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
        movdqa  xmm10, xmm0
        psllq   xmm10, 4
        paddq   xmm0, xmm10
        psllq   xmm0, 5
        psubq   xmm0, xmm1
        pxor    xmm2, xmm0
        movups  XMMWORD PTR [rax-16], xmm2
        cmp     rax, rsi
        jne     .L16
        mov     rsi, QWORD PTR [rsp+16]
        mov     rax, QWORD PTR [rsp+5000]
        mov     r12d, 1
        mov     rdi, rsi
        and     rax, -2147483648
        and     edi, 2147483647
        or      rax, rdi
        mov     rdi, rax
        and     eax, 1
        neg     rax
        shr     rdi
        xor     rdi, QWORD PTR [rsp+3184]
        and     eax, 2567483615
        xor     rax, rdi
        mov     QWORD PTR [rsp+5000], rax
        jmp     .L14

--------

    45-50 instructions?  No `call`s
