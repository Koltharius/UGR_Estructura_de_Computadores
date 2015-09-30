#################################################
# printf.s
# gcc -m32 printf.s -o printf
#################################################

.section .data
i:      .int 12345         # variable entera
f:      .string "i = %d\n" # formato

#################################################

.section .text
        .extern printf     # printf en otro sitio
        .globl main        # función principal de C

main:   pushl %ebp         # preserva ebp
        movl  %esp, %ebp   # copia pila

        pushl (i)          # apila i
        pushl $f           # apila f
        call  printf       # llamada a función
        addl  $8, %esp     # restaura pila

        movl  $1, %eax     # exit
        movl  $0, %ebx     # 0
        int   $0x80        # llamada al sistema

#################################################

