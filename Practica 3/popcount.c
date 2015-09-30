#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <sys/time.h>	// para gettimeofday(), struct timeval

#define SIZE (1<<16) // tamaño suficiente para tiempo apreciable
// unsigned lista[SIZE]={0x80000000, 0x00100000, 0x00000800, 0x00000001};
// unsigned lista[SIZE]={0x7fffffff, 0xffefffff, 0xfffff7ff, 0xfffffffe, 0x01000024, 0x00356700, 0x8900ac00, 0x00bd00ef};
// unsigned lista[SIZE]={0x0	   , 0x10204080, 0x3590ac06, 0x70b0d0e0, 0xffffffff 0x12345678, 0x9abcdef0, 0xcafebeef};
unsigned lista[SIZE];
#define WSIZE 8*sizeof(int)

unsigned popcount1(unsigned *array, int len){
  int i, j;
  unsigned result = 0;
  unsigned x;
  for(j=0; j<len; j++)
  {
    x = array[j];
    for(i=0; i<WSIZE; i++)
    {
      unsigned mask = 1 << i;
      result += (x & mask) != 0;
    }
  }

  return result;
}

unsigned popcount2(unsigned *array, int len){
  int j;
  unsigned result = 0;
  unsigned x;

  for(j=0; j<len; j++)
  {
    x = array[j];
    do
    {
      result += x&0x1;
      x >>= 1;
    } while(x);
  }

  return result;
}

unsigned popcount3(unsigned *array, int len){
  int i;
  unsigned result=0;
  unsigned x;

  for(i=0; i<len; i++)
  {
    x = array[i];
    asm("\n"
"ini3:								\n\t"	// seguir mientras que x!=0
"		shr %[x]					\n\t"	// LSB en CF
"		adc $0, %[result]				\n\t"	// acarreo
"		test %[x],%[x]				\n\t"	// miramos si x=0
"		jnz ini3					\n\t"	// saltamos de nuevo si quedan bits a uno
      : [result]"+r" (result)					// E/S: añadir lo acumulado por el momento
      : [x] "r" (x)						// entrada: valor elemento
      );
    }

  return result;
}

unsigned popcount4(unsigned *array, int len){
  int i, j;
  unsigned val = 0;
  unsigned result = 0;
  unsigned x;

  for(j=0; j<len; j++)
  {
    x = array[j];
    for(i=0; i<8; i++)
    {
      val += x & 0x01010101;
      x >> 1;
    }

    val += (val >> 16);
    val += (val >> 8);

    result += (val & 0xFF);
    val = 0;
  }

  return result;
}

unsigned popcount5 (unsigned *array, int len)
{
  int i;
  unsigned val, result=0;
  int SSE_mask[] = {0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f};
  int SSE_LUTb[] = {0x02010100, 0x03020201, 0x03020201, 0x04030302};
  //	3 2 1 0 	7 6 5 4 	1110 9 8	15141312

  if (len & 0x3)
    printf("leyendo 128b pero len no multiplo de 4?\n");

    for (i=0;i<len;i+=4)
      {
        asm("movdqu 	%[x], %%xmm0 	\n\t"
        "movdqa		%%xmm0, %%xmm1	\n\t"	// dos copias de x
        "movdqu 	%[m], %%xmm6	\n\t"	// mascara
        "psrlw		$4, %%xmm1 		\n\t"
        "pand 		%%xmm6, %%xmm0	\n\t"	//; xmm0 - nibbles inferiores
        "pand		%%xmm6, %%xmm1	\n\t"	//; xmm1 - nibbles superiores

        "movdqu		%[l], %%xmm2	\n\t"	//; ...como pshufb sobrescribe LUT
        "movdqa		%%xmm2, %%xmm3	\n\t"	//; ...queremos 2 copias
        "pshufb		%%xmm0,	%%xmm2	\n\t"	//; xmm2 = vector popcount inferiores
        "pshufb		%%xmm1,	%%xmm3	\n\t"	//; xmm3 = vector popcount superiores

        "paddb		%%xmm2, %%xmm3	\n\t"	//; xmm2 += xmm3 - vector popcount bytes
        "pxor		%%xmm0, %%xmm0	\n\t"	//; xmm0 = 0,0,0,0
        "psadbw		%%xmm0, %%xmm3	\n\t"	//; xmm3 = [pcnt bytes0..7|pcnt bytes8..15]
        "movhlps	%%xmm3,	%%xmm0	\n\t"	//; xmm0 = [	0 		  |pcnt bytes0..7]
        "paddd		%%xmm3,	%%xmm0	\n\t"	//; xmm0 = [	no usado  |pcnt bytes0..15]
        "movd 		%%xmm0, %[val]	\n\t"
        : [val]"=r" (val)
        : [x]  "m"	(array[i]),
        [m]  "m"	(SSE_mask[0]),
        [l]  "m"	(SSE_LUTb[0])
        );
        result += val;
      }

      return result;
    }

void crono(unsigned (*func)(), char* msg){
  unsigned result;
  struct timeval tv1,tv2; // gettimeofday() secs-usecs
  long           tv_usecs;  // y sus cuentas

  gettimeofday(&tv1, NULL);
  result = func(lista, SIZE);
  gettimeofday(&tv2, NULL);

  tv_usecs = (tv2.tv_sec -tv1.tv_sec)*1E6+
  (tv2.tv_usec-tv1.tv_usec);
  printf("resultultado = %d\t", result);
  printf("%s:%9ld us\n", msg, tv_usecs);
}

int main(){
  int i;
  for(i=0; i<SIZE;i++)  // inicializar array
    lista[i]=i; // se queda en cache


  crono(popcount1,"popcount1 (En C con for    )");
  crono(popcount2,"popcount2 (En C con while    )");
  crono(popcount3,"popcount3 (Con bloque asm    )");
  crono(popcount4,"popcount4 (Version resuelta del libro    )");
  crono(popcount5,"popcount5 (Con SSSE3    )");
  printf("N*(N+1)/2=%d\n", (SIZE-1)*(SIZE/2));  /*OF*/

  exit(0);
}
