/*
 * config.h
 *
 * Created: 1/24/2025 3:31:05 PM
 *  Author: itsLydt
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#include "ATSAMD21/drivers/gpio/gpio.h"

// pin and port assignments for LCD control signals
PortGroup* const CTRL_PORT = GPIOA;
const uint8_t PIN_RS = 4;
const uint8_t PIN_RW = 5;
const uint8_t PIN_E  = 6;

// pin and port assignments for LCD parallel interface
const PortGroup* const DATA_PORT = GPIOB;
const uint8_t PIN_DB0 = 0;	// DB pins are assumed to be consecutive


// pin and port assignments for transceiver control signals
const PortGroup* const TRAN_PORT = GPIOA;
// remember that these signals are inverted by the NPN circuit
#define PIN_DIR 9
#define PIN_ENABLE 9

#endif /* CONFIG_H_ */