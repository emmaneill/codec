/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 13:42:16 22/11/18
 */
#ifndef _BOE_NEWORDER_PACKET_H_
#define _BOE_NEWORDER_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>

#include "BoePacketUtils.h"
#include "BoeHeaderPacket.h"


namespace neueda
{

PACKED(class boeNewOrderPacket
{
public:
        char mClOrdID[20];
        uint8_t mSide;
        uint32_t mOrderQty;
        uint8_t mNumberOfBitfields;
        char mClearingFirm[4];
        char mClearingAccount[4];
        uint64_t mPrice;
        char mExecInst[1];
        char mOrdType[1];
        char mTimeInForce[1];
        uint32_t mMinQty;
        uint32_t mMaxFloor;
        char mSymbol[8];
        uint8_t mSymbolSfx;
        char mCurrency[3];
        char mIDSource[1];
        char mSecurityID[16];
        char mSecurityExchange[4];
        char mCapacity[1];
        char mRoutingInst[4];
        char mAccount[16];
        char mDisplayIndicator[1];
        uint64_t mPegDifference;
        char mPreventParticipantMatch[3];
        uint64_t mExpireTime;
        char mLiquidityProvision[1];
        uint32_t mClientID;
        uint32_t mInvestorID;
        uint32_t mExecutorID;
        char mOrderOrigination[1];
        char mRoutStrategy[4];
        char mAlgorithmicIndicator[1];
        uint8_t mClientQualifiedRole;
        uint8_t mInvestorQualifiedRole;
        uint8_t mExecutorQualifiedRole;

    boeNewOrderPacket ()
    {
        memset (mClOrdID, 0, 20);
        mSide = 0;
        mOrderQty = 0;
        mNumberOfBitfields = 0;
        memset (mClearingFirm, 0, 4);
        memset (mClearingAccount, 0, 4);
        mPrice = UINT64_MAX;
        memset (mExecInst, 0, 1);
        memset (mOrdType, 0, 1);
        memset (mTimeInForce, 0, 1);
        mMinQty = UINT32_MAX;
        mMaxFloor = UINT32_MAX;
        memset (mSymbol, 0, 8);
        mSymbolSfx = UINT8_MAX;
        memset (mCurrency, 0, 3);
        memset (mIDSource, 0, 1);
        memset (mSecurityID, 0, 16);
        memset (mSecurityExchange, 0, 4);
        memset (mCapacity, 0, 1);
        memset (mRoutingInst, 0, 4);
        memset (mAccount, 0, 16);
        memset (mDisplayIndicator, 0, 1);
        mPegDifference = UINT64_MAX;
        memset (mPreventParticipantMatch, 0, 3);
        mExpireTime = UINT64_MAX;
        memset (mLiquidityProvision, 0, 1);
        mClientID = UINT32_MAX;
        mInvestorID = UINT32_MAX;
        mExecutorID = UINT32_MAX;
        memset (mOrderOrigination, 0, 1);
        memset (mRoutStrategy, 0, 4);
        memset (mAlgorithmicIndicator, 0, 1);
        mClientQualifiedRole = UINT8_MAX;
        mInvestorQualifiedRole = UINT8_MAX;
        mExecutorQualifiedRole = UINT8_MAX;
    }

    
    string getClOrdID ()
    {
        return getString (mClOrdID, sizeof (mClOrdID));
    }

    bool setClOrdID (const string& val)
    {
        return setString (mClOrdID, val, sizeof (mClOrdID));
    }
    
    uint8_t getSide ()
    {
        return mSide;
    }

    bool setSide (uint8_t val)
    {
        mSide = val;
        return true;
    }
    
    uint32_t getOrderQty ()
    {
        return mOrderQty;
    }

    bool setOrderQty (uint32_t val)
    {
        mOrderQty = val;
        return true;
    }
    
    uint8_t getNumberOfBitfields ()
    {
        return mNumberOfBitfields;
    }

    bool setNumberOfBitfields (uint8_t val)
    {
        mNumberOfBitfields = val;
        return true;
    }
    
    string getClearingFirm ()
    {
        return getString (mClearingFirm, sizeof (mClearingFirm));
    }

    bool setClearingFirm (const string& val)
    {
        return setString (mClearingFirm, val, sizeof (mClearingFirm));
    }
    
    string getClearingAccount ()
    {
        return getString (mClearingAccount, sizeof (mClearingAccount));
    }

    bool setClearingAccount (const string& val)
    {
        return setString (mClearingAccount, val, sizeof (mClearingAccount));
    }
    
    uint64_t getPrice ()
    {
        return mPrice;
    }

    bool setPrice (uint64_t val)
    {
        mPrice = val;
        return true;
    }
    
    string getExecInst ()
    {
        return getString (mExecInst, sizeof (mExecInst));
    }

    bool setExecInst (const string& val)
    {
        return setString (mExecInst, val, sizeof (mExecInst));
    }
    
    string getOrdType ()
    {
        return getString (mOrdType, sizeof (mOrdType));
    }

    bool setOrdType (const string& val)
    {
        return setString (mOrdType, val, sizeof (mOrdType));
    }
    
    string getTimeInForce ()
    {
        return getString (mTimeInForce, sizeof (mTimeInForce));
    }

    bool setTimeInForce (const string& val)
    {
        return setString (mTimeInForce, val, sizeof (mTimeInForce));
    }
    
    uint32_t getMinQty ()
    {
        return mMinQty;
    }

    bool setMinQty (uint32_t val)
    {
        mMinQty = val;
        return true;
    }
    
    uint32_t getMaxFloor ()
    {
        return mMaxFloor;
    }

    bool setMaxFloor (uint32_t val)
    {
        mMaxFloor = val;
        return true;
    }
    
    string getSymbol ()
    {
        return getString (mSymbol, sizeof (mSymbol));
    }

    bool setSymbol (const string& val)
    {
        return setString (mSymbol, val, sizeof (mSymbol));
    }
    
    uint8_t getSymbolSfx ()
    {
        return mSymbolSfx;
    }

    bool setSymbolSfx (uint8_t val)
    {
        mSymbolSfx = val;
        return true;
    }
    
    string getCurrency ()
    {
        return getString (mCurrency, sizeof (mCurrency));
    }

    bool setCurrency (const string& val)
    {
        return setString (mCurrency, val, sizeof (mCurrency));
    }
    
    string getIDSource ()
    {
        return getString (mIDSource, sizeof (mIDSource));
    }

    bool setIDSource (const string& val)
    {
        return setString (mIDSource, val, sizeof (mIDSource));
    }
    
    string getSecurityID ()
    {
        return getString (mSecurityID, sizeof (mSecurityID));
    }

    bool setSecurityID (const string& val)
    {
        return setString (mSecurityID, val, sizeof (mSecurityID));
    }
    
    string getSecurityExchange ()
    {
        return getString (mSecurityExchange, sizeof (mSecurityExchange));
    }

    bool setSecurityExchange (const string& val)
    {
        return setString (mSecurityExchange, val, sizeof (mSecurityExchange));
    }
    
    string getCapacity ()
    {
        return getString (mCapacity, sizeof (mCapacity));
    }

    bool setCapacity (const string& val)
    {
        return setString (mCapacity, val, sizeof (mCapacity));
    }
    
    string getRoutingInst ()
    {
        return getString (mRoutingInst, sizeof (mRoutingInst));
    }

    bool setRoutingInst (const string& val)
    {
        return setString (mRoutingInst, val, sizeof (mRoutingInst));
    }
    
    string getAccount ()
    {
        return getString (mAccount, sizeof (mAccount));
    }

    bool setAccount (const string& val)
    {
        return setString (mAccount, val, sizeof (mAccount));
    }
    
    string getDisplayIndicator ()
    {
        return getString (mDisplayIndicator, sizeof (mDisplayIndicator));
    }

    bool setDisplayIndicator (const string& val)
    {
        return setString (mDisplayIndicator, val, sizeof (mDisplayIndicator));
    }
    
    uint64_t getPegDifference ()
    {
        return mPegDifference;
    }

    bool setPegDifference (uint64_t val)
    {
        mPegDifference = val;
        return true;
    }
    
    string getPreventParticipantMatch ()
    {
        return getString (mPreventParticipantMatch, sizeof (mPreventParticipantMatch));
    }

    bool setPreventParticipantMatch (const string& val)
    {
        return setString (mPreventParticipantMatch, val, sizeof (mPreventParticipantMatch));
    }
    
    uint64_t getExpireTime ()
    {
        return mExpireTime;
    }

    bool setExpireTime (uint64_t val)
    {
        mExpireTime = val;
        return true;
    }
    
    string getLiquidityProvision ()
    {
        return getString (mLiquidityProvision, sizeof (mLiquidityProvision));
    }

    bool setLiquidityProvision (const string& val)
    {
        return setString (mLiquidityProvision, val, sizeof (mLiquidityProvision));
    }
    
    uint32_t getClientID ()
    {
        return mClientID;
    }

    bool setClientID (uint32_t val)
    {
        mClientID = val;
        return true;
    }
    
    uint32_t getInvestorID ()
    {
        return mInvestorID;
    }

    bool setInvestorID (uint32_t val)
    {
        mInvestorID = val;
        return true;
    }
    
    uint32_t getExecutorID ()
    {
        return mExecutorID;
    }

    bool setExecutorID (uint32_t val)
    {
        mExecutorID = val;
        return true;
    }
    
    string getOrderOrigination ()
    {
        return getString (mOrderOrigination, sizeof (mOrderOrigination));
    }

    bool setOrderOrigination (const string& val)
    {
        return setString (mOrderOrigination, val, sizeof (mOrderOrigination));
    }
    
    string getRoutStrategy ()
    {
        return getString (mRoutStrategy, sizeof (mRoutStrategy));
    }

    bool setRoutStrategy (const string& val)
    {
        return setString (mRoutStrategy, val, sizeof (mRoutStrategy));
    }
    
    string getAlgorithmicIndicator ()
    {
        return getString (mAlgorithmicIndicator, sizeof (mAlgorithmicIndicator));
    }

    bool setAlgorithmicIndicator (const string& val)
    {
        return setString (mAlgorithmicIndicator, val, sizeof (mAlgorithmicIndicator));
    }
    
    uint8_t getClientQualifiedRole ()
    {
        return mClientQualifiedRole;
    }

    bool setClientQualifiedRole (uint8_t val)
    {
        mClientQualifiedRole = val;
        return true;
    }
    
    uint8_t getInvestorQualifiedRole ()
    {
        return mInvestorQualifiedRole;
    }

    bool setInvestorQualifiedRole (uint8_t val)
    {
        mInvestorQualifiedRole = val;
        return true;
    }
    
    uint8_t getExecutorQualifiedRole ()
    {
        return mExecutorQualifiedRole;
    }

    bool setExecutorQualifiedRole (uint8_t val)
    {
        mExecutorQualifiedRole = val;
        return true;
    }
});

} // namespace neueda

#endif
