.section .data
    .macro linea
     #	.int 1,1,1,1
     #	.int 2,2,2,2
     #	.int 1,2,3,4
     #	.int -1,-1,-1,-1
     #	.int  0xfffffffff,0xfffffffff,0xfffffffff,0xfffffffff
     #	.int 0x08000000,0x08000000,0x08000000,0x08000000
     #	.int  0x10000000,0x20000000,0x40000000,0x80000000
    .endm
  lista: .irpc i,12345678
          linea
    .endr
    
longlista:
	.int	(.-lista)/4
cociente:
	.int   	0
resto:
	.int	0

.section .text
_start:.global _start

	mov     $lista, %ebx
	mov  longlista, %ecx
	call suma
	mov  %eax,cociente
	mov  %edx,resto

	mov $1, %eax
	mov $0, %ebx
	int $0x80

suma:
	push     %edx
	mov  $0, %esi
	mov  $0, %ebp
	mov  $0, %edi
bucle:
	mov  (%ebx, %esi, 4), %eax
	cltd
	add %eax, %ebp
	adc %edx, %edi
	inc      %esi
	cmp %esi,%ecx
	jne bucle

	mov %edi, %edx
	mov %ebp, %eax

	idivl %ecx

	pop %edx
	ret
