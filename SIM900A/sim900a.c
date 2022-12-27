#include "sim900a.h"
#include "usart.h"		
#include "delay.h"	 
#include "string.h" 
#include "usart2.h" 
#include "math.h"
#include "stdio.h"
//********************************************************************************
//无
//////////////////////////////////////////////////////////////////////////////////	
u8 SIM900_CSQ[3];
u8 dtbuf[50];   								//打印缓存器	
u8 Flag_Rec_Message=0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//usmart支持部分 
//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART2_RX_STA;
//     1,清零USART2_RX_STA;
void sim_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		printf("%s",USART2_RX_BUF);	//发送到串口
		if(mode)USART2_RX_STA=0;		
	} 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//ATK-SIM900A 各项测试(拨号测试、短信测试、GPRS测试)共用代码
//sim900a发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* sim900a_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//向sim900a发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;USART2_RX_REC_ATCOMMAD=1;
	if((u32)cmd<=0XFF)
	{
		while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
		USART2->DR=(u32)cmd;
	}else u2_printf("%s\r\n",cmd);//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(sim900a_check_cmd(ack))break;//得到有效数据 
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	USART2_RX_STA=0;USART2_RX_REC_ATCOMMAD=0;
	return res;
} 

u8 sim900a_work_test(void)
{
	if(sim900a_send_cmd((u8 *)"AT",(u8 *)"OK",100))
	{
		if(sim900a_send_cmd((u8 *)"AT",(u8 *)"OK",100))return SIM_COMMUNTION_ERR;	//通信不上
	}		
	if(sim900a_send_cmd((u8 *)"AT+CPIN?",(u8 *)"READY",400))return SIM_CPIN_ERR;	//没有SIM卡
	if(sim900a_send_cmd((u8 *)"AT+CREG?",(u8 *)"0,1",400))
	{
		if(strstr((const char*)USART2_RX_BUF,"0,5")==NULL)
		{
			 if(!sim900a_send_cmd((u8 *)"AT+CSQ",(u8 *)"OK",200))	
			 {
					memcpy(SIM900_CSQ,USART2_RX_BUF+15,2);
			 }
			 return SIM_CREG_FAIL;	//等待附着到网络
		}
	}	
	return SIM_OK;
}
u8 GSM_Dect(void)
{
	u8 res;
	res=sim900a_work_test();	
	switch(res)
	{
		case SIM_OK:
			UART3SendString("GSM模块自检成功\r\n",strlen("GSM模块自检成功\r\n"));
			break;
		case SIM_COMMUNTION_ERR:
			UART3SendString("与GSM模块未通讯成功，请等待\r\n",strlen("与GSM模块未通讯成功，请等待\r\n"));
			break;
		case SIM_CPIN_ERR:
			UART3SendString("没检测到SIM卡\r\n",strlen("没检测到SIM卡\r\n"));	
			break;
		case SIM_CREG_FAIL:
			UART3SendString("注册网络中。。。\r\n",strlen("注册网络中。。。\r\n"));	
			UART3SendString("当前信号值：",strlen("当前信号值："));	UART3SendString(SIM900_CSQ,2);UART3SendString("\r\n",2);
			break;		
		default:
			break;
	}
	return res;
}
u8 sim900a_send_enmessage(void)	//让数字，字母等内容以英文输出
{	
	if(sim900a_send_cmd((u8 *)"AT+CMGF=1",(u8 *)"OK",100))	return SIM_CMGF_ERR;
	if(sim900a_send_cmd((u8 *)"AT+CSCS=\"GSM\"",(u8 *)"OK",100))	return SIM_CSCS_ERR;
	if(sim900a_send_cmd((u8 *)"AT+CSCA?",(u8 *)"OK",100))	return SIM_CSCA_ERR;
	if(sim900a_send_cmd((u8 *)"AT+CSMP=17,167,0,241",(u8 *)"OK",100))	return SIM_CSMP_ERR;
	if(sim900a_send_cmd((u8 *)"AT+CNMI=2,2",(u8 *)"OK",200))return SIM_CNMI_ERR;	//设置短信自动上报
	return SIM_OK;
}

u8 SIM_HANDLE_MESSAGE_CMD(void)
{
	if(strstr((char*)USART2_RX_BUF,"aa")!=NULL)			return 1;
	else if(strstr((char*)USART2_RX_BUF,"bb")!=NULL)	return 2;
	else if(strstr((char*)USART2_RX_BUF,"cc")!=NULL)	return 3;
	else if(strstr((char*)USART2_RX_BUF,"dd")!=NULL)	return 4;
	else if(strstr((char*)USART2_RX_BUF,"ee")!=NULL)	return 5;
	return 0;
}

u8 sim900a_send_chmessage_zc(u8*number,u8*content)
{
	u8 cmd[100];
	sprintf((char*)cmd,"AT+CMGS=\"%s\"",number);
	if(sim900a_send_cmd((u8 *)"AT+CMGF=1",(u8 *)"OK",100))	return SIM_CMGF_ERR;
	if(sim900a_send_cmd((u8 *)"AT+CSCS=\"UCS2\"",(u8 *)"OK",100))	return SIM_CSCS_ERR;
	if(sim900a_send_cmd((u8 *)"AT+CSCA?",(u8 *)"OK",100))	return SIM_CSCA_ERR;
	if(sim900a_send_cmd((u8 *)"AT+CSMP=17,167,0,25",(u8 *)"OK",100))	return SIM_CSMP_ERR;
	if(sim900a_send_cmd(cmd,(u8 *)">",100))	return SIM_CMGS_ERR;
	UART2SendString(content,strlen((char*)content));
	//u2_printf("%s",content);
	if(sim900a_send_cmd((u8 *)0x1A,(u8 *)"+CMGS:",1500))	return SIM_CMGS_SEND_FAIL;
	return SIM_OK;
}


