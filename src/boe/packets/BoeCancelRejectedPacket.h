/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_CANCEL_REJECTED_PACKET_H
#define BOE_CANCEL_REJECTED_PACKET_H

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

PACKED(class BoeCancelRejectedPacket
{
    public:
		uint64_t			    mTransactionTime;
        char                    mClOrdID[20];
        char                    mCancelRejectReason[1];
        char                    mCancelRejectText[60];
        char                    mReservedInternal[1];
        uint8_t	                mNumberOfReturnBitfields;
        
        uint64_t getTransactionTime()
        {
            return mTransactionTime;
        }

	    string getClOrdID()
	    {
	        return getString(mClOrdID, sizeof(mClOrdID));
    	}
		
		string getCancelRejectReason()
	    {
	        return getString(mCancelRejectReason, sizeof(mCancelRejectReason));
    	}
        
        string getCancelRejectText()
	    {
	        return getString(mCancelRejectText, sizeof(mCancelRejectText));
    	}
        
        uint8_t getNumberOfReturnBitfields()
        {
            return mNumberOfReturnBitfields;
        } 

        bool setTransactionTime(uint64_t val)
        {
            mTransactionTime = val;
            return true;
        }

        bool setClOrdID(char* buf)
        {
            return setString(mClOrdID, (unsigned char*)buf, 20);
        }
		
		bool setCancelRejectReason(char* buf)
        {
            return setString(mCancelRejectReason, (unsigned char*)buf, 1);
        }
        
        bool setCancelRejectText(char* buf)
        {
            return setString(mCancelRejectText, (unsigned char*)buf, 60);
        }
        
        bool setNumberOfReturnBitfields(uint8_t buf)
        {
            mNumberOfReturnBitfields = buf;
            return true;
        }
});


} // namespace neueda

#endif // BOE_Logout_RESPONSE_PACKET_H