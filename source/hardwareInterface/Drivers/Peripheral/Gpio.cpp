/**
 * Project Untitled
 */

#ifndef _DRIVER_GPIO_H
#define _DRIVER_GPIO_H

#include <RTOSCodeStyle.h>

#include <Hal/Peripheral/GpioDefs.h>
#include "Gpio.h"
#include <Arduino.h>

using namespace DRIVERS;

/**
 * @brief Set the state the pin should output, when the pin is configured as input do nothing
 *
 * @details It still will change the output register when configured as input
 *
 * @implement should be implemented in the drivers section
 *
 * @param state the state it should set the pin
 */
void Gpio::set(bool state) {
	digitalWrite(_pin, state);
}

/**
 * @brief Get the state of the pin, when configured to output it will return the current output state, when input will read the pin
 *
 * @implement should be implemented in the drivers section
 *
 * @return the state the pin is current configured
 */
bool Gpio::get() {
	return digitalRead(_pin);
}

/**
 * @brief configure the pin
 *
 * @details Configure if the pin mode and pull
 *
 * @implement should be implemented in the drivers section
 *
 * @param state the state it should set the pin
 */
void Gpio::config(MODE config, PULL pull) {
	switch (config) {
	case GPIO_INPUT:
		switch (pull) {
		case GPIO_FLOAT:
			pinMode(_pin, INPUT);
			break;
		case GPIO_PULLDOWN:
			break;
		case GPIO_PULLUP:
			pinMode(_pin, INPUT_PULLUP);
			break;
		default:
			break;
		}
		break;
	case GPIO_OUTPUT_PUSHPULL:
		pinMode(_pin, OUTPUT);
		break;
	default:
		break;
	}
}

#endif //_DRIVER_GPIO_H
