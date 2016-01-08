/*
 * DCF_Handler.h

 *
 *  Created on: 15 nov. 2015
 *      Author: Jasper
 */

#ifndef DCF_HANDLER_H_
#define DCF_HANDLER_H_

#include "stm32l1xx.h"
#include <stdbool.h>


void Read_BCD(unsigned char cur_bit, unsigned char bitnr, unsigned char *resultaat);
void Decode (unsigned char dec_array[59], uint8_t *cest_pointer);
void TimeUpdateStatusFlag(int clear);

#endif /* DCF_HANDLER_H_ */
