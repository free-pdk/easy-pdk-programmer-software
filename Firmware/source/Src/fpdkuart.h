#ifndef __FPDKUART_H_
#define __FPDKUART_H_

#include "stdint.h"
#include "stdbool.h"

void FPDKUART_Init(void);
void FPDKUART_DeInit(void);

void FPDKUART_SendData(const uint8_t* dat, const uint16_t len);

void FPDKUART_HandleQueue(void);

#endif //__FPDKUART_H_
