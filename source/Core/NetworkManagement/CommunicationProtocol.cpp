/*
 * CommunicationProtocol.ino
 *
 *	@details this protocol comprises the lays 2 and 3 of communication, reliable data transfer and addressing information
 *
 *  Created on: 5 de out de 2019
 *      Author: breno
 */

#include <NetworkManagement/CommunicationProtocol.h>

#include <Kernel/Traits.h>
#include <MemoryManagement/StaticCircularQueue.h>
#include <NetworkManagement/CommunicationProtocol_PLink.h>
#include <RTOSCodeStyle.h>
#include <Utils/Debug/Debug.h>
#include <Utils/TimeKeeper/Timer.h>
//#include "nrf.h"

#define INVALID_ADDRESS 0XFF

const CommunicationProtocol::MESSAGE_HANDLER_HOOK CommunicationProtocol::MESSAGE_HANDLER_LIST[] = COMMUNICATION_PROTOCOL_MESSAGE_HANDLER_LIST;

CommunicationProtocol::CommunicationPackage::ComunicationProtocolAddress CommunicationProtocol::CommunicationPackage::_defaultSourceAddress = Traits<T_COMMUNICATION_PROTOCOL>::address;



CommunicationProtocol::CommunicationPackage::CommunicationPackage(uint8_t destination, uint8_t source)
{
	memset(_payload, 0x00, MAX_PAYLOAD_SIZE);
	prefix.source = source;
	prefix.destination = destination;
}

CommunicationProtocol::CommunicationPackage& CommunicationProtocol::CommunicationPackage::operator=(const CommunicationPackage& package)
{
	memcpy(this, &package, sizeof(CommunicationPackage));
	return *this;
}

bool CommunicationProtocol::CommunicationPackage::addPayload(const uint8_t* buffer, uint8_t size)
{
	bool retval = false;
	if (size <= MAX_PAYLOAD_SIZE)
	{
		memcpy(_payload, buffer, size);
		prefix.payloadSize = size;
		retval = true;
	}
	return retval;
}

bool CommunicationProtocol::CommunicationPackage::checkResend(const CommunicationPackage& package) const
{
	bool retval = false;
	if (memcmp(this, &package, getBinaryStreamSize()) != 0)
	{
		retval = true;
	}
	return retval;
}

bool CommunicationProtocol::CommunicationPackage::checkEqualPrefix(const CommunicationPackage& package) const
{
	bool retval = false;
	if (memcmp(this, &package.prefix, sizeof(package.prefix)) == 0)
	{
		retval = true;
	}
	return retval;
}


/*
 *  @brief: To receive we will parse and have handlers organized by the destination
 */
void CommunicationProtocol::receive(uint8_t* buffer, uint8_t size)
{
	trace << F("CommunicationProtocol::receive(uint8_t* buffer, uint8_t size)") << DEBUG::endl;

	CommunicationPackage* ptr;
	ptr = (CommunicationPackage*)(void*)buffer;

	hdebug << F("Communication Protocol Receive: ") << DEBUG::endl;
	ptr->publish(hdebug);

	//check CRC
	if (ptr->checkCRC() == true)
	{
		//check Resend
		if (ptr->checkResend(_lastPackage) == true)
		{
			if (ptr->isACK() == false)
			{
				//it is not ack message

				//message successful received
				_lastPackage = (*ptr);

				//call the handlers for the address
				bool ackEnqueued = false;
				for (uint8 index = 0; index < ARRAY_LENGHT(MESSAGE_HANDLER_LIST); ++index)
				{
					if (MESSAGE_HANDLER_LIST[index].address == ptr->getDestination())
					{
						MESSAGE_HANDLER_LIST[index].messageHandler(ptr);

						if (ptr->isRequestingACK() == true &&
							ackEnqueued == false)
						{
							//enqueue the ack if any of the handlers has the address, enqueue it only one time
							ackEnqueued = true;
							//if it request ack send it, even if the package is the same
							ptr->prepareACK(_ACK);
							_WaitingToSendACK = true;
						}
					}
				}
			}
			else
			{
				hdebug << F("ACK Received") << DEBUG::endl;
				if (ptr->checkACK(_queue.front()) == true)
				{
					//sucessful completed sending
					finishedMessageTransmission();
				}
				//if it is an ack message we will stop retry's and consider the message delivered
			}
		}
		else
		{
			hdebug << F("Resend package Identified") << DEBUG::endl;
			//it it is a resend we will still send the ack if the address is registered in this communicationProtocol
			if (ptr->isRequestingACK() == true)
			{
				//call the handlers for the address
				for (uint8 index = 0; index < ARRAY_LENGHT(MESSAGE_HANDLER_LIST); ++index)
				{
					if (MESSAGE_HANDLER_LIST[index].address == ptr->getDestination())
					{
						//if any the address is handled by any of the handlers prepare the ack
						ptr->prepareACK(_ACK);
						_WaitingToSendACK = true;
						break;
					}
				}
			}
		}
	}
	else
	{
		hdebug << F("CRC don't match") << DEBUG::endl;
	}

//	ptr->publish(debug);
//	CommandProtocol::receive(ptr->_payload, payloadSize);
}

