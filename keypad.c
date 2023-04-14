#include "main.h"

#define Port GPIOB
int keybuff[3] = {-1, -1, -1};
int keyPad(void);

int keyScanEOS(void)
{
	int key = keyPad();
	keybuff [0] = keybuff[1];
	keybuff [1] = keybuff[2];
	keybuff [2] = key;
	if((keybuff[0] == keybuff[1])&&(keybuff[2] == -1))
	{
		int n = keybuff[0];
		return n;
	}
}

int keyPad(void)
{
	int number = -1;
	Port->BSRR = ~GPIO_PIN_4;
	Port->BSRR = GPIO_PIN_5;
	Port->BSRR = GPIO_PIN_6;
	Port->BSRR = GPIO_PIN_7;
	if (!(Port->IDR & GPIO_PIN_0)) number = 7;
	else if(!(Port->IDR & GPIO_PIN_1)) number = 8;
	else if(!(Port->IDR & GPIO_PIN_2)) number = 9;
	else if(!(Port->IDR & GPIO_PIN_3)) number = 10;
	
	Port->BSRR = GPIO_PIN_4;
	Port->BSRR = ~GPIO_PIN_5;
	if (!(Port->IDR & GPIO_PIN_0)) number = 4;
	else if(!(Port->IDR & GPIO_PIN_1)) number = 5;
	else if(!(Port->IDR & GPIO_PIN_2)) number = 6;
	else if(!(Port->IDR & GPIO_PIN_3)) number = 11;
	
	Port->BSRR = GPIO_PIN_5;
	Port->BSRR = ~GPIO_PIN_6;
	if (!(Port->IDR & GPIO_PIN_0)) number = 1;
	else if(!(Port->IDR & GPIO_PIN_1)) number = 2;
	else if(!(Port->IDR & GPIO_PIN_2)) number = 3;
	else if(!(Port->IDR & GPIO_PIN_3)) number = 12;
	
	Port->BSRR = GPIO_PIN_6;
	Port->BSRR = ~GPIO_PIN_7;
	if (!(Port->IDR & GPIO_PIN_0)) number = 13;
	else if(!(Port->IDR & GPIO_PIN_1)) number = 0;
	else if(!(Port->IDR & GPIO_PIN_2)) number = 14;
	else if(!(Port->IDR & GPIO_PIN_3)) number = 15;
	
	Port->BSRR = GPIO_PIN_7;
	return number;
}
