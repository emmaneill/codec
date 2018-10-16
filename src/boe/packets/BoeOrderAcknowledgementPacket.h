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
		uint64_t			    mTransactionTime;
        char                    mClOrdID[20];
        uint64_t                mOrderID;
        char                    mReservedInternal[1];
        uint8_t	                mNumberOfReturnBitfields;
        

        BoeOrderAcknowledgementPacket ()
        {
        }

        uint64_t getTransactionTime()
        {
            return mTransactionTime;
        }

	    string getClOrdID()
	    {
	        return getString(mClOrdID, sizeof(mClOrdID));
    	}
		
		uint64_t getOrderID()
	    {
	        return mOrderID;
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
		
		bool setOrderID(uint64_t val)
        {
            mOrderID = val;
            return true;
        }
        
        bool setNumberOfReturnBitfields(uint8_t val)
        {
            mNumberOfReturnBitfields = val;
            return true;
        }
});


} // namespace neueda

#endif // BOE_ORDER_ACKNOWLEDGEMENT_PACKET_H
