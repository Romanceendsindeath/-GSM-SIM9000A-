#include "BEEP.h"

void BEEP_init(void){
	
	GPIO_InitTypeDef GPIO_InitBEEP;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitBEEP.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitBEEP.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitBEEP.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitBEEP);
	
	BEEP_text = 1;  //¹Ø±Õ·äÃùÆ÷
}

