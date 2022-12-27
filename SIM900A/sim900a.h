#ifndef __SIM900A_H__
#define __SIM900A_H__	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
#define SIM_OK 0
#define SIM_COMMUNTION_ERR 0xff
#define SIM_CPIN_ERR 0xfe
#define SIM_CREG_FAIL 0xfd
#define SIM_MAKE_CALL_ERR 0Xfc
#define SIM_ATA_ERR       0xfb

#define SIM_CMGF_ERR 0xfa
#define SIM_CSCS_ERR 0xf9
#define SIM_CSCA_ERR 0xf8
#define SIM_CSMP_ERR 0Xf7
#define SIM_CMGS_ERR       0xf6
#define SIM_CMGS_SEND_FAIL       0xf5

#define SIM_CNMI_ERR 0xf4


extern u8 Flag_Rec_Message;	//收到短信标示
extern u8 Flag_Rec_Call;


extern u8 SIM900_CSQ[3];
extern u8 GSM_Dect(void);
extern u8 sim900a_send_enmessage(void);
extern u8 SIM_HANDLE_MESSAGE_CMD(void);
extern u8 sim900a_send_chmessage_zc(u8*number,u8*content);
#endif





