/*
 * CommunicationProtocol_PLink.h
 *
 *  Created on: 4 de dez de 2019
 *      Author: breno
 */

#ifndef SYSTEM_COMMUNICATIONPROTOCOL_PLINK_H_
#define SYSTEM_COMMUNICATIONPROTOCOL_PLINK_H_

#include <NetworkManagement/CommandProtocol.h>

//TODO MAKE IT POSSIBLE TO HAVE MORE THAN ONE LIST
#define COMMUNICATION_PROTOCOL_MESSAGE_HANDLER_LIST 												\
{	                                            													\
		{Traits<T_COMMUNICATION_PROTOCOL>::address, CommandProtocol::receive},            						\
};


#endif /* SYSTEM_COMMUNICATIONPROTOCOL_PLINK_H_ */
