/**
 * Project Untitled
 */

#ifndef _HAL_GPIODEFS_H
#define _HAL_GPIODEFS_H

namespace HAL
{
    class GpioDefs
    {
    public:
        enum MODE
        {
        	GPIO_OUTPUT_PUSHPULL, GPIO_OUTPUT_OPENDRAIN, GPIO_INPUT
        };

        enum PULL
        {
        	GPIO_FLOAT, GPIO_PULLDOWN, GPIO_PULLUP
        };

        enum PORT
        {
        	GPIO_PORTA, GPIO_PORTB, GPIO_PORTC, GPIO_PORTD, GPIO_PORTE, GPIO_PORTF
        };
    };
}
;

#endif //_HAL_GPIODEFS_H
