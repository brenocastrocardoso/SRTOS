/*
 * CommandProtocol.h
 *
 *  Created on: 5 de out de 2019
 *      Author: breno
 */

#ifndef SYSTEM_COMMANDPROTOCOL_H_
#define SYSTEM_COMMANDPROTOCOL_H_

//#include <cstring>

#include <NetworkManagement/CommunicationProtocol.h>
#include <RTOS.h>
#include <string.h>

class CommandProtocol
{
public:
	class Command
	{

		friend class CommandProtocol;
public:
		static constexpr uint8 MAX_PAYLOAD_SIZE = 10;

		typedef CommunicationProtocol::CommunicationPackage::ComunicationProtocolAddress Address;

		enum MESSAGE_TYPE
		{
			COMMAND = 0,
			FEEDBACK = 1
		};

		enum COMMAND_TYPE
		{
			OBJECT_COMMAND = 0,
			BOARD_COMMAND = 1,
		};

		struct __attribute__((packed)) CommandIdentifier
		{
			uint8 object_or_module	: 6;
			uint8 is_Feedback			: 1;
			uint8 isBoardComamnd 		: 1;
			uint8 command				: 7;
			uint8 reserved			: 1;

			CommandIdentifier(MESSAGE_TYPE messageType = COMMAND, COMMAND_TYPE commandType = BOARD_COMMAND, uint8 object=0, uint8 argCommand=0)
			{
				reserved = 1;
				is_Feedback = messageType;
				isBoardComamnd = commandType;
				object_or_module = object;
				command = argCommand;
			}

		};

		struct __attribute__((packed)) CommandPackage
		{
			CommandIdentifier identifier;
			uint8 payload[MAX_PAYLOAD_SIZE];

			CommandPackage& operator=(const CommandPackage& package)
			{
				memcpy(this, &package, sizeof(CommandPackage));
				return *this;
			}
		};
private:
		CommandPackage package;
		uint8 _payloadSize;
		Address source;			//we are going to save the source and destination of from the communication protocol
		Address destination;	//we are going to save the source and destination of from the communication protocol

	public:

		Command(uint8 object=0, uint8 argCommand=0, MESSAGE_TYPE messageType = COMMAND, COMMAND_TYPE commandType = OBJECT_COMMAND)
		{
			_payloadSize = 0;
			source = 0;
			destination = 0;
//			memset(&package, 0x00, sizeof(package));

			package.identifier.is_Feedback = messageType;
			package.identifier.isBoardComamnd = commandType;
			package.identifier.object_or_module = object;
			package.identifier.command = argCommand;
		}

		void setIdentifier(uint8 object=0, uint8 argCommand=0, MESSAGE_TYPE messageType = COMMAND, COMMAND_TYPE commandType = OBJECT_COMMAND)
		{
			package.identifier.is_Feedback = messageType;
			package.identifier.isBoardComamnd = commandType;
			package.identifier.object_or_module = object;
			package.identifier.command = argCommand;
		}

		void addPayload(uint8* buffer, uint8 size)
		{
			if (size <= MAX_PAYLOAD_SIZE)
			{
				_payloadSize = size;
				memcpy(package.payload, buffer, size);
			}
		}

		template<typename STREAM>
		void publish (STREAM& stream) const
		{
			stream << "Command Package:" 						<< STREAM::newline;
			stream << STREAM::tab << "is_Feedback        :" 	<< (int)package.identifier.is_Feedback << STREAM::newline;
			stream << STREAM::tab << "isBoardComamnd     :"		<< (int)package.identifier.isBoardComamnd << STREAM::newline;
			stream << STREAM::tab << "object_or_module   :" 	<< (int)package.identifier.object_or_module << STREAM::newline;
			stream << STREAM::tab << "command            :"		<< (int)package.identifier.command << STREAM::newline;
			stream << STREAM::tab << "payload            :";
			for (uint8 index = 0;index<_payloadSize;index++)
			{
				stream << (int)(package.payload[index]) << " ";
			}
			stream << STREAM::newline;

			stream << STREAM::tab << "sizeof Identifier  : " << (int)sizeof(package.identifier) << STREAM ::newline;
			stream << STREAM::tab << "size               : " << (int)_payloadSize << STREAM ::newline;
			stream << STREAM::tab << "source             : " << (int)source << STREAM ::newline;
			stream << STREAM::tab << "destination        : " << (int)destination << STREAM ::endl;
		}

		uint8* getPayload(void)
		{
			return (uint8*)(void*)&package.payload;
		}

		uint8 getPayloadSize(void)
		{
			return (uint8)_payloadSize;
		}

		uint8 getCommand(void)
		{
			return (uint8)package.identifier.command;
		}

		uint8 getObject(void)
		{
			return (uint8)package.identifier.object_or_module;
		}

		CommandPackage& getCommandPackage(void)
		{
			return package;
		}

		uint8* getBinaryStream()
		{
			return (uint8*)(void*)&package;
		}

		uint8 getBinaryStreamSize()
		{
			return sizeof(package.identifier) + _payloadSize;
		}

		bool send(Address destination)
		{
			setDestination(destination);
			return send();
		};

		bool send(CommunicationProtocol* communicationProtocol = _defaultCommunicationProtocol)
		{
			if (_defaultCommunicationProtocol != nullptr)
			{
				CommunicationProtocol::CommunicationPackage packageSend(destination);

				packageSend.addPayload((uint8*)(void*)getBinaryStream(), getBinaryStreamSize());
				//enqueue
				return _defaultCommunicationProtocol->send(packageSend);
			}
			else
			{
				return false;
			}
		}

		void setSource		(Address address)	{source = address;}
		void setDestination	(Address address)	{destination = address;}

	};

	typedef void(*COMMAND_HANDLER)(Command& command);

	struct COMMAND_HANDLER_HOOK
	{
		uint8* object_or_module;
		COMMAND_HANDLER commandHandler;
		COMMAND_HANDLER feedbackHandler;
	};

	static const COMMAND_HANDLER_HOOK OBJECT_COMMAND_HANDLER_LIST[];

	static const COMMAND_HANDLER_HOOK BOARD_COMMAND_HANDLER_LIST[];

	static void receive(CommunicationProtocol::CommunicationPackage* package);
	static void setDefaultCommunicationProtocol(CommunicationProtocol& protocol) {_defaultCommunicationProtocol = &protocol;}

	static CommunicationProtocol* _defaultCommunicationProtocol;

};


#endif /* SYSTEM_COMMANDPROTOCOL_H_ */
