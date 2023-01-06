/**
 * Project Untitled
 */


#include <Hal/Archtecture/SchedullerHardwareInterface.h>
#include <Kernel/Scheduller/Scheduller.h>

/**
 * Scheduller implementation
 */

Scheduller* Scheduller::_schedullerRef = nullptr;

volatile HAL::SchedullerHardwareInterface::TCB * volatile pxCurrentTCB;

