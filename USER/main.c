#include "string.h"
#include "sim900a.h"
#include "usart2.h"
#include "sys.h"
#include "delay.h"
#include "JDQ.h"
#include "HC_SR501.h"
#include "BEEP.h"

int main(void){
	
	u8 res=1;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 
	USART2_Init(115200);	//��ʼ������2 
	USART3_Init(9600);	//
	JDQ_init();
	delay_init();
	HC_SR501_init();
	BEEP_init();
	res=1;  //��־λ��һ
	BEEP_text = 1;  //�������ر�
	PAout(4) = 0;   //�̵���1�ر�
	PAout(5) = 0;   //�̵���2�ر�
	
		while(res)
	{
		res=GSM_Dect();
		delay_ms(2000);
	}	
	res=1;
	while(res)
	{
		res=sim900a_send_enmessage();
	}
	while(1)
	{
		if(Flag_Rec_Message==1)	   //���յ�����  
		{
			Flag_Rec_Message=0;  
			res=SIM_HANDLE_MESSAGE_CMD();
			
			if(res!=5)
			{ //����ģʽ��Ӧ״̬
				if(res==1)		PAout(4) = 1;
				else if(res==2)	PAout(4) = 0;
				else if(res==3)	PAout(5) = 1;
				else if(res==4)	PAout(5) = 0;
			}	
		}
		
	if( res == 5 ){
		if(HC_SR501_text == 1){  //���˴��뱨��
				BEEP_text = 0;
		sim900a_send_chmessage_zc((u8*)"00310035003600310036003600300038003900300032  ",(u8*)"67094EBA95EF5165FF01 ");	//���͵�18028957220������Ϊ  ���˴���(00310035003600310036003600300038003900300032)
				delay_ms(3000);
			}
		BEEP_text = 1;
		}

	}
}


