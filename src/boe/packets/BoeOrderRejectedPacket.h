/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_ORDER_REJECTED_PACKET_H
#define BOE_ORDER_REJECTED_PACKET_H

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

PACKED(class BoeOrderRejectedPacket
{
    public:
		uint64_t			    mTransactionTime;
        char                    mClOrdID[20];
        char                    mOrderRejectReason[1];
        char                    mOrderRejectText[60];
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
		
		string getOrderRejectReason()
	    {
	        return getString(mOrderRejectReason, sizeof(mOrderRejectReason));
    	}
        
        string getOrderRejectText()
	    {
	        return getString(mOrderRejectText, sizeof(mOrderRejectText));
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
		
		bool setOrderRejectReason(char* buf)
        {
            return setString(mOrderRejectReason, (unsigned char*)buf, 1);
        }
        
        bool setOrderRejectText(char* buf)
        {
            return setString(mOrderRejectText, (unsigned char*)buf, 60);
        }
        
        bool setNumberOfReturnBitfields(uint8_t buf)
        {
            mNumberOfReturnBitfields = buf;
            return true;
        }
});


} // namespace neueda

#endif // BOE_Logout_RESPONSE_PACKET_H