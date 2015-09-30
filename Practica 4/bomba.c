#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <string.h>	// para strncmp()/strlen()
#include <sys/time.h>	// para gettimeofday(), struct timeval

//char pass[]="granada\n"; //ASCII 103 114 97 110 97 100 97
char tloz1[] = {104, 115, 98, 111, 98, 101, 98}; //El codigo ASCII+1
int  tloz2  = 666; //La autentica contraseña seria (201*3)+63


void tres(){
	printf("\n");
	printf("***************\n");
	printf("*** BOOM!!! ***\n");
	printf("***************\n");
	printf("\n");
	exit(-1);
}

void dos()
{
	tres();
}

void uno()
{
	printf("\n");
	printf("*************************\n");
	printf("*** bomba desactivada ***\n");
	printf("*************************\n");
	printf("\n");
	exit(0);
}

//Comprobación de la clave
void asd(int p)
{
	int aux=tloz2;
	aux = tloz2 * 2;
	aux = aux + 64;

	if(p != aux)
		dos();
}

//Combrobacion de la contraseña
void qwe(char *p)
{
	int c=0;
	char aux[strlen(tloz1)];

	while(c<strlen(tloz1))
	{
		aux[c]=p[c]+1;
		c++;
	}

	if(strncmp(aux,tloz1,strlen(tloz1)))
	{
		dos();
	}
}

int main(){
#define SIZE 100
	char c1[SIZE];
	int  c2;
#define TLIM 60
	struct timeval tv1,tv2;	// gettimeofday() secs-usecs

	gettimeofday(&tv1,NULL);

	printf("Introduce la contraseña: ");
	fgets(c1,SIZE,stdin);
	qwe(c1);

	gettimeofday(&tv2,NULL);
	if (tv2.tv_sec - tv1.tv_sec > TLIM)
	    dos();

	printf("Introduce el código: ");
	scanf("%i",&c2);
	asd(c2);

	gettimeofday(&tv1,NULL);
	if (tv1.tv_sec - tv2.tv_sec > TLIM)
		dos();

	uno();
}
