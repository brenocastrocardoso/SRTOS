/*
 * AuteCodeStyle.h
 *
 *  Created on: 2 de dez de 2019
 *      Author: breno
 */

#ifndef RTOS_COMPATIBILITY_H_
#define RTOS_COMPATIBILITY_H_


#ifndef ARDUINO
	#define CSTRING_INCLUDE <cstring>
    #define CSTDIO_INCLUDE <cstdio>
    #define STRING_INCLUDE <String>
	#define STRING std::string
	#define STD_NAMESPACE std

	#define F(param)	param
#else
	#define CSTRING_INCLUDE <Arduino.h>
	#define CSTDIO_INCLUDE <Arduino.h>
    #define STRING_INCLUDE <Arduino.h>
	#define STRING String
	#define STD_NAMESPACE
#endif

#endif /* RTOS_UTILITY_AUTECODESTYLE_H_ */
