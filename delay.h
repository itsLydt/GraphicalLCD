/*
 * delay.h
 *
 * Created: 1/24/2025 4:58:22 PM
 *  Author: lydia.klaus
 */ 


#ifndef DELAY_H_
#define DELAY_H_

#include "config.h"

/* default number of cpu cycles in one millisecond */
const uint32_t CYCLES_PER_MS = SYSTEM_CLOCK_FREQ / 1000;
#define CYCLES_PER_US  (CYCLES_PER_MS / 1000)
#define CYCLES_PER_NS (CYCLES_PER_US / 1000)

/* wait at least ms milliseconds, approximately */
void delay_ms(uint32_t ms){
	ms *= CYCLES_PER_MS / 6; //6 = appx number of instructions in the loop body below
	for(; ms > 0; --ms){
		__asm volatile("nop");
	}
}

/* wait at least us microseconds, approximately */
void delay_us(uint32_t us){
	us *= CYCLES_PER_US / 6; //6 = appx number of instructions in the loop body below
	for(; us > 0; --us){
		__asm volatile("nop");
	}
}

/* wait at least ns nanoseconds, approximately */
void delay_ns(uint32_t ns){
	ns *= CYCLES_PER_NS / 6; //6 = appx number of instructions in the loop body below
	for(; ns > 0; --ns){
		__asm volatile("nop");
	}
}


#endif /* DELAY_H_ */