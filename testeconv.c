#include <stdio.h>
#include <stdlib.h>
#include "converteutf832.h"

FILE *AbriArq (char *path, char *mode)
{
	FILE *arq = fopen (path, mode);
	if (arq == NULL)
	{
		fprintf(stderr,"Erro ao abrir o arquivo %s\n", path);
                exit (1);
	}
	else
		return arq;
}

int  FechaArquivo(FILE *arquivo, char *path)
{
    if (fclose(arquivo) != 0)
    {
      fprintf(stderr,"Erro ao fechar o arquivo %s", path);
      return -1;
    }
    return 0;
}

void Exibe_UTF32(FILE *arquivo_entrada)
{
	unsigned int c;
        unsigned int bom;
        
        fread(&bom, sizeof(int), 1, arquivo_entrada);
        
        printf("O bom (Bit of Mark): %X\n", bom);

	while(fread(&c, sizeof(int), 1, arquivo_entrada) == 1)
	{
    	printf("Caractere lido: U+%X\n", c);
	}
	return;
}

void Exibe_UTF8(FILE *arquivo_entrada)
{
	unsigned char c;

	while(fread(&c, sizeof(char), 1, arquivo_entrada) == 1) 
	{
		printf("Caractere lido: U+%X\n", c);
	}
	return;
}


int main (void)
{
        FILE *entrada_8, *saida_32, *entrada_32, *saida_8;
	char *path;
	int fim;

	path = "utf8_peq.txt";
	entrada_8 = AbriArq(path, "rb");

	path = "saida_UTF-32.txt";
	saida_32 = AbriArq(path, "wb");

	path = "utf32_peq.txt";
	entrada_32 = AbriArq(path, "rb");

	path = "saida_UTF-8.txt";
	saida_8 = AbriArq(path, "wb");


	int i = convUtf8p32(entrada_8,saida_32);
        if (i == -1)
      	  return i;
	int j = convUtf32p8(entrada_32,saida_8);
        if (j == -1)
      	  return j;
      	
      	path = "utf8_peq.txt";
        fim = FechaArquivo(entrada_8, path);
      	if (fim == -1)
      	  return fim;
      	  
      	path = "saida_UTF-32.txt";
      	fim = FechaArquivo(saida_32, path);
      	if (fim == -1)
      	  return fim;
      	
      	path = "utf32_peq.txt";
      	fim = FechaArquivo(entrada_32, path);
      	if (fim == -1)
      	  return fim;
      	  
      	path = "saida_UTF-8.txt";
      	fim = FechaArquivo(saida_8, path);
      	if (fim == -1)
      	  return fim;  
        
	return 0;
}

