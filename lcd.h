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
void Clear();

#endif /* LCD_H_ */