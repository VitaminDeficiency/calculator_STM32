#include "main.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
#include "keypad.h"

char Elements[10];
Elements[0] = '\0';
char ElementsTemp[10] = {"0"};

float number1 = 0.0f, number2 = 0.0f;
int p = 0, K = 0, t = 0, flag = 0, flag2 = 0, dp = 0, idx = 0, idx2 = 0;

void EOS_init(unsigned int x );
void pick(void);
void math();
void scanLED(void);
void insert(unsigned char X);
void reverse(char *str, int len);
int intToStr(int x, char str[], int d);
void addNeg(char* str, int neg);
void ftoa(float n, char *res, int afterpoint);
void popDot(char* a, unsigned char chr, char* b);
void xoaso0phanle(char str[]);
int count(int numCount);
void print(unsigned char num);
void clear();

void EOS_init(unsigned int x ){
	SysTick->LOAD = 8000000/1000*x;
	SysTick->VAL  = 0;
	SysTick->CTRL = 7;
}

void pick(void)
{
	p = keyPad();
	if(flag==0)
	{
		if(p == 0){ insert(0x30); flag = 1; }
		if(p == 1){	insert(0x31); flag = 1; }
		if(p == 2){ insert(0x32); flag = 1; }
		if(p == 3){ insert(0x33); flag = 1; }
		if(p == 4){ insert(0x34); flag = 1; }
		if(p == 5){ insert(0x35); flag = 1; }
		if(p == 6){ insert(0x36); flag = 1; }
		if(p == 7){ insert(0x37); flag = 1; }
		if(p == 8){ insert(0x38); flag = 1; }
		if(p == 9){ insert(0x39); flag = 1; }
		if(p == 10){ K = 5; flag = 1; }
		if(p == 11){ K = 3; flag = 1; }
		if(p == 12){ if(Elements[0]=='\0') insert(0x2d); else K = 2; flag = 1; }
		if(p == 13){ clear(); flag = 1; }
		if(p == 14){ K = 4; flag = 1; }
		if(p == 15){ K = 1; flag = 1; }
	}
	if (p == -1 && flag == 1) flag = 0;
}

void math(void)
{
	pick();
  switch (K)
  {
  case 1:
    number1 = atof(Elements);
    Elements[0] = '\0';
    t = 1;
    break;
  case 2:
    number1 = atof(Elements);
    Elements[0] = '\0';
    t = 2;
    break;
  case 3:
    number1 = atof(Elements);
    Elements[0] = '\0';
    t = 3;
    break;
  case 4:
    number2 = atof(Elements);
    Elements[0] = '\0';
    if(t == 0) number1 =  number2;
    if(t == 1) number1 += number2;
    if(t == 2) number1 -= number2;
    if(t == 3) number1 *= number2;
    if(t == 4) number1 /= number2;
    if((number1>99999999.0)||(number1<-9999999.0)) clear();
		else
		{
			ftoa(number1, Elements, 8);
			if(flag2 == 1) xoaso0phanle(Elements);
		}
    t = 0;
    break;
  case 5:
    number1 = atof(Elements);
    Elements[0] = '\0';
    t = 4;
    break;
  }
  K = 0;
}

void scanLED(void)
{
	popDot(Elements, 0x2e, ElementsTemp);
	idx2 = (strlen(Elements) > 9) ? 9:(strlen(Elements));
  dp = (Elements[idx2 - 1 - idx] != 0x2e) ? 0:1;
  print(ElementsTemp[idx]);
	if (ElementsTemp[idx] != '\0')
	{
		switch(idx)
		{
			case 0: GPIOA->BSRR = GPIO_PIN_15; break;
			case 1: GPIOA->BSRR = GPIO_PIN_14; break;
			case 2: GPIOA->BSRR = GPIO_PIN_13; break;
			case 3: GPIOA->BSRR = GPIO_PIN_12; break;
			case 4: GPIOA->BSRR = GPIO_PIN_11; break;
			case 5: GPIOA->BSRR = GPIO_PIN_10; break;
			case 6: GPIOA->BSRR = GPIO_PIN_9; break;
			case 7: GPIOA->BSRR = GPIO_PIN_8; break;
		}
		idx++;
	} else
	{
		idx = 0;
		GPIOA->ODR = 0x00;
	}
	if(idx>7) idx = 0;
}

