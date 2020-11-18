	.file	"tlb.c"
	.text
	.section	.rodata
.LC0:
	.string	"#pages\t proc\t sum"
.LC2:
	.string	"%d\t %.6f\t %ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movl	%edi, -68(%rbp)
	movq	%rsi, -80(%rbp)
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	movl	$1, -4(%rbp)
	jmp	.L2
.L7:
	movl	$1048576, %eax
	cltd
	idivl	-4(%rbp)
	movl	%eax, -28(%rbp)
	call	clock@PLT
	movq	%rax, -40(%rbp)
	movq	$0, -16(%rbp)
	movl	$0, -20(%rbp)
	jmp	.L3
.L6:
	movl	$0, -24(%rbp)
	jmp	.L4
.L5:
	addq	$1, -16(%rbp)
	addl	$1, -24(%rbp)
.L4:
	movl	-24(%rbp), %eax
	cmpl	-4(%rbp), %eax
	jl	.L5
	addl	$1, -20(%rbp)
.L3:
	movl	-20(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L6
	call	clock@PLT
	movq	%rax, -48(%rbp)
	movq	-48(%rbp), %rax
	subq	-40(%rbp), %rax
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC1(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movsd	%xmm0, -56(%rbp)
	movq	-16(%rbp), %rdx
	movq	-56(%rbp), %rcx
	movl	-4(%rbp), %eax
	movq	%rcx, %xmm0
	movl	%eax, %esi
	leaq	.LC2(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	addl	$1, -4(%rbp)
.L2:
	cmpl	$16, -4(%rbp)
	jle	.L7
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC1:
	.long	0
	.long	1093567616
	.ident	"GCC: (Debian 10.2.0-15) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
