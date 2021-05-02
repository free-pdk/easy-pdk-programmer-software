/*
Copyright (C) 2019-2021  freepdk  https://free-pdk.github.io

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "fpdkuart.h"
#include "fpdkusb.h"

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#define FPDKUART_QUEUE_SIZE 32                                                                     //32 byte internal RX queue (-> 32 bytes transmitted from IC during debug can be buffered, sending out via USB us MUCH faster)

extern UART_HandleTypeDef huart1;

static uint8_t  _uartRXBuffer[2*1];                                                                //double buffer DMA 1 byte each
static uint8_t  _uartRXQueue[FPDKUART_QUEUE_SIZE];
static uint32_t _uartRXQueueWPos;
static uint32_t _uartRXQueueRPos;
static bool     _uartRXAutoBaudFinished;

static void _FPDKUART_HandleRx(UART_HandleTypeDef *huart, uint8_t c)
{
  if( _uartRXQueueWPos - _uartRXQueueRPos < FPDKUART_QUEUE_SIZE )
    _uartRXQueue[(_uartRXQueueWPos++) % FPDKUART_QUEUE_SIZE] = c;
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
  _FPDKUART_HandleRx( huart, _uartRXBuffer[0] );
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  _FPDKUART_HandleRx( huart, _uartRXBuffer[1] );
}

void FPDKUART_Init(void)
{
  _uartRXQueueWPos = 0;
  _uartRXQueueRPos = 0;
  _uartRXAutoBaudFinished = false;
  HAL_UART_Abort(&huart1);
  HAL_UART_DeInit(&huart1);
  HAL_UART_Init(&huart1);
  HAL_UART_Receive_DMA( &huart1, _uartRXBuffer, sizeof(_uartRXBuffer));
}

void FPDKUART_DeInit(void)
{
  HAL_UART_Abort(&huart1);
  HAL_UART_DeInit(&huart1);
}

void FPDKUART_SendData(const uint8_t* dat, const uint16_t len)
{
  while( HAL_BUSY == HAL_UART_Transmit_IT( &huart1, (uint8_t*)dat, len) ) {;}
}

void FPDKUART_HandleQueue(void)
{
  if( !_uartRXAutoBaudFinished )
  {
    if( __HAL_UART_GET_FLAG(&huart1,UART_FLAG_ABRF) != RESET)                                      //auto baud detection on STM finished?
    {
      if( __HAL_UART_GET_FLAG(&huart1, UART_FLAG_ABRE) != RESET )                                  //auto baud error occured ?
        FPDKUART_Init();                                                                           //restart uart on error
      else
      {
        HAL_UART_Abort(&huart1);                                                                   //reset DMA with new baud rate
        HAL_UART_Receive_DMA( &huart1, _uartRXBuffer, sizeof(_uartRXBuffer));
        _uartRXAutoBaudFinished = true;

        char connectstring[64];
        snprintf( connectstring, sizeof(connectstring), "Connected @%" PRIu32 " baud\n", HAL_RCC_GetPCLK1Freq() / USART1->BRR );
        FPDKUSB_SendDebug((uint8_t*)connectstring, strlen(connectstring));
      }
    }
  }
  else
  {
    if( _uartRXQueueWPos > _uartRXQueueRPos )
    {
      if( (0 == _uartRXQueueRPos) && ('U' == _uartRXQueue[0]) )                                    //skip special auto baud char if detected as first char
        _uartRXQueueRPos++;

      uint32_t sendlen = _uartRXQueueWPos - _uartRXQueueRPos;
      if( sendlen )
      {
        uint32_t rpos = _uartRXQueueRPos % FPDKUART_QUEUE_SIZE;
        if( (rpos + sendlen) > FPDKUART_QUEUE_SIZE )
          sendlen = FPDKUART_QUEUE_SIZE - rpos;
        FPDKUSB_SendDebug(&_uartRXQueue[rpos],sendlen);
        _uartRXQueueRPos += sendlen;
      }
    }
  }
}

