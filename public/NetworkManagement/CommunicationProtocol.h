/*
 * CommunicationProtocol.h
 *
 * 	This module has the objective to create a high level protocol that can ran in any communication media and will guarantee the message deliver
 *
 *  Created on: 5 de out de 2019
 *      Author: breno
 */

#ifndef SYSTEM_COMMUNICATIONPROTOCOL_H_
#define SYSTEM_COMMUNICATIONPROTOCOL_H_

#include <Kernel/Traits.h>
#include <MemoryManagement/StaticCircularQueue.h>
#include <RTOS.h>
#include <Utils/Debug/Debug.h>
#include <Utils/TimeKeeper/Timer.h>
#include <WString.h>

class CommunicationProtocol
{
public:

	class CommunicationPackage	//TODO add packed
	{
		friend class CommunicationProtocol;
	public:

		typedef uint8 ComunicationProtocolAddress;

		struct __attribute__((packed)) CommunicationPackagePrefix
		{
			//node that is sending
			ComunicationProtocolAddress source;
			//node that is receiving
			ComunicationProtocolAddress destination;
			//message id to avoid repetition
			uint8 messageId 		: 2;
			//size of the payload
			uint8 payloadSize 	: 6;

		public:
			CommunicationPackagePrefix()
			{
				source = 0;
				destination = 0;
				messageId = 0;
				payloadSize = 0;
			}

			CommunicationPackagePrefix& operator=(const CommunicationPackagePrefix& identifier)
			{
				source = identifier.source;
				destination = identifier.destination;
				messageId = identifier.messageId;
				payloadSize = identifier.payloadSize;
				return *this;
			}

		};

		struct __attribute__((packed)) CommunicationPackageSufix
		{
			//bit to store if the sending node is looking for ack
			uint8 AckRequest		: 1;
			//bit to identify if this message is an ack
			uint8 AckMessage		: 1;
			// crc to avoid payload corruption
			uint8 _CRC			: 6;

		public:
			CommunicationPackageSufix()
			{
				AckRequest = 0;
				AckMessage = 0;
				_CRC = 0;
			}

			CommunicationPackageSufix& operator=(const CommunicationPackageSufix& sufix)
			{
				AckRequest = sufix.AckRequest;
				AckMessage = sufix.AckMessage;
				_CRC = sufix._CRC;
				return *this;
			}

		};

		static constexpr uint8 MAX_PAYLOAD_SIZE = 20;
		static ComunicationProtocolAddress _defaultSourceAddress;

		CommunicationPackagePrefix prefix;
		CommunicationPackageSufix sufix;	//By now changed the sufix to be before the payload so the getPayloadBuffer will have a valid information
		uint8 _payload[MAX_PAYLOAD_SIZE];


	public:
		CommunicationPackage(uint8 destination = 0, uint8 source = _defaultSourceAddress);

		/**
		 *  CAUTION WHEN USING THIS FUNCTION WE HAVE ONLY ONE DEFAULT ADDRESS FOR ALL COMMUNICATION PROTOCOLS
		 */
		static void setDefaultAddress(ComunicationProtocolAddress address){_defaultSourceAddress = address;}

		CommunicationPackage& operator=(const CommunicationPackage& package);

		void setMessageId(uint8 messageId)
		{
			prefix.messageId = messageId;
		}

		void setIdentifier(CommunicationPackagePrefix& identifier)
		{
			//make a deep copy from the identifier
			prefix = identifier;
		}

		bool addPayload(const uint8* buffer, uint8 size);

		uint8 getPayloadSize(void)  const {return prefix.payloadSize;}
		const uint8* getPayload(void)  const {return _payload;}
		ComunicationProtocolAddress getSource(void)  const {return prefix.source;}
		ComunicationProtocolAddress getDestination(void)  const {return prefix.destination;}


		bool isACK() const {return (sufix.AckMessage == 1) ? true : false;}
		bool isRequestingACK() const {return (sufix.AckRequest == 1) ? true : false;}
		void setACKRequest(bool value){sufix.AckRequest = (unsigned int)value;}

		uint8 calculateCRC(void) const
		{
			uint8 crc = 0;
			uint8* auxPointer = (uint8*)(void*)&prefix;
			CommunicationPackageSufix calcSufix;

			calcSufix = sufix;
			calcSufix._CRC = 0;


			for (uint8 index = 0;index<sizeof(CommunicationPackagePrefix);index++)
			{
				crc ^= auxPointer[index];
			}

			hdebug << F("CRC 1: ") << crc << DEBUG::endl;

			for (uint8 index = 0;index<prefix.payloadSize;index++)
			{
				crc ^= _payload[index];
			}

			hdebug << F("CRC 2: ") << crc << DEBUG::endl;
			auxPointer = (uint8*)(void*)&calcSufix;

			for (uint8 index = 0;index<sizeof(CommunicationPackageSufix);index++)
			{
				crc ^= auxPointer[index];
			}
			hdebug << F("CRC 3: ") << crc << DEBUG::endl;

			return crc;
		}

