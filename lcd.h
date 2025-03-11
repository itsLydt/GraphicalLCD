/*
 * lcd.h
 *
 * Created: 1/24/2025 3:03:47 PM
 *  Author: itsLydt
 *
 *  Written for the NHD-14432WG-BTFH-V#T
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "ATSAMD21/drivers/gpio/gpio.h"
#include "delay.h"

//init LCD
void LCD_Init();

/*	Graphic display has 144 x 32 pixels, aka 32 rows of 8 bytes.
	This function writes 1 byte (8 pixels) at the indicated row and column.
	Note that this will increment the cursor position
*/
void WriteGraphic(uint8_t row, uint8_t col, uint8_t byte);

/* Fill DDRAM with "20H" and set DDRAM address counter (AC) to "00H" */
void ClearDisplay();
/* Set AC to "00H" and put cursor at origin. Contents of DDRAM are unchanged. */
void ReturnHome();
/* Set Entry Mode */
void SetEntryMode(_Bool cursor_increment, _Bool shift_display);
/* Set Display Mode: display on/off, cursor on/off, character blink on/off */
void SetDisplayMode(_Bool enable_display, _Bool enable_cursor, _Bool enable_blink);
/* Set Cursor/Display Shift */
void SetCursorDisplayShift(_Bool scroll, _Bool right);

void SetGDRAMAddr(uint8_t row, uint8_t col);
void SetDDRAMAddr(uint8_t addr);
void SetCGRAMAddr(uint8_t addr);

void write_command(uint8_t cmd);
void write_data(uint8_t data);
void Smile();
void Aww();

void FillWith(uint8_t val);
void FillHalf(uint8_t val, uint8_t mode);


void DrawChar(char c);

#endif /* LCD_H_ */