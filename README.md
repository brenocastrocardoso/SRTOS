# SRTOS

For some time now I had been wanting to create an RTOS for learning purpose and I had some generic code that could be put to use so I decided to put the effort and finally make it. The main purpose is make it easier to read and easy to port.

Feel free to user, give feedback or help on the development.

## Status
  Version: 0.1.0  
  State: Under Development

## Design principles:
 - Configuration is good if it is an option

## Features Implemented
 - Scheduller : It is implemented an fixed priority Scheduller
 - Time support: There are Timer, Alarm, Chronometer, RTC and Epoch Date support
 - File System: There are a custom file system to help manage non volatile data, it is very primitive and based on FAT.

## Desired Features
  - EDF Scheduller policy
  - Port Scheduller to ARM based boards

## Notes
  - I'm currently developing that in an Arduino Uno and the driver implementations on that is direct based on Arduino functions
  - Most of the hardware context switch I got from FreeRTOS guide
