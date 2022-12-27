#include "JDQ.h"

void JDQ_init(void){  //PA4  PA5  高电平触发

	GPIO_InitTypeDef GPIO_InitJDQ;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitJDQ.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitJDQ.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitJDQ.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitJDQ);
}

