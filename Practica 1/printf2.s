#################################################
# printf2.s
# gcc -m32 printf2.s -o printf2
#################################################

.section .data
i: .int 12345           # variable entera
f: .string "i = %d\n"   # formato

#################################################

.section .text
        .extern printf   # printf en otro sitio
        .globl main      # función principal de C

main:   pushl %ebp       # preserva ebp
        movl  %esp, %ebp # copia pila

        pushl (i)        # apila i
        pushl $f         # apila f
        call  printf     # llamada a función
        addl  $8, %esp   # restaura pila

        xor   %eax, %eax # valor de retorno 0
        call exit        # llamada a función

#################################################

