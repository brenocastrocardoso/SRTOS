/*
 * DataTime.h
 *
 *  Created on: 21 de jul de 2020
 *      Author: breno
 */

#ifndef _DATATIME_H_
#define _DATATIME_H_

#include <RTOS.h>
#include <Kernel/Kernel.h>

class DateTime_Commun
{
public:
    enum WEEKDAY : uint8 {
      MONDAY,
      TUESDAY,
      WEDNESDAY,
      THURSDAY,
      FRIDAY,
      SATURDAY,
      SUNDAY,
    };

    enum MONTH : uint8 {
        JANUARY,
        FEBRUARY,
        MARCH,
        APRIL,
        MAY,
        JUNE,
        JULY,
        AUGUST,
        SEPTEMBER,
        OCTOBER,
        NOVERMBER,
        DECEMBER,
    };

    static constexpr uint32 secondsInAMinute = 60;
    static constexpr uint32 minutesInAHour = 60;
    static constexpr uint32 hourInADay = 24;
    static constexpr uint32 daysInAWeek = 7;

    static constexpr uint32 daysInANormalYear = 365;
    static constexpr uint32 daysInALeapYear = 366;

    static constexpr uint32 monthsInAYear = 12;

    static constexpr uint32 secondsInADay = hourInADay*minutesInAHour*secondsInAMinute;
    static constexpr uint32 secondsInAHour = minutesInAHour*secondsInAMinute;

    static const char* MONTH_NAMES[];
    static const char* WEEKDAY_NAMES[];

    typedef uint32 Timestamp;

    Timestamp secondsInAYear(uint16 year)
    {
        return secondsInADay*daysInAYear(year);
    }

    Timestamp secondsInAMonth(MONTH month, uint16 year)
    {
        return daysInAMonth(month, year)*secondsInADay;
    }

    uint16 daysInAYear(uint16 year)
    {
        uint16 retval = daysInANormalYear;
        if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
                retval = daysInALeapYear;
        return retval;

    }

    uint8 daysInAMonth (MONTH month, uint16 year)
    {
        const uint8 daysInAMonth[] = {
                31,           // JANUARY,
                28,           // FEBRUARY,
                31,           // MARCH,
                30,           // ABRIL,
                31,           // MAY,
                30,           // JUNE,
                31,           // JULY,
                31,           // AUGUST,
                30,           // SEPTEMBER,
                31,           // OCTOBER,
                30,           // NOVERMBER,
                31            // DEZEMBER,
        };

        uint8 retval = daysInAMonth[month];

        if (month == FEBRUARY)
        {
            if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
                retval = 29;
        }

        return retval;
    }
};


class DateTime : DateTime_Commun
{
    static constexpr WEEKDAY firstDayWeekday = THURSDAY;
    static constexpr uint16 fristYear = 1970;

    uint8 second;
    uint8 minute;
    uint8 hour;
    uint8 day;
    MONTH month;
    uint16 year;
    WEEKDAY weekday;
public:
    DateTime(Timestamp timestamp)
    {

        Timestamp remainingTime = 0;

        remainingTime = timestamp;

        year = fristYear;
        Timestamp secondsInTheYear = secondsInAYear(year);
        while(remainingTime > secondsInTheYear)
        {
            remainingTime -= secondsInTheYear;
            ++year;
            secondsInTheYear = secondsInAYear(year);
        }

        for(month = JANUARY; month <= DECEMBER; month = (MONTH)(((uint8)month)+1))
        {
            Timestamp monthDaysTimestamp = daysInAMonth(month, year)*secondsInADay;
            if (remainingTime <= monthDaysTimestamp)
            {
                break;
            }
            remainingTime -= monthDaysTimestamp;
        }

        //there is no day 0
        day = remainingTime / secondsInADay +1;
        remainingTime = remainingTime % secondsInADay;

        hour = remainingTime / secondsInAHour;
        remainingTime = remainingTime % secondsInAHour;

        minute = remainingTime / secondsInAMinute;
        remainingTime = remainingTime % secondsInAMinute;

        second = remainingTime;

        weekday = dayOfWeek(timestamp);


    }

    Timestamp getTimestamp()
    {
        Timestamp accumulatedTimestamp = 0;

        for(uint16 indexYear = fristYear; indexYear < year; indexYear++)
        {
            accumulatedTimestamp += secondsInAYear(year);
        }

        for(MONTH indexMonth = JANUARY; indexMonth < month; indexMonth = (MONTH)(((uint8)indexMonth)+1))
        {
            accumulatedTimestamp += daysInAMonth(month, year)*secondsInADay;
        }

        accumulatedTimestamp += secondsInADay*(day-1);
        accumulatedTimestamp +=  secondsInAHour*hour;
        accumulatedTimestamp +=  secondsInAMinute*minute;
        accumulatedTimestamp +=  second;

        return accumulatedTimestamp;
    }

    WEEKDAY dayOfWeek(Timestamp timestamp)
    {
        uint32 days = timestamp / secondsInADay;
        return (WEEKDAY)(uint8)(((days % daysInAWeek) + (uint8)firstDayWeekday)%daysInAWeek);
    }


    friend RTOS::Kernel::KernelOstream& operator<<(RTOS::Kernel::KernelOstream& os, const DateTime& dt);
};

class Day
{
public:
    virtual void addDay(void) = 0;
    virtual void subDay(void) = 0;

    Day& operator++(void){addDay(); return *this;}
    Day& operator--(void){subDay(); return *this;}
    virtual ~Day(){}
};

class Hour
{
public:
    virtual void addHour(void) = 0;
    virtual void subHour(void) = 0;

    Hour& operator++(void){addHour(); return *this;}
    Hour& operator--(void){subHour(); return *this;}
    virtual ~Hour(){}
};

class Minute
{
public:
    virtual void addMinute(void) = 0;
    virtual void subMinute(void) = 0;

    Minute& operator++(void){addMinute(); return *this;}
    Minute& operator--(void){subMinute(); return *this;}
    virtual ~Minute(){}
};


class Timepoint : public DateTime_Commun, public Day, public Hour, public Minute
{
    typedef uint32 TimestampType;
private:
    TimestampType _timestamp;

public:
    Timepoint(DateTime time)
    {
        _timestamp = time.getTimestamp();
    }

    Timepoint(TimestampType time)
	{
		_timestamp = time;
	}

    operator uint32()
	{
    	return _timestamp;
	}

    Minute& Minute(void){return *this;}
    Hour& Hour(void){return *this;}
    Day& Day(void){return *this;}

    DateTime dateTime(void){ return _timestamp;}

    virtual void addDay(void){ _timestamp += secondsInADay;}
    virtual void subDay(void){ _timestamp -= secondsInADay;}

    virtual void addHour(void){ _timestamp += secondsInAHour;}
    virtual void subHour(void){ _timestamp -= secondsInAHour;}

    virtual void addMinute(void){ _timestamp += secondsInAMinute; }
    virtual void subMinute(void){ _timestamp -= secondsInAMinute; }

    Timepoint& operator++(void){++_timestamp; return *this;}
    Timepoint& operator--(void){--_timestamp; return *this;}

    virtual ~Timepoint(){}

};


#endif /* RTOS_PUBLIC_UTILS_RTC_DATATIME_H_ */