bool CommunicationProtocol::send(CommunicationPackage& package)
{
	bool retval = false;

	package.setACKRequest(true);
	retval = _queue.push(package);

	hdebug << F("Communication Protocol Send: ") << DEBUG::endl;
	_queue.publish(debug);

	return retval;

}

void CommunicationProtocol::sendHandler()
{
//	trace << F("CommunicationProtocol::sendHandler()") << DEBUG::endl;
//	if (nrf::isReady() == true)
	if (false == true)
	{
		if (_WaitingToSendACK == true)
		{
			sendAck();
		}
		else if (_WaitingToReceiveACK == true)
		{
			checkForRetry();
		}
		else
		{
			if (_queue.empty() == false)
			{
				sendNextPackageonQueue();
			}
		}
	}
}

void CommunicationProtocol::sendAck(void)
{
	trace << F("CommunicationProtocol::sendAck()") << DEBUG::endl;
	//if there is one ack waiting to be send it will have priority
//	nrf::send(_ACK.getBinaryStream(), _ACK.getBinaryStreamSize());
	_WaitingToSendACK = false;
}

void CommunicationProtocol::checkForRetry(void)
{
	trace << F("CommunicationProtocol::checkForRetry()") << DEBUG::endl;
	if (_timer.getState() != Timer::TIMER_RUNNING)
	{
		if(reTrysCounter < Traits<T_COMMUNICATION_PROTOCOL>::AckResendTrys)
		{
			hdebug << F("reTrysCounter: ") << reTrysCounter << DEBUG::endl;
			reTrysCounter++;
			//if the timer is done resend the packeage
			CommunicationPackage& MessagePackage = _queue.front();
//			nrf::send(MessagePackage.getBinaryStream(), MessagePackage.getBinaryStreamSize());

			//restart the timer
			_timer.setTimer(Traits<T_COMMUNICATION_PROTOCOL>::AckWaitTime);
		}
		else
		{
			//failed to deliver the message
			finishedMessageTransmission();
		}
	}
}

void CommunicationProtocol::sendNextPackageonQueue(void)
{
	trace << F("CommunicationProtocol::sendNextPackageonQueue()") << DEBUG::endl;
	CommunicationPackage& newMessagePackage = _queue.front();
	newMessagePackage.publish(debug);
	newMessagePackage.setMessageId(_messageId);
	newMessagePackage.updateCRC();
	_messageId++;
//	nrf::send(newMessagePackage.getBinaryStream(), newMessagePackage.getBinaryStreamSize());

	if (newMessagePackage.isRequestingACK() == true)
	{
		_WaitingToReceiveACK = true;
		reTrysCounter = 0;
		//set the timer to the ack
		_timer.setTimer(Traits<T_COMMUNICATION_PROTOCOL>::AckWaitTime);
	}
	else
	{
		finishedMessageTransmission();
	}
}

void CommunicationProtocol::finishedMessageTransmission(void)
{
	_queue.pop();
	_WaitingToReceiveACK = false;
	reTrysCounter = 0;
}











