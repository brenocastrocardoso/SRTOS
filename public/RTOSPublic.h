/*
 * RTOSPublic.h
 *
 *  Created on: 25 de ago de 2020
 *      Author: breno
 */

#ifndef RTOS_PUBLIC_H_
#define RTOS_PUBLIC_H_

#include <IO/OStream.h>
#include <Kernel/Kernel.h>


namespace RTOS
{
	extern Kernel::KernelEEprom& UFat;
	extern Kernel::KernelOstream& cout;

	extern const STREAM::SPECIAL_CHARACTHERES& endl;
	extern const STREAM::SPECIAL_CHARACTHERES& tab;
	extern const STREAM::SPECIAL_CHARACTHERES& newline;
	extern const STREAM::SPECIAL_CHARACTHERES& hex;
};

#endif /* RTOS_PUBLIC_H_ */
