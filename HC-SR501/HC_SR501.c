#include "HC_SR501.h"

void HC_SR501_init(void){  //����������ߵ�ƽ  PA6

	GPIO_InitTypeDef GPIO_InitHC_SR501;   
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitHC_SR501.GPIO_Mode=GPIO_Mode_IPU;  //��������
	GPIO_InitHC_SR501.GPIO_Pin=GPIO_Pin_6;    //  PA6
	GPIO_InitHC_SR501.GPIO_Speed=GPIO_Speed_50MHz;  //�����ٶ�
	GPIO_Init(GPIOA,&GPIO_InitHC_SR501);
	
	PAout(6)=0;
                                                                                         
}


