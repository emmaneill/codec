/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_LOGOUT_RESPONSE_PACKET_H
#define BOE_LOGOUT_RESPONSE_PACKET_H

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

PACKED(class BoeLogoutResponsePacket
{
    public:
        char                    mLogoutReasonStatus[1];
        char                    mLogoutReasonText[60];
        uint32_t                mLastReceivedSequenceNumber;
        uint8_t	                mNumberOfReturnBitfields;

        BoeLogoutResponsePacket ()
        {
        }

        string  getLogoutReason()
        {
            return getString(mLogoutReasonStatus, sizeof(mLogoutReasonStatus));
        }

        string  getLogoutReasonText()
        {
            return getString(mLogoutReasonText, sizeof(mLogoutReasonText)) ;
        }

        uint32_t getLastReceivedSequenceNumber()
        {
            return mLastReceivedSequenceNumber;
        }

	    uint8_t getNumberOfReturnBitfields()
	    {
	        return mNumberOfReturnBitfields;
    	}
        
        bool setLogoutReason(char* buf)
        {
            return setString(mLogoutReasonStatus, (unsigned char*)buf, sizeof(mLogoutReasonStatus));
        }

        bool setLogoutReasonText(char* buf)
        {
            return setString(mLogoutReasonText, (unsigned char*)buf, sizeof(mLogoutReasonText));
        }

        bool setLastReceivedSequenceNumber(uint32_t buf)
        {
            mLastReceivedSequenceNumber = buf;
            return true;
        }

        bool setNumberOfReturnBitfields(uint8_t buf)
        {
            mNumberOfReturnBitfields = buf;
            return true;
        }

});


} // namespace neueda

#endif // BOE_Logout_RESPONSE_PACKET_H
