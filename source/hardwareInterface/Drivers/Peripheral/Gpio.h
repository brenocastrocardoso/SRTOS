/**
 * Project Untitled
 */

#ifndef _DRIVER_GPIO_CPP
#define _DRIVER_GPIO_CPP

#include <RTOSCodeStyle.h>

#include <Hal/Peripheral/GpioDefs.h>

namespace DRIVERS
{
    class Gpio : public HAL::GpioDefs
    {
    public:

        Gpio(PORT port, uint8 pin)
        {
            _port = port;
            _pin = pin;
        }

        /**
         * @brief Set the state the pin should output, when the pin is configured as input do nothing
         *
         * @details It still will change the output register when configured as input
         *
         * @implement should be implemented in the drivers section
         *
         * @param state the state it should set the pin
         */
        void set(bool state);

        /**
         * @brief Get the state of the pin, when configured to output it will return the current output state, when input will read the pin
         *
         * @implement should be implemented in the drivers section
         *
         * @return the state the pin is current configured
         */
        bool get();

        /**
         * @brief configure the pin
         *
         * @details Configure if the pin mode and pull
         *
         * @implement should be implemented in the drivers section
         *
         * @param state the state it should set the pin
         */
        void config(MODE config = GPIO_INPUT,
                PULL pull= GPIO_FLOAT);

    private:
        PORT _port;
        uint8 _pin;
    };
}

#endif //_DRIVER_GPIO_H
