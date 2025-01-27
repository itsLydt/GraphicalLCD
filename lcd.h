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


#endif /* LCD_H_ */