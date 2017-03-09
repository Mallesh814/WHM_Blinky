#include "parser.h"

void str_cpy(char* dest,char* src)
		{
			uint32_t i,len1;
			len1 = str_len(dest);
			for(i=0;src[i] != '\0';i++) dest[i] = src[i];
			if(i > len1) dest[i] = '\0';
		}

char* str_ncpy(char* dest,char* src,uint32_t n)
		{
			uint32_t len1=0,len2=0,i;
			len1 = str_len(dest);
			len2 = str_len(src);
			if(n > len2) n=len2;

			for(i=0;i < n;i++) dest[i] = src[i];
			if(i > len1) dest[i] = '\0';
			return dest;
		}

uint32_t str_len(char* str)
{
	uint32_t len;
	for(len = 0;str[len] != '\0';len++);
	return(len);
}


//****** Function that sends the received STRING to UART interface

void transfer(char* data){
		while (*data){
			UARTCharPut(UART0_BASE, *data);
			data++;
		}
	}

void dec_ascii(uint32_t num,char*pasc){
	uint32_t j;
	j=num;
	do{
		++pasc;
		j=j/10;
	}while(j);
	*pasc = '\0';
	do{
		*(--pasc)=(num%10) + 0x30;
			num = num/10;
	}while(num);
}

uint32_t ascii_dec(char* asc,uint32_t* num)										// blink determines the mode of operation
{
	uint32_t dec=0;
	while(*asc)
		{
			if (((*asc) ^ 0x30) <= 9)	dec = (dec * 10) + ((*asc) ^ 0x30);	// (asc[k] ^ 0x30) will be > 9 for non numerical characters
			else
			{
				*num = dec;
				return 0;														// second element holds the half converted value
			}
			asc++;
		}
	*num = dec;
	return 1;
}

uint32_t ascii_hex_dec(char* asc,uint32_t* num)										// blink determines the mode of operation
{
	uint32_t hex=0;
	while(*asc)
		{
			if((*asc) >= '0' &&  (*asc) <= '9') hex = 	(hex * 16) + ((*asc) - '0');
			else if((*asc) >= 'A' &&  (*asc) <= 'F') hex = 	(hex * 16) + (0xA + (*asc) - 'A');
			else if((*asc) >= 'a' &&  (*asc) <= 'f') hex = 	(hex * 16) + (0xA + (*asc) - 'a');
			else return 0;
			asc++;
		}
	*num = hex;
	return 1;
}
uint32_t int_hex_ascii(char* hex_str, uint32_t num)
{
	uint32_t i=0;
	char str[11]="0x00000000";
	char *pstr = &str[9];
	*hex_str = '\0';//	*(hex_str+1) = 'x';	*(hex_str+2) = '\0';

	while(num){
		i = num%16;

		if(i <= 9)	*pstr = (i + '0');
		else if(i <= 15) *pstr =(i  - 0xA + 'A');	
		else	return 0;

		num = num/16;
		pstr--;
	}
	str_cpy(hex_str,str);
	return 1;
}

