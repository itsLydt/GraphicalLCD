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


enum InstructionMode {
	BASIC,
	EXTENDED
} instr_set;


enum InterfaceMode {
	PARALLEL8,
	PARALLEL4 
	/*,
	SPI	
	*/
} comm_interface;

_Bool graphic_display_on = 0;

void function_set(enum InterfaceMode interface, enum InstructionMode mode);
void ext_function_set(enum InterfaceMode interface, enum InstructionMode mode, _Bool enable_graphic_display);

enum BasicInstructionSet {
	DISPLAY_CLEAR			= 0x01,
	RETURN_HOME				= 0x02,
	ENTRY_MODE_SET			= 0x04,	
	DISPLAY_CONTROL			= 0x08,
	CURSOR_SHIFT_CONTROL	= 0x10,
	FUNCTION_SET			= 0x20,
	SET_CGRAM_ADDR			= 0x40,
	SET_DDRAM_ADDR			= 0x80
};

enum ExtendedInstructionSet {
	STANDBY					= 0x01,
	SCROLL_RAM_SELECT		= 0x02,
	REVERSE_LINE			= 0x04,
	EXTENDED_FUNCTION_SET	= 0x20,
	SET_SCROLL_ADDR			= 0x40,
	SET_GDRAM_ADDR			= 0x80
};

void LCD_Init(){
	// configure GPIO pins that will be used to control the display
	struct GPIO_PinConfig_t t = { .enableInputBuffer = 1, .enablePull = 1 }; 
	GPIO_ConfigurePort(DATA_PORT, DB_MASK << PIN_DB0, GPIO_OUT, &t);
	
	GPIO_SetPortDirection(CTRL_PORT, CTRL_MASK, GPIO_OUT);
	
	function_set(PARALLEL8, BASIC);	// 8-bit mode, basic instruction
	SetDisplayMode(1, 0, 0);	// Display on, cursor off, blink off
	SetEntryMode(1, 0);			// Cursor increment, no shift
	ReturnHome();
	ClearDisplay();
}

void WriteGraphic(uint8_t row, uint8_t col, uint8_t byte){
	SetGDRAMAddr(row, 0);
	write_data(byte);
}

void ClearDisplay() {
	// basic mode
	if(instr_set == EXTENDED){
		// set LCD to basic instruction mode before sending
		ext_function_set(comm_interface, BASIC, graphic_display_on);	
	}
	write_command(DISPLAY_CLEAR);
}

void ReturnHome(){
	// basic mode
	if(instr_set == EXTENDED){
		// set LCD to basic instruction mode before sending
		ext_function_set(comm_interface, BASIC, graphic_display_on);	
	}
	write_command(RETURN_HOME);
}

void SetEntryMode(_Bool cursor_increment, _Bool shift_display){
	// basic mode
	if(instr_set == EXTENDED){
		// set LCD to basic instruction mode before sending
		ext_function_set(comm_interface, BASIC, graphic_display_on);
	}
	uint8_t cmd = ENTRY_MODE_SET;
	cmd |= (cursor_increment << 1);
	cmd |= (shift_display);
	write_command(cmd);
}

/* Set Display Mode: display on/off, cursor on/off, character blink on/off */
void SetDisplayMode(_Bool enable_display, _Bool enable_cursor, _Bool enable_blink){
	// basic mode
	if(instr_set == EXTENDED){
		ext_function_set(comm_interface, BASIC, graphic_display_on);
	}
	uint8_t cmd = DISPLAY_CONTROL;
	cmd |= (enable_display << 2);
	cmd |= (enable_cursor << 1);
	cmd |= (enable_blink);
	write_command(cmd);
}

/* TBT not really sure how this differs from SetEntryMode. Scroll = 1: display will shift,  = 0: cursor will shift. 
Other param refers to shift direction */
void SetCursorDisplayShift(_Bool scroll, _Bool right){
	// basic mode
	if(instr_set == EXTENDED){
		ext_function_set(comm_interface, BASIC, graphic_display_on);
	}
	uint8_t cmd = CURSOR_SHIFT_CONTROL;
	cmd |= (scroll << 3);
	cmd |= (right << 2);
	write_command(cmd);
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
	wait_ready();
}

void SetCGRAMAddr(uint8_t addr){
	//basic command
	if(instr_set == EXTENDED){
		ext_function_set(comm_interface, BASIC, graphic_display_on);
	}
	addr |= (0x01 << 6);
	addr &= (0x7F);
	write_command(addr);
}

void SetGDRAMAddr(uint8_t row, uint8_t col){
	//extended command
	if(instr_set == BASIC){
		function_set(comm_interface, EXTENDED);
	}
	if(!graphic_display_on){
		ext_function_set(comm_interface, EXTENDED, 1);
	}
	row |= (0x01 << 7);
	row &= 0xBF;
	write_command(row);
	col |= (0x01 << 7);
	col &= (0x8F);
	write_command(col);
}


void SetDDRAMAddr(uint8_t addr){
	//basic command
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
	for (uint8_t page = 0; page < 32; page++)
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
	uint8_t data = (DB_MASK << PIN_DB0) & GPIO_ReadPort(DATA_PORT);
	int ac = data & 0x7F;
	return data & (1 << 7);
}

void wait_ready(){
	// set parallel pins to input
	GPIO_SetPortDirection(DATA_PORT, DB_MASK << PIN_DB0, GPIO_IN);
	GPIO_WritePort(DATA_PORT, DB_MASK << PIN_DB0, 1);

	// set transceiver direction to read
	GPIO_WritePin(TRAN_PORT, PIN_DIR, 0);
	
	GPIO_WritePin(CTRL_PORT, PIN_RS, 1);			// RS = 0
	GPIO_WritePin(CTRL_PORT, PIN_RW, 0);			// Read; RW = 1
	GPIO_WritePin(CTRL_PORT, PIN_E, 0);				// E = 1
	while(isBusy());
	GPIO_WritePin(CTRL_PORT, PIN_E, 1);				// E = 0
	
	// set transceiver direction to write
	GPIO_WritePin(TRAN_PORT, PIN_DIR, 1);
	// set parallel pins to output
	GPIO_SetPortDirection(DATA_PORT, DB_MASK << PIN_DB0, GPIO_OUT);
}

void function_set(enum InterfaceMode interface, enum InstructionMode mode){
	uint8_t cmd = FUNCTION_SET;
	if(interface == PARALLEL8){
		cmd |= (1 << 4);
	}
	if(mode == EXTENDED){
		cmd |= (1 << 2);
	}
	write_command(cmd);
	instr_set = mode;
	comm_interface = interface;
}

void ext_function_set(enum InterfaceMode interface, enum InstructionMode mode, _Bool enable_graphic_display){
	uint8_t cmd = EXTENDED_FUNCTION_SET;
	if(interface == PARALLEL8){
		cmd |= (1 << 4);
	}
	if(mode == EXTENDED){
		cmd |= (1 << 2);
	}
	if(enable_graphic_display){
		cmd |= (1 << 1);
	}
	write_command(cmd);
	graphic_display_on = enable_graphic_display;
	instr_set = mode;
	comm_interface = interface;
};


void DrawChar(char c){
	function_set(comm_interface, EXTENDED);
	ext_function_set(comm_interface, EXTENDED, true);
	uint16_t char_code = c + 0xA260;
	write_data((char_code >> 8) & 0xFF);
	write_data(char_code & 0xFF);
}