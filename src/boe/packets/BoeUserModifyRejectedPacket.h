/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_MODIFY_REJECTED_PACKET_H
#define BOE_MODIFY_REJECTED_PACKET_H

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

PACKED(class BoeModifyRejectedPacket
{
    public:
		uint64_t				mTransactionTime;
        char                    mClOrdID[20];
        char                    mModifyRejectReason[1];
        char                    mModifyRejectText[60];
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
		
		string getModifyRejectReason()
	    {
	        return getString(mModifyRejectReason, sizeof(mModifyRejectReason));
    	}
        
        string getModifyRejectText()
	    {
	        return getString(mModifyRejectText, sizeof(mModifyRejectText));
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
		
		bool setModifyRejectReason(char* buf)
        {
            return setString(mModifyRejectReason, (unsigned char*)buf, 1);
        }
        
        bool setModifyRejectText(char* buf)
        {
            return setString(mModifyRejectText, (unsigned char*)buf, 60);
        }
        
        bool setNumberOfReturnBitfields(uint8_t buf)
        {
            mNumberOfReturnBitfields = buf;
            return true;
        }
});


} // namespace neueda

#endif // BOE_Logout_RESPONSE_PACKET_H
