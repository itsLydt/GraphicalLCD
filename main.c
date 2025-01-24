/*
 * GraphicalLCD.c
 *
 * Created: 1/24/2025 2:38:50 PM
 * Author : itsLydt
 */ 


#include "sam.h"

void TRAN_Init();

int main(void)
{
	TRAN_Init();
	LCD_Init();

    while (1) 
    {
    }
}

/* initialize pins for the transceiver */
void TRAN_Init(){
	GPIO_SetPinDirection(TRAN_PORT, PIN_DIR, GPIO_OUT);
	GPIO_SetPinDirection(TRAN_PORT, PIN_ENABLE, GPIO_OUT);
	GPIO_WritePin(TRAN_PORT, PIN_DIR, 1);	 // high causes DIR to become low, setting the MCU to be the input side of the transceiver
	GPIO_WritePin(TRAN_PORT, PIN_ENABLE, 1); // high causes OE to become low, enabling the transceiver
}