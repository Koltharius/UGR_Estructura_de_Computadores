.section
  .macro linea
  # .int 1,1,1,1
  # .int 2,2,2,2
  # .int 1,2,3,4
  # .int -1,-1,-1,-1
  # .int 0xffffffff,0xffffffff,0xffffffff,0xffffffff
  # .int 0x08000000,0x08000000,0x08000000,0x08000000
  # .int 0x10000000,0x20000000,0x40000000,0x80000000
  .endm
lista: .irpc i,12345678
      linea
  .endr

.section .text
_start: -global _start

  mov $lista, %ebx
  mov longlista, %ecx
  call suma
  mov %eax, resultado

  push resultado
  push resultado
  push $formato
  call printf
  add $12, %esp

  mov $1, %eax
  mov $0, %ebx
  int $0x80

suma:
  push %edx
  mov $0, %eax
  mov $0, %edx

bucle:
  add (%ebx, %edx, 4), %eax
  inc %edx
  cmp %edx, %ecx
  jne bucle

  pop %edx
  ret
