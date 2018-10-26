/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_ORDER_RESTATED_PACKET_H
#define BOE_ORDER_RESTATED_PACKET_H

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

PACKED(class BoeOrderRestatedPacket
{
    public:
		uint64_t			    mTransactTime;
        char                    mClOrdID[20];
        uint64_t                mOrderID;
        char                    mRestatementReason[1];
        char                    mReservedInternal[1];
        uint8_t	                mNumberOfBitfields;
        
        uint64_t getTransactTime()
        {
            return  mTransactTime;
        }

	    string getClOrdID()
	    {
	        return getString(mClOrdID, sizeof(mClOrdID));
    	}
		
		uint64_t getOrderID()
	    {
	        return mOrderID;
    	}
        
        string getRestatementReason()
	    {
	        return getString(mRestatementReason, sizeof(mRestatementReason));
    	}
        
        uint8_t getNumberOfBitfields()
        {
            return mNumberOfBitfields;
        } 

        bool setTransactTime(uint64_t val)
        {
            mTransactTime = val;
            return true;
        }

        bool setClOrdID(char* buf)
        {
            return setString(mClOrdID, (unsigned char*)buf, 20);
        }
		
		bool setOrderID(uint64_t val)
        {
            mOrderID = val;
            return true;
        }
        
        bool setRestatementReason(char* buf)
        {
            return setString(mRestatementReason, (unsigned char*)buf, 1);
        }
        
        bool setNumberOfBitfields(uint8_t buf)
        {
            mNumberOfBitfields = buf;
            return true;
        }
});


} // namespace neueda

#endif // BOE_Logout_RESPONSE_PACKET_H