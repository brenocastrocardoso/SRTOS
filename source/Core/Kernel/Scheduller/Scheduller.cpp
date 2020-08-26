/**
 * Project Untitled
 */


#include <Kernel/Scheduller/Scheduller.h>
#include <Hal/Archtecture/SchedullerHardwareInterface.h>

/**
 * Scheduller implementation
 */

Scheduller* Scheduller::_schedullerRef = nullptr;

volatile HAL::SchedullerHardwareInterface::TCB * volatile pxCurrentTCB;

