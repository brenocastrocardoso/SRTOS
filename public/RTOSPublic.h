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
	Kernel::KernelEEprom& UFat = *(Kernel::UFat);
	Kernel::KernelOstream& cout = Kernel::cout;

	const STREAM::SPECIAL_CHARACTHERES& endl = STREAM::endl;
	const STREAM::SPECIAL_CHARACTHERES& tab = STREAM::tab;
	const STREAM::SPECIAL_CHARACTHERES& newline = STREAM::newline;
	const STREAM::SPECIAL_CHARACTHERES& hex = STREAM::hex;
};

#endif /* RTOS_PUBLIC_H_ */