		void updateCRC(void)
		{
			sufix._CRC = calculateCRC();
		}

		bool checkCRC(void) const
		{
			CommunicationPackageSufix expectedSufix;
			expectedSufix._CRC = calculateCRC(); //lets use the structure so it will take care of the bit masking
			bool retval = false;
			if (expectedSufix._CRC == sufix._CRC)
			{
				retval = true;
			}
			return retval;
		}

		bool checkResend(const CommunicationPackage& package) const;

		bool checkEqualPrefix(const CommunicationPackage& package) const;
		bool checkACK(const CommunicationPackage& package) const
		{
			bool retval = true;
			if (package.prefix.source != prefix.destination)
			{
				retval = false;
			}
			if (package.prefix.destination != prefix.source)
			{
				retval = false;
			}
			if (package.prefix.messageId != prefix.messageId)
			{
				retval = false;
			}
			if (prefix.payloadSize != 0)
			{
				retval = false;
			}
			return retval;
		}

		template<typename STREAM>
		void publish (STREAM& stream) const
		{
			stream << "Communication Package:" << STREAM::newline;
			stream << STREAM::tab << "source        :" << (int)prefix.source << STREAM::newline;
			stream << STREAM::tab << "destination   :" << (int)prefix.destination << STREAM::newline;
			stream << STREAM::tab << "messageId     :" << (int)prefix.messageId << STREAM::newline;
			stream << STREAM::tab << "payloadSize   :" << (int)prefix.payloadSize << STREAM::newline;
			stream << STREAM::tab << "payload       :";
			for (uint8 index = 0;index<prefix.payloadSize;index++)
			{
				stream << int(_payload[index]) << " ";
			}
			stream << STREAM::newline;
			stream << STREAM::tab << "ACK Request   :" << (int)sufix.AckRequest << STREAM::newline;
			stream << STREAM::tab << "ACK Message   :" << (int)sufix.AckMessage << STREAM::newline;
			stream << STREAM::tab << "CRC           :" << (int)sufix._CRC << STREAM::newline;
		}

		uint8* getBinaryStream()  const
		{
			return (uint8*)(void*)this;
		}

		uint8 getBinaryStreamSize()  const
		{
			return prefix.payloadSize + sizeof(prefix) + sizeof(sufix);
		}

		void prepareACK(CommunicationPackage& package)  const
		{
			package = (*this);
			package.prefix.destination = prefix.source;
			package.prefix.source = prefix.destination;
			package.prefix.payloadSize = 0;
			package.sufix.AckMessage = 1;
			package.sufix.AckRequest = 0;
			package.updateCRC();
		}

		template<typename type>
		CommunicationPackage& operator<<(type& payload)
		{
			addPayload(payload.getBinaryStream(), payload.getBinaryStreamSize());
			return *this;
		}
	};

public:

	typedef void(*MESSAGE_HANDLER)(CommunicationProtocol::CommunicationPackage* package);

	struct MESSAGE_HANDLER_HOOK
	{
		uint8 address;
		MESSAGE_HANDLER messageHandler;
	};

	static const MESSAGE_HANDLER_HOOK MESSAGE_HANDLER_LIST[];

	bool send(CommunicationPackage& package);
	void sendHandler(void);
	void receive(uint8* buffer, uint8 size);

	CommunicationProtocol(CommunicationPackage::ComunicationProtocolAddress address = Traits<T_COMMUNICATION_PROTOCOL>::address) : _lastPackage(0xFF, 0XFF), _ACK(0xFF, 0XFF)
	{
		_messageId = 0;
		_WaitingToSendACK = false;
		_WaitingToReceiveACK = false;
		reTrysCounter = 0;

		CommunicationPackage::setDefaultAddress(address);
	}

private:
	void finishedMessageTransmission(void);
	void sendNextPackageonQueue(void);
	void checkForRetry(void);
	void sendAck(void);


private:
	uint8 _messageId;
	CommunicationPackage _lastPackage;
	CommunicationPackage _ACK;
	bool _WaitingToSendACK;
	bool _WaitingToReceiveACK;
	StaticCircularQueue<CommunicationPackage, Traits<T_COMMUNICATION_PROTOCOL>::queueSize> _queue;
	Timer _timer;
	uint8 reTrysCounter;
};



#endif /* SYSTEM_COMMUNICATIONPROTOCOL_H_ */
