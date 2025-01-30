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

	ClearDisplay();
	FillWith(0xFF);
	//write_command(0x30); // change to basic instruction mode
	ReturnHome();
	
	char c = 'A';
	_Bool page = 0;
	uint8_t background = 0;
	
	DrawChar('H');
	DrawChar('i');
	delay_ms(2000);
	while (1) 
	{
		switch(background){
		case 0:
			FillWith(0xFF);
			write_command(0x30); // change back to basic instruction mode
			break;
		case 1: 
			FillHalf(0xFF, 0);
			write_command(0x30); // change back to basic instruction mode
			break;
		case 2: 
			FillHalf(0xFF, 1);
			write_command(0x30);
			break;
		default:
			write_command(0x34); // set extended function mode on + GD off
			write_command(0x30); // change back to basic instruction mode
			break;
		}
		background = (++background) % 4;
		
		if(page){
			SetDDRAMAddr(16);    
		}
		else {
			ReturnHome();
		}
		page = !page;

		for(int i = 0; i < 18; i++){
			write_data(c);
			++c;
			if(c > 126){
				c = 33;
			}
			delay_ms(500);
		}
    }
}

/* initialize pins for the transceiver */
void TRAN_Init(){
	GPIO_SetPinDirection(TRAN_PORT, PIN_DIR, GPIO_OUT);
	GPIO_SetPinDirection(TRAN_PORT, PIN_ENABLE, GPIO_OUT);
	GPIO_WritePin(TRAN_PORT, PIN_DIR, 1);	 // high causes DIR to become low, setting the MCU to be the input side of the transceiver
	GPIO_WritePin(TRAN_PORT, PIN_ENABLE, 1); // high causes OE to become low, enabling the transceiver
}