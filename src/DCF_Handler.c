/*
 * DCF_Handler.c

 *
 *  Created on: 15 nov. 2015
 *      Author: Jasper
 */
#include "DCF_Handler.h"
#include "main.h"



/*Status, in during startup FALSE, after set up time TRUE*/
bool TimeStatus = false;

void TimeUpdateStatusFlag(int status)
{
  if (status == 0)
  {
    TimeStatus = false;
  }
  else if (status == 1)
  {
    TimeStatus = true;
  }
}

/*
Function Decode
Decodes the DCF array which is passed from the SysTick Int
the function checks for parity and decodes:
Minute, Hour, Day, Month, Year
*/
void Decode (unsigned char dec_array[59], uint8_t *cest_pointer)
{
  uint8_t bitje;
//  uint8_t CEST_//BIT; NOT USED
  /*Declare paritybit status*/
  uint8_t parity;
  uint8_t ParityMin;
  uint8_t ParityHrs;
  uint8_t ParityDate;
  /*Store the DCF information*/
  unsigned char dcf_min;
  unsigned char dcf_uur;
  unsigned char dcf_dag;
  unsigned char dcf_mnd;
  unsigned char dcf_jaar;

  /*
  Function that checks the parity for the minute bits
  Bit 28 is parity bit for minute info
  */
  for (bitje=21; bitje<28; bitje++){
    if (dec_array[bitje]==1) parity++;
  }
  if (parity%2 == dec_array[28]){
    ParityMin = 1; //Parity is OK
  }
  else{
    ParityMin = 0; //Parity NOT OK
  }

  parity=0;

  /*Bit 35 is parity*/
  for (bitje=29; bitje<35; bitje++){
    if (dec_array[bitje]==1) parity++;
  }
  if (parity%2 == dec_array[35]){
    ParityHrs = 1;
  }
  else{
    ParityHrs = 0;
  }

  parity=0;
  /*Bit 58 is parity*/
  for (bitje=36; bitje<58; bitje++){
    if (dec_array[bitje]==1) parity++;
  }
  if (parity%2 == dec_array[58]){
    ParityDate = 1;
  }
  else{
    ParityDate = 0;
  }
  /*When all parity ckecks are ok: Decode the whole array*/
  if ((ParityMin==1)&&(ParityHrs==1)&&(ParityDate==1)){
  /*Check for CEST*/
  /*When bit 17 is high, CEST is active (Central European Summer Time)*/
  /*when bit 18 is high, CET is active (Central European Time)*/
    if(dec_array[17] == 1){
      *cest_pointer = 1;
    }
    else if (dec_array[18] == 1){
      *cest_pointer = 0;
    }

    /* Below, decode the minute */
    dcf_min = 0;
    for( bitje = 0; bitje < 7; bitje++){
    /* bit 21 - 27 are the minutes */
      Read_BCD(dec_array[21 + bitje], bitje, &dcf_min);
    }
    /* Below, decode the hour */
    dcf_uur = 0;
    for( bitje = 0; bitje < 6; bitje++){
    /* bitnr 29 tot 35 bevat de minuten info */
      Read_BCD(dec_array[29 + bitje], bitje, &dcf_uur); /* bitje omzetten naar bcd */
    }
    /* Hieronder de dagen die we gaan decoderen */
    dcf_dag = 0;
    for( bitje = 0; bitje < 6; bitje++){
      Read_BCD(dec_array[36 + bitje], bitje, &dcf_dag);
    }
    /* Hieronder de maand die we gaan decoderen */
    dcf_mnd = 0;
    for( bitje = 0; bitje < 5; bitje++){
      Read_BCD(dec_array[45 + bitje], bitje, &dcf_mnd);
    }
    /* Hieronder de jaren die we gaan decoderen */
    dcf_jaar = 0;
    for( bitje = 0; bitje < 8; bitje++){
      Read_BCD(dec_array[50 + bitje], bitje, &dcf_jaar);
    }


    if (TimeStatus == false){

      SetDate_Configuration(dcf_jaar, dcf_mnd, dcf_dag);
      HAL_Delay(200);
      SetTime_Configuration(dcf_uur, dcf_min);
      /*Indicate that time is updated*/
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
      TimeUpdateStatusFlag(1);
      Update_RTC();
//      SunSetCalculationFlag = false;
    }
  }
}

void Read_BCD(unsigned char cur_bit, unsigned char bitnr, unsigned char *resultaat){
  switch( bitnr){
    case 0:
      *resultaat = cur_bit; /* bit waarde 1 */
      break;
    case 1:
      *resultaat += cur_bit*2; /* bit waarde x 2 */
      break;
    case 2:
      *resultaat += cur_bit*4; /* bit waarde x 4 */
      break;
    case 3:
      *resultaat += cur_bit*8; /* bit waarde x 8 */
      break;
    case 4:
      *resultaat += cur_bit*10; /* bit waarde x 10 */
      break;
    case 5:
      *resultaat += cur_bit*20; /* bit waarde x 20 */
      break;
    case 6:
      *resultaat += cur_bit*40; /* bit waarde x 40 */
      break;
    case 7:
      *resultaat += cur_bit*80; /* bit waarde x 80 */
      break;
  }
}

/*
The function below sets the Timestatus to false at 5.30 in the morning
When TimeStatus is set to false, the RTC is updated within the next complete received time frame
*/
void CheckForUpdate(unsigned char Hours, unsigned char Minutes){
  if ((Hours == 05) && (Minutes == 30)){
    TimeStatus = false;
  }
}


