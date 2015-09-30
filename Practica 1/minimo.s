.text
	.globl _start

_start:
	movl    $1, %eax   # exit
	xorl    %ebx, %ebx # 0
	int     $0x80      # llamada al sistema

