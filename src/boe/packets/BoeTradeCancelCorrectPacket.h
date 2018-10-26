/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_TRADE_CANCEL_CORRECT_PACKET_H
#define BOE_TRADE_CANCEL_CORRECT_PACKET_H

#include <string>
#include <vector>
#include <sstream>
#include <cstddef>
#include <stdint.h>
#include <stdexcept>
#include "BoePacketUtils.h"

namespace neueda
{

PACKED(class BoeTradeCancelCorrectPacket
{
    public:
        uint64_t     	        mTransactTime;
        char                    mClOrdID[20];
        uint64_t                mOrderID;
        uint64_t                mExecRefID;
        char                    mSide[1];
        char                    mBaseLiquidityIndicator[1];
        char                    mClearingFirm[4];
        char                    mClearingAccount[4];
        uint32_t	            mLastShares;
        uint64_t                mLastPx;
        uint64_t                mCorrectedPrice;
        char     	            mOrigTime[8];
        char                    mReservedInternal[1];
        uint8_t                 mNumberOfBitfields;
        

        BoeTradeCancelCorrectPacket ()
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

        uint64_t getExecRefID()
        {
            return mExecRefID;
        }
        
        string getSide()
        {
            return getString(mSide, sizeof(mSide));
        }
        
        string getBaseLiquidityIndicator()
        {
            return getString(mBaseLiquidityIndicator, sizeof(mBaseLiquidityIndicator));
        }
        
        string getClearingFirm()
        {
            return getString(mClearingFirm, sizeof(mClearingFirm));
        }
        
        string getClearingAccount()
        {
            return getString(mClearingAccount, sizeof(mClearingAccount));
        }

        uint32_t getLastShares ()
        {
            return mLastShares;
        }

        uint64_t getLastPx ()
        {
            return mLastPx;
        }
        
        uint64_t getCorrectedPrice ()
        {
            return mCorrectedPrice;
        }

        string getOrigTime()
        {
            return getString(mOrigTime, sizeof(mOrigTime));
        }

        uint16_t getNumberOfBitfields ()
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
            return mOrderID;
        }

        bool setExecRefID(uint64_t val)
        {
            return mExecRefID;
        }
        
        bool setSide(char* buf)
        {
            return setString(mSide, (unsigned char*)buf, 1);
        }
        
        bool setBaseLiquidityIndicator(char* buf)
        {
            return setString(mBaseLiquidityIndicator, (unsigned char*)buf, 1);
        }
        
        bool setClearingFirm(char* buf)
        {
            return setString(mClearingFirm, (unsigned char*)buf, 4);
        }
        
        bool setClearingAccount(char* buf)
        {
            return setString(mClearingAccount, (unsigned char*)buf, 4);
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
        
        bool setCorrectedPrice(uint64_t val)
        {
            mCorrectedPrice = val;
            return true;
        }

        bool setOrigTime(char* buf)
        {
            return setString(mOrigTime, (unsigned char*)buf, 8);
        }

        bool setNumberOfBitfields(uint8_t val)
        {
            mNumberOfBitfields = val;
            return true;
        }

});


} // namespace neueda

#endif // BOE_ORDER_EXECUTION_PACKET_H
