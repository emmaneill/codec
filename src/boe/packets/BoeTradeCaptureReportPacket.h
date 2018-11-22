/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 13:42:16 22/11/18
 */
#ifndef _BOE_TRADECAPTUREREPORT_PACKET_H_
#define _BOE_TRADECAPTUREREPORT_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>

#include "BoePacketUtils.h"
#include "BoeHeaderPacket.h"


namespace neueda
{

PACKED(class boeTradeCaptureReportPacket
{
public:
        char mTradeReportID[20];
        uint32_t mLastShares;
        uint64_t mLastPx;
        uint8_t mNumberOfBitfields;
        uint8_t mNoSides;
        char mSymbol[8];
        char mCurrency[3];
        char mIDSource[1];
        char mSecurityID[16];
        char mSecurityExchange[4];
        char mExecInst[1];
        char mCapacity[1];
        char mAccount[16];
        char mTransactionCategory[1];
        uint64_t mTradeTime;
        char mPartyRole[1];
        uint8_t mTradeReportTransType;
        uint64_t mTradeID;
        char mVenueType[1];
        uint8_t mTradingSessionSubID;
        uint8_t mMatchType;
        uint8_t mTrdSubType;
        uint8_t mSecondaryTrdType;
        uint8_t mTradePriceCondition;
        uint64_t mLargeSize;
        char mExecutionMethod[1];
        uint8_t mTradeReportType;
        uint8_t mTradeHandlingInstruction;
        char mTradeLinkID[1];
        char mTradeReportRefID[20];
        uint64_t mGrossTradeAmt;
        uint16_t mTolerance;
        uint8_t mOrderCategory;
        uint64_t mSettlementPrice;
        uint64_t mSettlementDate;
        char mPriceFormation[1];
        char mAlgorithmicIndicator[1];
        char mWaiverType[1];
        char mDeferralReason[1];
        char mSettlementCurrency[3];
        char mSettlementLocation[2];

    boeTradeCaptureReportPacket ()
    {
        memset (mTradeReportID, 0, 20);
        mLastShares = 0;
        mLastPx = 0;
        mNumberOfBitfields = 0;
        mNoSides = 0;
        memset (mSymbol, 0, 8);
        memset (mCurrency, 0, 3);
        memset (mIDSource, 0, 1);
        memset (mSecurityID, 0, 16);
        memset (mSecurityExchange, 0, 4);
        memset (mExecInst, 0, 1);
        memset (mCapacity, 0, 1);
        memset (mAccount, 0, 16);
        memset (mTransactionCategory, 0, 1);
        mTradeTime = UINT64_MAX;
        memset (mPartyRole, 0, 1);
        mTradeReportTransType = UINT8_MAX;
        mTradeID = UINT64_MAX;
        memset (mVenueType, 0, 1);
        mTradingSessionSubID = UINT8_MAX;
        mMatchType = UINT8_MAX;
        mTrdSubType = UINT8_MAX;
        mSecondaryTrdType = UINT8_MAX;
        mTradePriceCondition = UINT8_MAX;
        mLargeSize = UINT64_MAX;
        memset (mExecutionMethod, 0, 1);
        mTradeReportType = UINT8_MAX;
        mTradeHandlingInstruction = UINT8_MAX;
        memset (mTradeLinkID, 0, 1);
        memset (mTradeReportRefID, 0, 20);
        mGrossTradeAmt = UINT64_MAX;
        mTolerance = UINT16_MAX;
        mOrderCategory = UINT8_MAX;
        mSettlementPrice = UINT64_MAX;
        mSettlementDate = UINT64_MAX;
        memset (mPriceFormation, 0, 1);
        memset (mAlgorithmicIndicator, 0, 1);
        memset (mWaiverType, 0, 1);
        memset (mDeferralReason, 0, 1);
        memset (mSettlementCurrency, 0, 3);
        memset (mSettlementLocation, 0, 2);
    }

    
    string getTradeReportID ()
    {
        return getString (mTradeReportID, sizeof (mTradeReportID));
    }

    bool setTradeReportID (const string& val)
    {
        return setString (mTradeReportID, val, sizeof (mTradeReportID));
    }
    
    uint32_t getLastShares ()
    {
        return mLastShares;
    }

    bool setLastShares (uint32_t val)
    {
        mLastShares = val;
        return true;
    }
    
    uint64_t getLastPx ()
    {
        return mLastPx;
    }

    bool setLastPx (uint64_t val)
    {
        mLastPx = val;
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
    
    uint8_t getNoSides ()
    {
        return mNoSides;
    }

    bool setNoSides (uint8_t val)
    {
        mNoSides = val;
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
    
    string getExecInst ()
    {
        return getString (mExecInst, sizeof (mExecInst));
    }

    bool setExecInst (const string& val)
    {
        return setString (mExecInst, val, sizeof (mExecInst));
    }
    
    string getCapacity ()
    {
        return getString (mCapacity, sizeof (mCapacity));
    }

    bool setCapacity (const string& val)
    {
        return setString (mCapacity, val, sizeof (mCapacity));
    }
    
    string getAccount ()
    {
        return getString (mAccount, sizeof (mAccount));
    }

    bool setAccount (const string& val)
    {
        return setString (mAccount, val, sizeof (mAccount));
    }
    
    string getTransactionCategory ()
    {
        return getString (mTransactionCategory, sizeof (mTransactionCategory));
    }

    bool setTransactionCategory (const string& val)
    {
        return setString (mTransactionCategory, val, sizeof (mTransactionCategory));
    }
    
    uint64_t getTradeTime ()
    {
        return mTradeTime;
    }

    bool setTradeTime (uint64_t val)
    {
        mTradeTime = val;
        return true;
    }
    
    string getPartyRole ()
    {
        return getString (mPartyRole, sizeof (mPartyRole));
    }

    bool setPartyRole (const string& val)
    {
        return setString (mPartyRole, val, sizeof (mPartyRole));
    }
    
    uint8_t getTradeReportTransType ()
    {
        return mTradeReportTransType;
    }

    bool setTradeReportTransType (uint8_t val)
    {
        mTradeReportTransType = val;
        return true;
    }
    
    uint64_t getTradeID ()
    {
        return mTradeID;
    }

    bool setTradeID (uint64_t val)
    {
        mTradeID = val;
        return true;
    }
    
    string getVenueType ()
    {
        return getString (mVenueType, sizeof (mVenueType));
    }

    bool setVenueType (const string& val)
    {
        return setString (mVenueType, val, sizeof (mVenueType));
    }
    
    uint8_t getTradingSessionSubID ()
    {
        return mTradingSessionSubID;
    }

    bool setTradingSessionSubID (uint8_t val)
    {
        mTradingSessionSubID = val;
        return true;
    }
    
    uint8_t getMatchType ()
    {
        return mMatchType;
    }

    bool setMatchType (uint8_t val)
    {
        mMatchType = val;
        return true;
    }
    
    uint8_t getTrdSubType ()
    {
        return mTrdSubType;
    }

    bool setTrdSubType (uint8_t val)
    {
        mTrdSubType = val;
        return true;
    }
    
    uint8_t getSecondaryTrdType ()
    {
        return mSecondaryTrdType;
    }

    bool setSecondaryTrdType (uint8_t val)
    {
        mSecondaryTrdType = val;
        return true;
    }
    
    uint8_t getTradePriceCondition ()
    {
        return mTradePriceCondition;
    }

    bool setTradePriceCondition (uint8_t val)
    {
        mTradePriceCondition = val;
        return true;
    }
    
    uint64_t getLargeSize ()
    {
        return mLargeSize;
    }

    bool setLargeSize (uint64_t val)
    {
        mLargeSize = val;
        return true;
    }
    
    string getExecutionMethod ()
    {
        return getString (mExecutionMethod, sizeof (mExecutionMethod));
    }

    bool setExecutionMethod (const string& val)
    {
        return setString (mExecutionMethod, val, sizeof (mExecutionMethod));
    }
    
    uint8_t getTradeReportType ()
    {
        return mTradeReportType;
    }

    bool setTradeReportType (uint8_t val)
    {
        mTradeReportType = val;
        return true;
    }
    
    uint8_t getTradeHandlingInstruction ()
    {
        return mTradeHandlingInstruction;
    }

    bool setTradeHandlingInstruction (uint8_t val)
    {
        mTradeHandlingInstruction = val;
        return true;
    }
    
    string getTradeLinkID ()
    {
        return getString (mTradeLinkID, sizeof (mTradeLinkID));
    }

    bool setTradeLinkID (const string& val)
    {
        return setString (mTradeLinkID, val, sizeof (mTradeLinkID));
    }
    
    string getTradeReportRefID ()
    {
        return getString (mTradeReportRefID, sizeof (mTradeReportRefID));
    }

    bool setTradeReportRefID (const string& val)
    {
        return setString (mTradeReportRefID, val, sizeof (mTradeReportRefID));
    }
    
    uint64_t getGrossTradeAmt ()
    {
        return mGrossTradeAmt;
    }

    bool setGrossTradeAmt (uint64_t val)
    {
        mGrossTradeAmt = val;
        return true;
    }
    
    uint16_t getTolerance ()
    {
        return mTolerance;
    }

    bool setTolerance (uint16_t val)
    {
        mTolerance = val;
        return true;
    }
    
    uint8_t getOrderCategory ()
    {
        return mOrderCategory;
    }

    bool setOrderCategory (uint8_t val)
    {
        mOrderCategory = val;
        return true;
    }
    
    uint64_t getSettlementPrice ()
    {
        return mSettlementPrice;
    }

    bool setSettlementPrice (uint64_t val)
    {
        mSettlementPrice = val;
        return true;
    }
    
    uint64_t getSettlementDate ()
    {
        return mSettlementDate;
    }

    bool setSettlementDate (uint64_t val)
    {
        mSettlementDate = val;
        return true;
    }
    
    string getPriceFormation ()
    {
        return getString (mPriceFormation, sizeof (mPriceFormation));
    }

    bool setPriceFormation (const string& val)
    {
        return setString (mPriceFormation, val, sizeof (mPriceFormation));
    }
    
    string getAlgorithmicIndicator ()
    {
        return getString (mAlgorithmicIndicator, sizeof (mAlgorithmicIndicator));
    }

    bool setAlgorithmicIndicator (const string& val)
    {
        return setString (mAlgorithmicIndicator, val, sizeof (mAlgorithmicIndicator));
    }
    
    string getWaiverType ()
    {
        return getString (mWaiverType, sizeof (mWaiverType));
    }

    bool setWaiverType (const string& val)
    {
        return setString (mWaiverType, val, sizeof (mWaiverType));
    }
    
    string getDeferralReason ()
    {
        return getString (mDeferralReason, sizeof (mDeferralReason));
    }

    bool setDeferralReason (const string& val)
    {
        return setString (mDeferralReason, val, sizeof (mDeferralReason));
    }
    
    string getSettlementCurrency ()
    {
        return getString (mSettlementCurrency, sizeof (mSettlementCurrency));
    }

    bool setSettlementCurrency (const string& val)
    {
        return setString (mSettlementCurrency, val, sizeof (mSettlementCurrency));
    }
    
    string getSettlementLocation ()
    {
        return getString (mSettlementLocation, sizeof (mSettlementLocation));
    }

    bool setSettlementLocation (const string& val)
    {
        return setString (mSettlementLocation, val, sizeof (mSettlementLocation));
    }
});

} // namespace neueda

#endif
