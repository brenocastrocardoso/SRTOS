/*
 * Kernel.h
 *
 *  Created on: 14 de jul de 2020
 *      Author: breno
 */

#ifndef _KERNEL_H
#define _KERNEL_H

#include <Hal/Peripheral/Eeprom.h>
#include <Hal/Peripheral/Serial.h>
#include <IO/IOStream.h>
#include <IO/StdOIStream.h>
#include <Kernel/Scheduller/Scheduller.h>
#include <Kernel/Thread.h>
#include <MemoryManagement/uFAT.h>

namespace RTOS
{
	class Kernel
	{
	public:

		struct KernelInformationStruct
		{
			uint8 major;
			uint8 minor;
			uint8 patch;
		};

		static constexpr KernelInformationStruct KernelInformation=
		{
			.major = 0,
			.minor = 1,
			.patch = 0,
		};

		static void startUp();

		Kernel();

		void ApplicationStart();

		static void KernelSystickHandler(void);

		typedef RTOS::uFAT<HAL::EEprom> KernelEEprom;
	private:
		Scheduller _scheduller;
		Thread<400> _idleThread;
		Thread<5000> _appThread;

		static bool bootUpCompleted;

		HAL::Serial _stdSerial;
		RTOS::Ostream<HAL::Serial> _stdOstream;
		HAL::EEprom _stdEeprom;

		KernelEEprom _stdUFat;
public:
		static Kernel* KernelDebug;


	public:
		typedef RTOS::StdOstream<RTOS::Ostream<HAL::Serial>> KernelOstream;

		static KernelEEprom* UFat;
		static KernelOstream cout;
	};
}

#endif //_KERNEL_H
