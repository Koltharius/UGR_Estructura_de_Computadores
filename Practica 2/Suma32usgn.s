.section .data
lista:
	#.int	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	.int	0xf0000000, 0xe0000000, 0xe0000000, 0xd0000000,0xf0000000, 0xe0000000, 0xe0000000, 0xd0000000,0xf0000000, 0xe0000000, 0xe0000000, 0xd0000000,0xf0000000, 0xe0000000, 0xe0000000, 0xd0000000,0xf0000000, 0xe0000000, 0xe0000000, 0xd0000000,0xf0000000, 0xe0000000, 0xe0000000, 0xd0000000,0xf0000000, 0xe0000000, 0xe0000000, 0xd0000000,0xf0000000, 0xe0000000, 0xe0000000, 0xd0000000,0xf0000000, 0xe0000000, 0xe0000000, 0xd0000000,
	#.int 0x0, 0x7fffffff
longlista:
	.int	(.-lista)/4	# .= contador posiciones. Aritmética de etiquetas.
resultado:
	.quad   0		# 4B a FF para notar cuándo se modifica cada byte

# SECCIÓN DE CÓDIGO (.text, instrucciones máquina)
.section .text
_start:.global _start		# PROGRAMA PRINCIPAL-se puede abreviar de esta forma

	mov     $lista, %ebx	# dirección del array lista
	mov  longlista, %ecx	# número de elementos a sumar
	call suma		# llamar suma(&lista, longlista);
	mov  %eax,resultado	# salvar resultado
	mov  %edx,resultado+4

	# void _exit(int status);
	mov $1, %eax		#   exit: servicio 1 kernel Linux
	mov $0, %ebx	# status: código a retornar (0=OK)
	int $0x80		# llamar _exit(0);

# SUBRUTINA:	suma(int* lista, int longlista);
# entrada:	1) %ebx = dirección inicio array
#		2) %ecx = número de elementos a sumar
# salida: 	   %eax = resultado de la suma

suma:
	push     %edx		# preservar %edx (se usa aquí como índice)
	#mov  $0xffffffff, %eax  # poner a 0 acumulador
	mov  $0, %esi		# poner a 0 índice
	mov  $0, %ebp
	mov  $0, %edi
bucle:
	mov  (%ebx, %esi, 4), %eax
	cltd
	add %eax, %ebp		#Acumulador EDI:EBP
	adc %edx, %edi		# usado como acum
	inc      %esi			# incrementar índice
	cmp %esi,%ecx			# comparar con longitud
	jne bucle			# si no iguales, seguir acumulando

	mov %edi, %edx		#Colocamos el resultado donde lo espera _start
	mov %ebp, %eax
	pop %edx			# recuperar %edx antiguo
	ret
