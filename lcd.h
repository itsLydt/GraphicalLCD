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

extern const PortGroup* const CTRL_PORT;
extern const uint8_t PIN_RS;
extern const uint8_t PIN_RW;
extern const uint8_t PIN_E;

extern const PortGroup* const DATA_PORT;
extern const uint8_t PIN_DB0;

//init LCD
void LCD_Init();




#endif /* LCD_H_ */