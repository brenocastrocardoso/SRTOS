/*
 * DateTime.cpp
 *
 *  Created on: 21 de jul de 2020
 *      Author: breno
 */


#include <Utils/RTC/DateTime.h>

const char* DateTime_Commun::WEEKDAY_NAMES[] =
{
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday",
        "Sunday",
};

const char* DateTime_Commun::MONTH_NAMES[] =
{
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December",
};

RTOS::Kernel::KernelOstream& operator<<(RTOS::Kernel::KernelOstream& os, const DateTime& date)
{
    RTOS::Kernel::cout
            << date.WEEKDAY_NAMES[date.weekday]
            << ", "
            << (int)(date.day)
            << "-"
            << date.MONTH_NAMES[date.month]
            << "-"
            << (int)date.year
            << " "
            << (int)date.hour
            << ":"
            << (int)date.minute
            << ":"
            << (int)date.second;
    return RTOS::Kernel::cout;
}
