	.file	"tlb.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"#pages\t proc\t sum"
.LC2:
	.string	"%d\t %.6f\t %ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB22:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	movl	$0, %r13d
	movl	$1, %ebx
	jmp	.L6
.L4:
	addl	$1, %edx
	cmpl	%ebx, %edx
	jne	.L4
	leaq	1(%r13,%rbp), %rbp
.L5:
	addl	$1, %eax
	cmpl	%eax, %r12d
	je	.L3
.L2:
	movl	$0, %edx
	testl	%ebx, %ebx
	jg	.L4
	jmp	.L5
.L3:
	call	clock@PLT
	subq	%r14, %rax
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	divsd	.LC1(%rip), %xmm0
	movq	%rbp, %rdx
	movl	%ebx, %esi
	leaq	.LC2(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	addl	$1, %ebx
	addq	$1, %r13
	cmpl	$17, %ebx
	je	.L14
.L6:
	movl	$1048576, %eax
	cltd
	idivl	%ebx
	movl	%eax, %r12d
	call	clock@PLT
	movq	%rax, %r14
	movl	$0, %eax
	movl	$0, %ebp
	testl	%r12d, %r12d
	jg	.L2
	jmp	.L3
.L14:
	movl	$0, %eax
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE22:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	0
	.long	1093567616
	.ident	"GCC: (Debian 10.2.0-15) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
