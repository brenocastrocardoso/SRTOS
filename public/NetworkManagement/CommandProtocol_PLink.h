/*
 * CommandProtocol_PLink.h
 *
 *  Created on: 8 de dez de 2019
 *      Author: breno
 */

#ifndef SYSTEM_COMMANDPROTOCOL_PLINK_H_
#define SYSTEM_COMMANDPROTOCOL_PLINK_H_

//TODO: THE FIRST LINE
//TODO MAKE IT POSSIBLE TO HAVE MORE THAN ONE LIST
#define OBJECT_COMMAND_PROTOCOL_HANDLER_LIST 												\
{                                                                                  			\
		{nullptr, nullptr, nullptr},														\
		/*{nullptr, CENTRAL::objectCommandHandler, CENTRAL::objectFeedbackHandler},*/ \
};


#define BOARD_COMMAND_PROTOCOL_HANDLER_LIST 										\
{                                                                                   \
		{nullptr, nullptr, nullptr},												\
		/*{&ObjectConfiguration::CommandProtocolInfo::MODULE_ID, ObjectConfiguration::boardCommandHandler, ObjectConfiguration::boardFeedbackHandler},*/\
};

#endif /* SYSTEM_COMMANDPROTOCOL_PLINK_H_ */
