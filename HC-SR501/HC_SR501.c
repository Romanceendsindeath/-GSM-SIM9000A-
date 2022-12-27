#include "HC_SR501.h"

void HC_SR501_init(void){  //有人则输出高电平  PA6

	GPIO_InitTypeDef GPIO_InitHC_SR501;   
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitHC_SR501.GPIO_Mode=GPIO_Mode_IPU;  //下拉输入
	GPIO_InitHC_SR501.GPIO_Pin=GPIO_Pin_6;    //  PA6
	GPIO_InitHC_SR501.GPIO_Speed=GPIO_Speed_50MHz;  //传输速度
	GPIO_Init(GPIOA,&GPIO_InitHC_SR501);
	
	PAout(6)=0;
                                                                                         
}


