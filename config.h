/*
 * config.h
 *
 * Created: 1/24/2025 3:31:05 PM
 *  Author: itsLydt
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#include "ATSAMD21/drivers/gpio/gpio.h"

#define SYSTEM_CLOCK_FREQ 1000000U


// pin and port assignments for LCD control signals
PortGroup* const CTRL_PORT;
const uint8_t PIN_RS;
const uint8_t PIN_RW;
const uint8_t PIN_E;

// pin and port assignments for LCD parallel interface
PortGroup* const DATA_PORT;
const uint8_t PIN_DB0;	// DB pins are assumed to be consecutive

// pin and port assignments for transceiver control signals
PortGroup* const TRAN_PORT;
// remember that these signals are inverted by the NPN circuit
#define PIN_DIR 9
#define PIN_ENABLE 8

/* implementation of configurable items, this section is activated by main.c */
#ifdef CONFIG_IMPL
PortGroup* const CTRL_PORT = GPIOA;
const uint8_t PIN_RS = 4;
const uint8_t PIN_RW = 5;
const uint8_t PIN_E  = 6;

// pin and port assignments for LCD parallel interface
PortGroup* const DATA_PORT = GPIOB;
const uint8_t PIN_DB0 = 0;	// DB pins are assumed to be consecutive

// pin and port assignments for transceiver control signals
PortGroup* const TRAN_PORT = GPIOA;
#endif

#endif /* CONFIG_H_ */