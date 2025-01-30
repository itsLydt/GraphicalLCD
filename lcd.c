/*
 * lcd.c
 *
 * Created: 1/24/2025 3:08:01 PM
 *  Author: itsLydt
 */ 

#include "lcd.h"
#include "config.h"
#include "delay.h"

const uint8_t DB_MASK = 0xFF;
#define CTRL_MASK ((1 << PIN_RS) | (1 << PIN_RW) | (1 << PIN_E))

void write_command(uint8_t cmd);
void wait_ready();

void LCD_Init(){
	// configure GPIO pins that will be used to control the display
	struct GPIO_PinConfig_t t = { .enableInputBuffer = 1, .enablePull = 1 }; 
	GPIO_ConfigurePort(DATA_PORT, DB_MASK << PIN_DB0, GPIO_OUT, &t);
	
	GPIO_SetPortDirection(CTRL_PORT, CTRL_MASK, GPIO_OUT);
	
	write_command(0x38);        //Function Set: 8-bit mode, basic instruction
	SetDisplayMode(1, 0, 0);	// Display on, cursor off, blink off
	SetEntryMode(1, 0);			// Cursor increment, no shift
	ReturnHome();
	ClearDisplay();
}

void WriteGraphic(uint8_t row, uint8_t col, uint8_t byte){
	SetGDRAMAddr(row, 0);
	write_data(byte);
}

void write_command(uint8_t cmd){
	
	GPIO_WritePin(CTRL_PORT, PIN_RS, 1);			// RS = 0
	GPIO_WritePin(CTRL_PORT, PIN_RW, 1);			// Write; RW = 0
	GPIO_WritePin(CTRL_PORT, PIN_E, 0);				// E = 1
	GPIO_WriteValue(DATA_PORT, DB_MASK << PIN_DB0, cmd);	// write command data bus
	
	// hold data / E signal for 150ns
	//delay_us(1);
	GPIO_WritePin(CTRL_PORT, PIN_E, 1);				// E = 0
	wait_ready();
}

void write_data(uint8_t data){
	
	GPIO_WritePin(CTRL_PORT, PIN_RS, 0);			// RS = 1
	GPIO_WritePin(CTRL_PORT, PIN_RW, 1);			// Write; RW = 0
	GPIO_WritePin(CTRL_PORT, PIN_E, 0);				// E = 1
	GPIO_WriteValue(DATA_PORT, DB_MASK << PIN_DB0, data);	// write command data bus
	
	// hold data / E signal for 150ns
	//delay_us(1);
	GPIO_WritePin(CTRL_PORT, PIN_E, 1);				// E = 0
	delay_ns(20);
}

void ClearDisplay() {
	write_command(0x30);
	write_command(0x01);
}

void ReturnHome(){
	write_command(0x30);
	write_command(0x02);
}

void SetEntryMode(_Bool cursor_increment, _Bool shift_display){
	uint8_t cmd = 0x04;
	cmd |= (cursor_increment << 1);
	cmd |= (shift_display);
	write_command(cmd);
}

/* Set Display Mode: display on/off, cursor on/off, character blink on/off */
void SetDisplayMode(_Bool enable_display, _Bool enable_cursor, _Bool enable_blink){
	uint8_t cmd = 0x08;
	cmd |= (enable_display << 2);
	cmd |= (enable_cursor << 1);
	cmd |= (enable_blink);
	write_command(cmd);
}

void SetCGRAMAddr(uint8_t addr){
	addr |= (0x01 << 6);
	addr &= (0x7F);
	write_command(addr);
}

void SetGDRAMAddr(uint8_t row, uint8_t col){
	write_command(0x3E);
	row |= (0x01 << 7);
	row &= 0xBF;
	write_command(row);
	col |= (0x01 << 7);
	col &= (0x8F);
	write_command(col);
}


void SetDDRAMAddr(uint8_t addr){
	addr |= (0x01 << 7);
	addr &= (0xBF);
	write_command(addr);
}

void FillRowWith(uint8_t row, uint8_t val){
	SetGDRAMAddr(row, 0);
	for (int x = 0;x<18;x++)
	{
		write_data(val);
	}
}

void FillWith(uint8_t val)
{
	ReturnHome();
	for (uint8_t page = 0x80; page<0xA1; page++)
	{
		FillRowWith(page, val);
	}
}

void FillHalf(uint8_t val, uint8_t mode){
	for (uint8_t row = 0; row < 32; row++)
	{
		SetGDRAMAddr(row, 0);
		for (int x = 0;x<9;x++)
		{
			write_data(mode? 0 : val);
		}
		for (int x = 0;x<9;x++)
		{
			write_data(mode? val : 0);
		}
	}
}

void Smile(){
	unsigned int page = 0;
	for (int i = 0; i < 5; i++)
	{
		FillRowWith(page++, 0x00);
		FillRowWith(page++, 0x24);
		FillRowWith(page++, 0x00);
		FillRowWith(page++, 0x42);
		FillRowWith(page++, 0x3C);
		FillRowWith(page++, 0x00);
	}
	
	FillRowWith(page++, 0x00);
	FillRowWith(page++, 0x00);
}

void Aww(){
	unsigned int page = 0;
	for (int i = 0; i < 5; i++)
	{
		FillRowWith(page++, 0x66);
		FillRowWith(page++, 0x99);
		FillRowWith(page++, 0x42);
		FillRowWith(page++, 0x24);
		FillRowWith(page++, 0x18);
		FillRowWith(page++, 0x00);
	}
		
	FillRowWith(page++, 0x00);
	FillRowWith(page++, 0x00);

}

_Bool isBusy(){
	// assume port settings have been changed appropriately
	GPIO_WritePin(CTRL_PORT, PIN_RS, 1);			// RS = 0
	GPIO_WritePin(CTRL_PORT, PIN_RW, 0);			// Read; RW = 1
	GPIO_WritePin(CTRL_PORT, PIN_E, 0);				// E = 1
	delay_ns(140);
	uint8_t data = (DB_MASK << PIN_DB0) & GPIO_ReadPort(DATA_PORT);
	GPIO_WritePin(CTRL_PORT, PIN_E, 1);				// E = 0
	volatile int ac = data & 0x7F;
	if(data > 0){
		volatile int t = 5;
	}
	return data & (1 << 7);
}

void wait_ready(){
	// set parallel pins to input
	GPIO_SetPortDirection(DATA_PORT, DB_MASK << PIN_DB0, GPIO_IN);
	
	// disable transceiver
	GPIO_WritePin(TRAN_PORT, PIN_ENABLE, 0); 
	
	// set transceiver direction to read
	GPIO_WritePin(TRAN_PORT, PIN_DIR, 0);

	// enable transceiver
	GPIO_WritePin(TRAN_PORT, PIN_ENABLE, 1);
	
	while(isBusy());
	
	// set data bus back to output, etc
	// disable transceiver
	GPIO_WritePin(TRAN_PORT, PIN_ENABLE, 0);
	// set transceiver direction to write
	GPIO_WritePin(TRAN_PORT, PIN_DIR, 1);
	// enable transceiver
	GPIO_WritePin(TRAN_PORT, PIN_ENABLE, 1);
	// set parallel pins to output
	GPIO_SetPortDirection(DATA_PORT, DB_MASK << PIN_DB0, GPIO_OUT);
}

void DrawChar(char c){
	write_command(0x34); // set extended function mode on + GD off
	write_command(0x02);	// set SR = 0
	write_command(0x30); // change to basic instruction mode
	SetDisplayMode(0, 1, 1);
	SetCGRAMAddr(0);	
	write_data(c);
}