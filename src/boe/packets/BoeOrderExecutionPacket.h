/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_ORDER_EXECUTION_PACKET_H
#define BOE_ORDER_EXECUTION_PACKET_H

#include <string>
#include <vector>
#include <sstream>
#include <cstddef>
#include <stdint.h>
#include <stdexcept>
#include "BoePacketUtils.h"

namespace neueda
{

PACKED(class BoeOrderExecutionPacket
{
    public:
        uint64_t     	        mTransactionTime;
        char                    mClOrdID[20];
        uint64_t                mExecID;
        uint32_t	            mLastShares;
        uint64_t                mLastPx;
        uint32_t                mLeavesQty;
        char                    mBaseLiquidityIndicator[1];
        char                    mSubLiquidityIndicator[1];
        char                    mContraBroker[4];
        char                    mReservedInternal[1];
        uint8_t                 mNumberOfBitfields;


        BoeOrderExecutionPacket ()
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

        uint64_t getExecID()
        {
            return mExecID;
        }

        uint32_t getLastShares ()
        {
            return mLastShares;
        }

        uint64_t getLastPx ()
        {
            return mLastPx;
        }

	    uint32_t getLeavesQty()
	    {
	        return mLeavesQty;
    	}

        string getBaseLiquidityIndicator()
        {
            return getString(mBaseLiquidityIndicator, sizeof(mBaseLiquidityIndicator));
        }

        string getSubLiquidityIndicator()
        {
            return getString(mSubLiquidityIndicator, sizeof(mSubLiquidityIndicator));
        }

        string getContraBroker()
        {
            return getString(mContraBroker, sizeof(mContraBroker));
        }

        uint16_t getNumberOfBitfields ()
        {
            return mNumberOfBitfields;
        }

        bool setTransactionTime(uint64_t val)
        {
            mTransactionTime = val;
            return true;
        }

        bool setClOrdID(std::string val)
        {
            return setString(mClOrdID, val, 20);
        }

        bool setExecID(uint64_t val)
        {
            mExecID = val;
            return true; 
        }

        bool setLastShares(uint32_t val)
        {
            mLastShares = val;
            return true;
        }

        bool setLastPx(uint64_t val)
        {
            mLastPx = val;
            return true;
        }

        bool setLeavesQty(uint32_t val)
        {
            mLeavesQty = val;
            return true;
        }

        bool setBaseLiquidityIndicator(std::string val)
        {
            return setString(mBaseLiquidityIndicator, val, 1);
        }

        bool setSubLiquidityIndicator(std::string val)
        {
            return setString(mSubLiquidityIndicator, val, 1);
        }

        bool setContraBroker(std::string val)
        {
            return setString(mContraBroker, val, 4);
        }

        bool setNumberOfBitfields(uint8_t val)
        {
            mNumberOfBitfields = val;
            return true;
        }

});


} // namespace neueda

#endif // BOE_ORDER_EXECUTION_PACKET_H
