# SRTOS

For some time now I had been wanting to create an RTOS for learning purposes and I had some generic code that could be put to use so I decided to put the effort in and finally make it. The main purpose is to make it easy to read and easy to port.

Feel free to use, give feedback or help on the development.

## Status
  Version: 0.1.0  
  State: Under Development

## Features Implemented
 - Scheduler : It is implemented an fixed priority Scheduler
 - Time support: There are Timer, Alarm, Chronometer, RTC and Epoch Date support
 - File System: There are a custom file system to help manage non volatile data, it is very primitive and based on FAT.

## Desired Features
  - EDF Scheduler policy
  - Port Scheduler to ARM based boards
  - Good documentation
  - Communication protocols support
  - Profiling
  - Examples

## Notes
  - I'm currently developing that in an Arduino Uno and the driver implementations on that is direct based on Arduino functions
  - Most of the hardware context switch I got from FreeRTOS guide
