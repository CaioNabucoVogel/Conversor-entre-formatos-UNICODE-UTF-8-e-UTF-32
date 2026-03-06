/* Caio Nabuco Vogel 2310878 3WA */
/* Vinícius Nahuel Naveiras Coutinho 2311693 3WA */


#include <stdio.h>
#include "converteutf832.h"
#include <stdint.h>

int convUtf8p32(FILE *arquivo_entrada, FILE *arquivo_saida)
{
	unsigned char c1, c2, c3, c4, t1, t2;
	unsigned int s, i;
        size_t QTDbytes;
        
	s = 0xFEFF;
	c2 = 0x00;
	c3 = 0x00;
	c4 = 0x00;
	t1 = 0x00;
	t2 = 0x00;

	QTDbytes = fwrite(&s,sizeof(int),1,arquivo_saida);
	
	if(QTDbytes == 0)
	{
	  fprintf(stderr,"Erro ao gravar");
	  return -1;
	}
      
	while(fread(&c1, sizeof(char), 1, arquivo_entrada) == 1)
	{
    	if(c1 >= 0xF0 && c1 <=0xF48FBFBF)
    	{
      	
      	QTDbytes = fread(&c2, sizeof(char), 1, arquivo_entrada);
      	if(QTDbytes == 0)
	{
	  fprintf(stderr,"Erro ao Ler");
	  return -1;
	}
      	QTDbytes = fread(&c3, sizeof(char), 1, arquivo_entrada);
  	if(QTDbytes == 0)
	{
	  fprintf(stderr,"Erro ao Ler");
	  return -1;
	}
      	QTDbytes = fread(&c4, sizeof(char), 1, arquivo_entrada);
        if(QTDbytes == 0)
	{
	  fprintf(stderr,"Erro ao Ler");
	  return -1;
	}
      	
      	s = 0x00;
      	c4 = c4 & 0x3f;
      	c3 = c3 & 0x3f;
      	c2 = c2 & 0x3f;
      	c1 = c1 << 4;
      	c1 = c1 >> 2;
      	
      	t1 = c2;
      	t2 = c3;
      	
        t1 = t1 >>4;
        c2 = c2 <<4;
        t2 = t2 >>2;
        c3 = c3 <<6;
      	
      	c1 = c1 | t1;
      	c2 = c2 | t2;
      	c3 = c3 | c4;
      	
      	s = c1 << 16;
      	s = s | (c2 << 8);
      	s = s | c3;
      	
      	QTDbytes = fwrite(&s,sizeof(int),1,arquivo_saida);
      	
      	if(QTDbytes == 0)
	{
	  fprintf(stderr,"Erro ao gravar");
	  return -1;
	}
      	
        i++;	
    	}
    	else if (c1 >= 0xE0)
    	{
      	
      	QTDbytes = fread(&c2, sizeof(char), 1, arquivo_entrada);
      	
        if(QTDbytes == 0)
	{
	  fprintf(stderr,"Erro ao Ler");
	  return -1;
	}
      	
      	QTDbytes = fread(&c3, sizeof(char), 1, arquivo_entrada);
      	
        if(QTDbytes == 0)
	{
	  fprintf(stderr,"Erro ao Ler");
	  return -1;
	}
      	
      	s = 0x00;
      	c3 = c3 & 0x3f;
      	c2 = c2 & 0x3f;
      	c1 = c1 << 4;
      	t1 = c2;
    	 
      	t1 = t1 << 6;
      	t1 = t1 | c3;
    	 
      	c2 = c2 >> 2;
      	c1 = c1 | c2;


      	s = c1 << 8;
      	s = s | t1;
      	
      	QTDbytes = fwrite(&s,sizeof(int),1,arquivo_saida);
      	
      	if(QTDbytes == 0)
	{
	  fprintf(stderr,"Erro ao gravar");
	  return -1;
	}
      	i++;
    	}
    	else if (c1 >= 0xC0)
    	{
      	
      	QTDbytes = fread(&c2, sizeof(char), 1, arquivo_entrada);
      	
        if(QTDbytes == 0)
	{
	  fprintf(stderr,"Erro ao Ler");
	  return -1;
	}
      	
      	s = 0x00;
      	c2 = c2 & 0x3f;
      	c1 = c1 << 6;
      	s = s | c2;
      	s = s | c1;
      	
      	QTDbytes = fwrite(&s,sizeof(int),1,arquivo_saida);
      	if(QTDbytes == 0)
	{
	  fprintf(stderr,"Erro ao gravar");
	  return -1;
	}
      	
      	i++;
    	}
    	else
    	{
      	
      	s = c1 & 0x7F;
      	
      	QTDbytes = fwrite(&s,sizeof(int),1,arquivo_saida);
    	if(QTDbytes == 0)
	{
	  fprintf(stderr,"Erro ao gravar");
	  return -1;
	}
    	
    	i++;
    	}
	}
	
	if (i == 0)
	{
	  fprintf(stderr,"Erro ao Ler, Nada foi lido");
	  return -1;
	}
	return 0;
}

