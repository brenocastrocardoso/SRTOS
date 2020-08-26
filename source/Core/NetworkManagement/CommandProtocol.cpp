/*
 * CommandProtocol.c
 *
 *  Created on: 11 de out de 2019
 *      Author: breno
 */

#include <RTOSCodeStyle.h>
#include <NetworkManagement/CommandProtocol.h>

#include <NetworkManagement/CommandProtocol_PLink.h>
#include <NetworkManagement/CommunicationProtocol.h>
#include <Utils/Debug/Debug.h>

const CommandProtocol::COMMAND_HANDLER_HOOK CommandProtocol::OBJECT_COMMAND_HANDLER_LIST[] = OBJECT_COMMAND_PROTOCOL_HANDLER_LIST;

const CommandProtocol::COMMAND_HANDLER_HOOK CommandProtocol::BOARD_COMMAND_HANDLER_LIST[] = BOARD_COMMAND_PROTOCOL_HANDLER_LIST;

CommunicationProtocol* CommandProtocol::_defaultCommunicationProtocol = nullptr;


void CommandProtocol::receive(CommunicationProtocol::CommunicationPackage* package)
{
	hdebug << F("CommandProtocol::receive(CommunicationProtocol::CommunicationPackage* package)") << DEBUG::endl;

	uint8_t payloadSize = package->getPayloadSize() - sizeof(Command::CommandIdentifier);
	Command::CommandPackage* ptr;

	Command command;
	command._payloadSize = payloadSize;
	command.source = package->getSource();
	command.destination = package->getDestination();
	command.package = *((Command::CommandPackage*)(void*)package->getPayload());
	command.publish(hdebug);

	ptr = &(command.package);
	if (ptr->identifier.isBoardComamnd == false)
	{
		for (uint8_t index =0; index < ARRAY_LENGHT(OBJECT_COMMAND_HANDLER_LIST);index++)
		{
			if ((OBJECT_COMMAND_HANDLER_LIST[index].object_or_module) == nullptr ||
			    *(OBJECT_COMMAND_HANDLER_LIST[index].object_or_module) == ptr->identifier.object_or_module)
			{
				if (ptr->identifier.is_Feedback == false)
				{
					if (OBJECT_COMMAND_HANDLER_LIST[index].commandHandler != nullptr)
					{
						OBJECT_COMMAND_HANDLER_LIST[index].commandHandler(command);
					}
				}
				else
				{
					if (OBJECT_COMMAND_HANDLER_LIST[index].feedbackHandler != nullptr)
					{
						OBJECT_COMMAND_HANDLER_LIST[index].feedbackHandler(command);
					}
				}
			}
		}
	}
	else
	{
		for (uint8_t index =0; index < ARRAY_LENGHT(BOARD_COMMAND_HANDLER_LIST);index++)
		{
			if (*(BOARD_COMMAND_HANDLER_LIST[index].object_or_module) == ptr->identifier.object_or_module)
			{
				if (ptr->identifier.is_Feedback == false)
				{
					if (BOARD_COMMAND_HANDLER_LIST[index].commandHandler != nullptr)
					{
						BOARD_COMMAND_HANDLER_LIST[index].commandHandler(command);
					}
				}
				else
				{
					if (BOARD_COMMAND_HANDLER_LIST[index].feedbackHandler != nullptr)
					{
						BOARD_COMMAND_HANDLER_LIST[index].feedbackHandler(command);
					}
				}
			}
		}
	}



}
