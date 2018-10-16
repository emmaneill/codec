/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_LOGIN_RESPONSE_PACKET_H
#define BOE_LOGIN_RESPONSE_PACKET_H

#include <string>
#include <vector>
#include <sstream>
#include <cstddef>
#include <stdint.h>
#include <stdexcept>
#include "BoeHeaderPacket.h"
#include "BoePacketUtils.h"

namespace neueda
{

PACKED(class BoeLoginResponsePacket
{
    public:
	  
        char                    mLoginResponseStatus[1];
        char                    mLoginResponseText[60];
        uint8_t                 mNoUnspecifiedUnitReplay;
        uint32_t                mLastReceivedSequenceNumber;
        uint8_t	                mNumberOfUnits;


        BoeLoginResponsePacket ()
        {
/*            mStartOfMessage = "";
            mMessageLength  = 0;
            mMessageType    = "";
            mMatchingUnit   = 0;
            mSeqNum         = 0; */
        }

        string  getLoginResponseStatus()
        {
            return getString(mLoginResponseStatus, sizeof(mLoginResponseStatus));
        }

        string  getLoginResponseText()
        {
            return getString(mLoginResponseText, sizeof(mLoginResponseText)) ;
        }

        uint8_t getNoUnspecifiedUnitReplay()
        {
            return mNoUnspecifiedUnitReplay;
        }

        uint32_t getLastReceivedSequenceNumber()
        {
            return mLastReceivedSequenceNumber;
        }

	    uint8_t getNumberOfUnits()
	    {
	        return mNumberOfUnits;
    	}

        bool setLoginResponseStatus(char* val)
        {
            return setString(mLoginResponseStatus, (unsigned char*)val, sizeof(mLoginResponseStatus));
        }

        bool setLoginResponseText(char* val)
        {
        
            return setString(mLoginResponseText, (unsigned char*)val, sizeof(mLoginResponseText));
        }

        bool setNoUnspecifiedUnitReplay(uint8_t val)
        {
            mNoUnspecifiedUnitReplay = val;
            return true;
        }

        bool setLastReceivedSequenceNumber(uint32_t val)
        {
            mLastReceivedSequenceNumber = val;
            return true;
        }

        bool setNumberOfUnits(uint8_t val)
        {
            mNumberOfUnits = val;
            return true;
        }

});


} // namespace neueda

#endif // BOE_LOGIN_RESPONSE_PACKET_H
