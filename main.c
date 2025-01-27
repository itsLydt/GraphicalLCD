/*
 * GraphicalLCD.c
 *
 * Created: 1/24/2025 2:38:50 PM
 * Author : itsLydt
 */ 


#include "sam.h"
#include "lcd.h"

#define CONFIG_IMPL
#include "config.h"

void TRAN_Init();

int main(void)
{
	TRAN_Init();
	LCD_Init();

    while (1) 
    {
		FillWith(0xFF);
		delay_ms(500);
		ClearDisplay();
		delay_ms(500);
		FillWith(0x0A);
		delay_ms(500);
		ClearDisplay();
		delay_ms(500);
    }
}

/* initialize pins for the transceiver */
void TRAN_Init(){
	GPIO_SetPinDirection(TRAN_PORT, PIN_DIR, GPIO_OUT);
	GPIO_SetPinDirection(TRAN_PORT, PIN_ENABLE, GPIO_OUT);
	GPIO_WritePin(TRAN_PORT, PIN_DIR, 1);	 // high causes DIR to become low, setting the MCU to be the input side of the transceiver
	GPIO_WritePin(TRAN_PORT, PIN_ENABLE, 1); // high causes OE to become low, enabling the transceiver
}