/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_ORDER_ACKNOWLEDGEMENT_PACKET_H
#define BOE_ORDER_ACKNOWLEDGEMENT_PACKET_H

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

PACKED(class BoeOrderAcknowledgementPacket
{
    public:
		uint64_t			    mTransactTime;
        char                    mClOrdID[20];
        uint64_t                mOrderID;
        char                    mReservedInternal[1];
        uint8_t	                mNumberOfBitfields;
        

        BoeOrderAcknowledgementPacket ()
        {
        }

        uint64_t getTransactTime()
        {
            return mTransactTime;
        }

	    string getClOrdID()
	    {
	        return getString(mClOrdID, sizeof(mClOrdID));
    	}
		
		uint64_t getOrderID()
	    {
	        return mOrderID;
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
        
        bool setNumberOfBitfields(uint8_t val)
        {
            mNumberOfBitfields = val;
            return true;
        }
});


} // namespace neueda

#endif // BOE_ORDER_ACKNOWLEDGEMENT_PACKET_H
