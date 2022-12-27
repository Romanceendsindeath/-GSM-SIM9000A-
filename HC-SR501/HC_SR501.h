#ifndef __HC_SR501_H

#define __HC_SR501_H

#include "sys.h"

#define HC_SR501_text GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)

void HC_SR501_init(void);

#endif
