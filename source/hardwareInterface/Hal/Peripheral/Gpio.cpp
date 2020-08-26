/*
 * Gpio.cpp
 *
 *  Created on: 16 de jul de 2020
 *      Author: breno
 */

#ifndef RTOS_SOURCE_DRIVERS_GPIO_CPP_
#define RTOS_SOURCE_DRIVERS_GPIO_CPP_

#include <IO/Gpio.h>
#include <Hal/Peripheral/Gpio.h>

using namespace HAL;

/**
 * @brief Set the state the pin should output, when the pin is configured as input do nothing
 *
 * @details It still will change the output register when configured as input
 *
 * @implement should be implemented in the drivers section
 *
 * @param state the state it should set the pin
 */
void Gpio::set(bool state)
{
    _gpio.set(state);
}

/**
 * @brief Get the state of the pin, when configured to output it will return the current output state, when input will read the pin
 *
 * @implement should be implemented in the drivers section
 *
 * @return the state the pin is current configured
 */
bool Gpio::get()
{
    return _gpio.get();
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
void Gpio::config(MODE config,
        PULL pulldown)
{
    _gpio.config(config, pulldown);
}




#endif /* RTOS_SOURCE_DRIVERS_GPIO_CPP_ */
