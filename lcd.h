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

void FillWith(uint8_t val);

/* Fill DDRAM with "20H" and set DDRAM address counter (AC) to "00H" */
void ClearDisplay();
/* Set AC to "00H" and put cursor at origin. Contents of DDRAM are unchanged. */
void ReturnHome();
/* Set Entry Mode */
void SetEntryMode(_Bool cursor_increment, _Bool shift_display);
/* Set Display Mode: display on/off, cursor on/off, character blink on/off */
void SetDisplayMode(_Bool enable_display, _Bool enable_cursor, _Bool enable_blink);

void SetGDRAMAddr(uint8_t row, uint8_t col);
/* not clear what this does */
void SetDDRAMAddr(uint8_t addr);
void write_data(uint8_t data);
void Smile();
void Aww();

void DrawChar(char c);

#endif /* LCD_H_ */