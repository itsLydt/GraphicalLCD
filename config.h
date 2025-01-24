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
static PortGroup* const CTRL_PORT = GPIOA;
static const uint8_t PIN_RS = 4;
static const uint8_t PIN_RW = 5;
static const uint8_t PIN_E  = 6;

// pin and port assignments for LCD parallel interface
static PortGroup* const DATA_PORT = GPIOB;
static const uint8_t PIN_DB0 = 0;	// DB pins are assumed to be consecutive

// pin and port assignments for transceiver control signals
static PortGroup* const TRAN_PORT = GPIOA;
// remember that these signals are inverted by the NPN circuit
#define PIN_DIR 9
#define PIN_ENABLE 9

#endif /* CONFIG_H_ */