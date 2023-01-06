/**
 * Project Untitled
 */

#ifndef _HAL_GPIO_H
#define _HAL_GPIO_H

#include <Drivers/Peripheral/Gpio.h>
#include <Hal/Peripheral/GpioDefs.h>

namespace HAL
{
    class Gpio : public GpioDefs
    {
    public:

        typedef DRIVERS::Gpio GpioDriver;

        Gpio(PORT port, uint8 pin, MODE mode = GPIO_OUTPUT_PUSHPULL, PULL pulldown = GPIO_FLOAT,
                bool state = false) : _gpio(port, pin)
        {
            set(state);
            config(mode, pulldown);
        }

        /**
         * @brief Set the state the pin output to the opposite state
         *
         * @details It still will change the output register when configured as input
         *
         * @param state the state it should set the pin
         */
        void toogle()
        {
            set(!get());
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
                PULL pulldown = GPIO_FLOAT);


    private:
        GpioDriver _gpio;
    };
}
;

#endif //_HAL_GPIO_H