int convUtf32p8(FILE *arquivo_entrada, FILE *arquivo_saida)
{
   
	unsigned int c, t1, t2, t3, m1, m2;
        unsigned int bom, i;
	size_t QTDbytes;        

        QTDbytes = fread(&bom, sizeof(int), 1, arquivo_entrada);
        
	if(QTDbytes == 0)
	{
	  fprintf(stderr,"Erro ao Ler o BOM");
	  return -1;
	}
      	
	if(bom != 0x0000FEFF && bom != 0xFFFE0000)
	{
	  fprintf(stderr,"BOM invalido");
	  return -1;		
	}	
        
        while(fread(&c, sizeof(int), 1, arquivo_entrada) == 1)
        {
    	  if(c >= 0x10000)
    	  {	
	    	if(bom != 0x0000FEFF)
		{	
			m1 = c;
			m2 = c;
			
			m1 = m1 & 0x00FF0000;
			m2 = m2 & 0x0000FF00;

			c = c >> 24;
			m1 = m1 >> 8;
			m2 = m2 << 8;

			c = c | m1 | m2;
		}
				
		t1 = c;
		t2 = c;
		t3 = c;

		c = c & 0x3F;
		t1 = t1 & 0xFC0;
		t2 = t2 & 0x3F000;
		t3 = t3 & 0x1C0000;

		t1 = t1 >> 6;
		t2 = t2 >> 12;
		t3 = t3 >> 18;
	
		c = c | 0x80;
		t1 = t1 | 0x80;
		t2 = t2 | 0x80;
		t3 = t3 | 0xF0;		
		
		QTDbytes = fwrite(&t3,sizeof(char),1,arquivo_saida);
		if(QTDbytes == 0)
		{
	  		fprintf(stderr,"Erro ao gravar");
	  		return -1;
		}	
		QTDbytes = fwrite(&t2,sizeof(char),1,arquivo_saida);
		if(QTDbytes == 0)
		{
	  		fprintf(stderr,"Erro ao gravar");
	  		return -1;
		}	
		QTDbytes = fwrite(&t1,sizeof(char),1,arquivo_saida);
		if(QTDbytes == 0)
		{
	  		fprintf(stderr,"Erro ao gravar");
	  		return -1;
		}	
		QTDbytes = fwrite(&c,sizeof(char),1,arquivo_saida);
		if(QTDbytes == 0)
		{
	  		fprintf(stderr,"Erro ao gravar");
	  		return -1;
		}	
		i++;
    	  }
          else if(c >= 0x0800)
          {

	  	if(bom != 0x0000FEFF)
		{	

			m1 = c;
						
			m1 = m1 & 0x00FF0000;

			c = c >> 24;
			m1 = m1 >> 8;

			c = c | m1;
		}

		t1 = c;
		t2 = c;

		c = c & 0x3F;
		t1 = t1 & 0xFC0;
		t2 = t2 & 0xF000;

		t1 = t1 >> 6;
		t2 = t2 >> 12;

		c = c | 0x80;
		t1 = t1 | 0x80;
		t2 = t2 | 0xE0;

		QTDbytes = fwrite(&t2,sizeof(char),1,arquivo_saida);
		if(QTDbytes == 0)
		{
	  		fprintf(stderr,"Erro ao gravar");
	  		return -1;
		}	
		QTDbytes = fwrite(&t1,sizeof(char),1,arquivo_saida);
		if(QTDbytes == 0)
		{
	  		fprintf(stderr,"Erro ao gravar");
	  		return -1;
		}	
		QTDbytes = fwrite(&c,sizeof(char),1,arquivo_saida);
		if(QTDbytes == 0)
		{
	  		fprintf(stderr,"Erro ao gravar");
	  		return -1;
		}	
		i++;
          }
          else if(c >= 0x0080)
          {
	    	if(bom != 0x0000FEFF)
		{

			m1 = c;

			m1 = m1 & 0x00FF0000;

			c = c >> 24;
			m1 = m1 >> 8;

			c = c | m1;
		}

		t1 = c;

		c = c & 0x3F;
		t1 = t1 & 0x7C0;

		t1 = t1 >> 6;

		c = c | 0x80;
		t1 = t1 | 0xC0;

		QTDbytes = fwrite(&t1,sizeof(char),1,arquivo_saida);
		if(QTDbytes == 0)
		{
	  		fprintf(stderr,"Erro ao gravar");
	  		return -1;
		}	
		QTDbytes = fwrite(&c,sizeof(char),1,arquivo_saida);
		if(QTDbytes == 0)
		{
	  		fprintf(stderr,"Erro ao gravar");
	  		return -1;
		}	
		i++;
          }
          else
          {	
		if(bom != 0x0000FEFF)
		{	
			c = c >> 24;			
		}
	
		QTDbytes = fwrite(&c,sizeof(char),1,arquivo_saida);	
		if(QTDbytes == 0)
		{
	  		fprintf(stderr,"Erro ao gravar");
	  		return -1;
		}	
		i++;
          }}
	if (i == 0)
	{
	  fprintf(stderr,"Erro ao Ler, Nada foi lido");
	  return -1;
	}
	return 0;
}