void insert(unsigned char X)
{
  for(int i = 0; i < 8; i++)
  if(Elements[i] == '\0')
  {
    Elements[i] = X;
    Elements[i+1] = '\0';
    break;
  }
}

void reverse(char *str, int len)
{
  int i=0, j=len-1, temp;
  while (i<j)
  {
      temp = str[i];
      str[i] = str[j];
      str[j] = temp;
      i++; j--;
  }
}

int intToStr(int x, char str[], int d)
{
  int i = 0;
  while (x)
  {
      str[i++] = (x%10) + '0';
      x /= 10;
  }
  while (i < d) str[i++] = '0';
  reverse(str, i);
  str[i] = '\0';
  return i;
}

void addNeg(char* str, int neg)
{
  int i = (strlen(str)>8) ? 8:strlen(str);
  if (neg)
  {
    while (i>0)
    {
      str[i] = str[i-1];
      i--;
    }
    str[i] = '-';
  }
}

void ftoa(float n, char *res, int afterpoint)
{
  int neg = 0;
  if (n < 0)
  {
    n *= -1;
    neg = 1;
  }
	int ipart;
	if(n<1) ipart = 0;
  else ipart = (int)n;

  float fpart = n - (float)ipart;
	
	if (fpart) flag2 = 1;
	else flag2 = 0;
	
  int i = intToStr(ipart, res, 1);

  fpart = fpart * pow(10, afterpoint);
  
  if (fpart)
  {
      res[i] = 0x2e;
      
      intToStr((int)fpart, res + i + 1, afterpoint);
  }
  addNeg(res,neg);
}

/*--------------------*/

void popDot(char* strIn, unsigned char dot, char* strOut)
{
  int a_cnt, b_cnt = 0, x;
  if (strlen(strIn) > 8) x = 8;
  else x = strlen(strIn)-1;
  for (a_cnt = x; a_cnt >= 0; a_cnt--)
  {
    if ( strIn[a_cnt] != dot )
    {
      strOut[b_cnt] = strIn[a_cnt];
      ++b_cnt;
    }
  }
  strOut[b_cnt] = '\0';
}

void xoaso0phanle(char str[])
{
	int i = 8;
	while((str[i]=='0')||(str[i]=='\0'))
	{
		str[i] = '\0';
		i--;
	}
}

void print(unsigned char num)
{
  switch (num)
  {
  case 0x30:
    if(dp == 1) GPIOA->ODR = 0x40;
    else GPIOA->ODR = 0xc0;
    break;
  case 0x31:
    if(dp == 1) GPIOA->ODR = 0x79;
    else GPIOA->ODR = 0xf9;
    break;
  case 0x32:
    if(dp == 1) GPIOA->ODR = 0x24;
    else GPIOA->ODR = 0xa4;
    break;
  case 0x33:
    if(dp == 1) GPIOA->ODR = 0x30;
    else GPIOA->ODR = 0xb0;
    break;
  case 0x34:
    if(dp == 1) GPIOA->ODR = 0x19;
    else GPIOA->ODR = 0x99;
    break;
  case 0x35:
    if(dp == 1) GPIOA->ODR = 0x12;
    else GPIOA->ODR = 0x92;
    break;
  case 0x36:
    if(dp == 1) GPIOA->ODR = 0x02;
    else GPIOA->ODR = 0x82;
    break;
  case 0x37:
    if(dp == 1) GPIOA->ODR = 0x78;
    else GPIOA->ODR = 0xf8;
    break;
  case 0x38:
    if(dp == 1) GPIOA->ODR = 0x00;
    else GPIOA->ODR = 0x80;
    break;
  case 0x39:
    if(dp == 1) GPIOA->ODR = 0x10;
    else GPIOA->ODR = 0x90;
    break;
	case 0x2d:
		GPIOA->ODR = 0xbf;
		break;
  }
}

void clear(void)
{
  Elements[0] = '\0';
  ElementsTemp[0] = '\0';
  number1 = 0;
  number2 = 0;
}
