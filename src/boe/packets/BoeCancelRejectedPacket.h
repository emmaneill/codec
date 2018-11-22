/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 13:42:16 22/11/18
 */
#ifndef _BOE_CANCELREJECTED_PACKET_H_
#define _BOE_CANCELREJECTED_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>

#include "BoePacketUtils.h"
#include "BoeHeaderPacket.h"


namespace neueda
{

PACKED(class boeCancelRejectedPacket
{
public:
        uint64_t mTransactTime;
        char mClOrdID[20];
        uint8_t mCancelRejectReason;
        char mCancelRejectText[60];
        char mReservedInternal[1];
        uint8_t mNumberOfBitfields;
        char mSide[1];
        uint64_t mPegDifference;
        uint64_t mPrice;
        char mExecInst[1];
        char mOrdType[1];
        char mTimeInForce[1];
        uint32_t mMinQty;
        char mSymbol[8];
        uint8_t mSymbolSfx;
        char mCurrency[3];
        char mIDSource[1];
        char mSecurityID[16];
        char mSecurityExchange[4];
        char mCapacity[1];
        char mAccount[16];
        char mClearingFirm[4];
        char mClearingAccount[4];
        char mDisplayIndicator[1];
        uint32_t mMaxFloor;
        uint32_t mOrderQty;
        char mPreventParticipantMatch[3];
        uint32_t mCorrectedSize;
        char mOrigClOrdID[20];
        uint32_t mLeavesQty;
        uint32_t mLastShares;
        uint64_t mDisplayPrice;
        uint64_t mWorkingPrice;
        char mBaseLiquidityIndicator[1];
        uint64_t mSecondaryOrderID;
        char mCcp[1];
        char mContraCapacity[1];
        uint8_t mAttributedQuote;
        uint64_t mBulkOrderIDs;
        char mBulkRejectReasons[1];
        char mPartyRole[1];
        char mSubLiquidityIndicator[1];
        uint16_t mTradeReportTypeReturn;
        uint8_t mTradePublishIndReturn;
        char mText[60];
        uint64_t mBidPx;
        uint64_t mLargeSize;
        char mLastMkt[4];
        char mFeeCode[2];
        char mEchoText[60];
        uint64_t mStopPx;
        char mRoutingInst[4];
        char mRoutStrategy[4];
        char mExDestination[1];
        char mTradeReportRefID[20];
        char mMarketingFeeCode[2];
        char mTargetPartyID[4];
        uint64_t mAuctionID;
        uint8_t mOrderCategory;
        char mLiquidityProvision[1];
        uint32_t mCmtaNumber;
        char mCrossType[1];
        char mCrossPrioritization[1];
        uint32_t mAllocQty;
        char mRoutingFirmID[4];
        char mWaiverType[1];
        char mCrossExclusionIndicator[1];
        char mPriceFormation[1];
        uint8_t mClientQualifiedRole;
        uint32_t mClientID;
        uint32_t mInvestorID;
        uint32_t mExecutorID;
        char mOrderOrigination[1];
        char mAlgorithmicIndicator[1];
        char mDeferralReason[1];
        uint8_t mInvestorQualifiedRole;
        uint8_t mExecutorQualifiedRole;
        char mCtiCode[2];
        uint8_t mManualOrderIndicator;
        uint64_t mTradeDate;
        uint8_t mVariancePrice;
        uint64_t mVarianceSize;
        uint8_t mOrigTASPrice;
        uint8_t mCumQty;
        uint8_t mDayOrderQty;
        uint8_t mDayCumQty;
        uint64_t mAvgPx;
        uint64_t mDayAvgPx;
        char mLegCFICode[2];
        uint64_t mLegMaturityDate;
        uint8_t mLegStrikePrice;
        uint32_t mSecondaryExecID;
        char mUsername[4];
        char mTradeReportingIndicator[1];
        char mTradePublishIndicator[1];
        uint8_t mReportTime;

    boeCancelRejectedPacket ()
    {
        mTransactTime = 0;
        memset (mClOrdID, 0, 20);
        mCancelRejectReason = 0;
        memset (mCancelRejectText, 0, 60);
        memset (mReservedInternal, 0, 1);
        mNumberOfBitfields = 0;
        memset (mSide, 0, 1);
        mPegDifference = UINT64_MAX;
        mPrice = UINT64_MAX;
        memset (mExecInst, 0, 1);
        memset (mOrdType, 0, 1);
        memset (mTimeInForce, 0, 1);
        mMinQty = UINT32_MAX;
        memset (mSymbol, 0, 8);
        mSymbolSfx = UINT8_MAX;
        memset (mCurrency, 0, 3);
        memset (mIDSource, 0, 1);
        memset (mSecurityID, 0, 16);
        memset (mSecurityExchange, 0, 4);
        memset (mCapacity, 0, 1);
        memset (mAccount, 0, 16);
        memset (mClearingFirm, 0, 4);
        memset (mClearingAccount, 0, 4);
        memset (mDisplayIndicator, 0, 1);
        mMaxFloor = UINT32_MAX;
        mOrderQty = UINT32_MAX;
        memset (mPreventParticipantMatch, 0, 3);
        mCorrectedSize = UINT32_MAX;
        memset (mOrigClOrdID, 0, 20);
        mLeavesQty = UINT32_MAX;
        mLastShares = UINT32_MAX;
        mDisplayPrice = UINT64_MAX;
        mWorkingPrice = UINT64_MAX;
        memset (mBaseLiquidityIndicator, 0, 1);
        mSecondaryOrderID = UINT64_MAX;
        memset (mCcp, 0, 1);
        memset (mContraCapacity, 0, 1);
        mAttributedQuote = UINT8_MAX;
        mBulkOrderIDs = UINT64_MAX;
        memset (mBulkRejectReasons, 0, 1);
        memset (mPartyRole, 0, 1);
        memset (mSubLiquidityIndicator, 0, 1);
        mTradeReportTypeReturn = UINT16_MAX;
        mTradePublishIndReturn = UINT8_MAX;
        memset (mText, 0, 60);
        mBidPx = UINT64_MAX;
        mLargeSize = UINT64_MAX;
        memset (mLastMkt, 0, 4);
        memset (mFeeCode, 0, 2);
        memset (mEchoText, 0, 60);
        mStopPx = UINT64_MAX;
        memset (mRoutingInst, 0, 4);
        memset (mRoutStrategy, 0, 4);
        memset (mExDestination, 0, 1);
        memset (mTradeReportRefID, 0, 20);
        memset (mMarketingFeeCode, 0, 2);
        memset (mTargetPartyID, 0, 4);
        mAuctionID = UINT64_MAX;
        mOrderCategory = UINT8_MAX;
        memset (mLiquidityProvision, 0, 1);
        mCmtaNumber = UINT32_MAX;
        memset (mCrossType, 0, 1);
        memset (mCrossPrioritization, 0, 1);
        mAllocQty = UINT32_MAX;
        memset (mRoutingFirmID, 0, 4);
        memset (mWaiverType, 0, 1);
        memset (mCrossExclusionIndicator, 0, 1);
        memset (mPriceFormation, 0, 1);
        mClientQualifiedRole = UINT8_MAX;
        mClientID = UINT32_MAX;
        mInvestorID = UINT32_MAX;
        mExecutorID = UINT32_MAX;
        memset (mOrderOrigination, 0, 1);
        memset (mAlgorithmicIndicator, 0, 1);
        memset (mDeferralReason, 0, 1);
        mInvestorQualifiedRole = UINT8_MAX;
        mExecutorQualifiedRole = UINT8_MAX;
        memset (mCtiCode, 0, 2);
        mManualOrderIndicator = UINT8_MAX;
        mTradeDate = UINT64_MAX;
        mVariancePrice = UINT8_MAX;
        mVarianceSize = UINT64_MAX;
        mOrigTASPrice = UINT8_MAX;
        mCumQty = UINT8_MAX;
        mDayOrderQty = UINT8_MAX;
        mDayCumQty = UINT8_MAX;
        mAvgPx = UINT64_MAX;
        mDayAvgPx = UINT64_MAX;
        memset (mLegCFICode, 0, 2);
        mLegMaturityDate = UINT64_MAX;
        mLegStrikePrice = UINT8_MAX;
        mSecondaryExecID = UINT32_MAX;
        memset (mUsername, 0, 4);
        memset (mTradeReportingIndicator, 0, 1);
        memset (mTradePublishIndicator, 0, 1);
        mReportTime = UINT8_MAX;
    }

    
    uint64_t getTransactTime ()
    {
        return mTransactTime;
    }

    bool setTransactTime (uint64_t val)
    {
        mTransactTime = val;
        return true;
    }
    
    string getClOrdID ()
    {
        return getString (mClOrdID, sizeof (mClOrdID));
    }

    bool setClOrdID (const string& val)
    {
        return setString (mClOrdID, val, sizeof (mClOrdID));
    }
    
    uint8_t getCancelRejectReason ()
    {
        return mCancelRejectReason;
    }

    bool setCancelRejectReason (uint8_t val)
    {
        mCancelRejectReason = val;
        return true;
    }
    
    string getCancelRejectText ()
    {
        return getString (mCancelRejectText, sizeof (mCancelRejectText));
    }

    bool setCancelRejectText (const string& val)
    {
        return setString (mCancelRejectText, val, sizeof (mCancelRejectText));
    }
    
    string getReservedInternal ()
    {
        return getString (mReservedInternal, sizeof (mReservedInternal));
    }

    bool setReservedInternal (const string& val)
    {
        return setString (mReservedInternal, val, sizeof (mReservedInternal));
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
    
    string getSide ()
    {
        return getString (mSide, sizeof (mSide));
    }

    bool setSide (const string& val)
    {
        return setString (mSide, val, sizeof (mSide));
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
    
    string getAccount ()
    {
        return getString (mAccount, sizeof (mAccount));
    }

    bool setAccount (const string& val)
    {
        return setString (mAccount, val, sizeof (mAccount));
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
    
    string getDisplayIndicator ()
    {
        return getString (mDisplayIndicator, sizeof (mDisplayIndicator));
    }

    bool setDisplayIndicator (const string& val)
    {
        return setString (mDisplayIndicator, val, sizeof (mDisplayIndicator));
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
    
    uint32_t getOrderQty ()
    {
        return mOrderQty;
    }

    bool setOrderQty (uint32_t val)
    {
        mOrderQty = val;
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
    
    uint32_t getCorrectedSize ()
    {
        return mCorrectedSize;
    }

    bool setCorrectedSize (uint32_t val)
    {
        mCorrectedSize = val;
        return true;
    }
    
    string getOrigClOrdID ()
    {
        return getString (mOrigClOrdID, sizeof (mOrigClOrdID));
    }

    bool setOrigClOrdID (const string& val)
    {
        return setString (mOrigClOrdID, val, sizeof (mOrigClOrdID));
    }
    
    uint32_t getLeavesQty ()
    {
        return mLeavesQty;
    }

    bool setLeavesQty (uint32_t val)
    {
        mLeavesQty = val;
        return true;
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
    
    uint64_t getDisplayPrice ()
    {
        return mDisplayPrice;
    }

    bool setDisplayPrice (uint64_t val)
    {
        mDisplayPrice = val;
        return true;
    }
    
    uint64_t getWorkingPrice ()
    {
        return mWorkingPrice;
    }

    bool setWorkingPrice (uint64_t val)
    {
        mWorkingPrice = val;
        return true;
    }
    
    string getBaseLiquidityIndicator ()
    {
        return getString (mBaseLiquidityIndicator, sizeof (mBaseLiquidityIndicator));
    }

    bool setBaseLiquidityIndicator (const string& val)
    {
        return setString (mBaseLiquidityIndicator, val, sizeof (mBaseLiquidityIndicator));
    }
    
    uint64_t getSecondaryOrderID ()
    {
        return mSecondaryOrderID;
    }

    bool setSecondaryOrderID (uint64_t val)
    {
        mSecondaryOrderID = val;
        return true;
    }
    
    string getCcp ()
    {
        return getString (mCcp, sizeof (mCcp));
    }

    bool setCcp (const string& val)
    {
        return setString (mCcp, val, sizeof (mCcp));
    }
    
    string getContraCapacity ()
    {
        return getString (mContraCapacity, sizeof (mContraCapacity));
    }

    bool setContraCapacity (const string& val)
    {
        return setString (mContraCapacity, val, sizeof (mContraCapacity));
    }
    
    uint8_t getAttributedQuote ()
    {
        return mAttributedQuote;
    }

    bool setAttributedQuote (uint8_t val)
    {
        mAttributedQuote = val;
        return true;
    }
    
    uint64_t getBulkOrderIDs ()
    {
        return mBulkOrderIDs;
    }

    bool setBulkOrderIDs (uint64_t val)
    {
        mBulkOrderIDs = val;
        return true;
    }
    
    string getBulkRejectReasons ()
    {
        return getString (mBulkRejectReasons, sizeof (mBulkRejectReasons));
    }

    bool setBulkRejectReasons (const string& val)
    {
        return setString (mBulkRejectReasons, val, sizeof (mBulkRejectReasons));
    }
    
    string getPartyRole ()
    {
        return getString (mPartyRole, sizeof (mPartyRole));
    }

    bool setPartyRole (const string& val)
    {
        return setString (mPartyRole, val, sizeof (mPartyRole));
    }
    
    string getSubLiquidityIndicator ()
    {
        return getString (mSubLiquidityIndicator, sizeof (mSubLiquidityIndicator));
    }

    bool setSubLiquidityIndicator (const string& val)
    {
        return setString (mSubLiquidityIndicator, val, sizeof (mSubLiquidityIndicator));
    }
    
    uint16_t getTradeReportTypeReturn ()
    {
        return mTradeReportTypeReturn;
    }

    bool setTradeReportTypeReturn (uint16_t val)
    {
        mTradeReportTypeReturn = val;
        return true;
    }
    
    uint8_t getTradePublishIndReturn ()
    {
        return mTradePublishIndReturn;
    }

    bool setTradePublishIndReturn (uint8_t val)
    {
        mTradePublishIndReturn = val;
        return true;
    }
    
    string getText ()
    {
        return getString (mText, sizeof (mText));
    }

    bool setText (const string& val)
    {
        return setString (mText, val, sizeof (mText));
    }
    
    uint64_t getBidPx ()
    {
        return mBidPx;
    }

    bool setBidPx (uint64_t val)
    {
        mBidPx = val;
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
    
    string getLastMkt ()
    {
        return getString (mLastMkt, sizeof (mLastMkt));
    }

    bool setLastMkt (const string& val)
    {
        return setString (mLastMkt, val, sizeof (mLastMkt));
    }
    
    string getFeeCode ()
    {
        return getString (mFeeCode, sizeof (mFeeCode));
    }

    bool setFeeCode (const string& val)
    {
        return setString (mFeeCode, val, sizeof (mFeeCode));
    }
    
    string getEchoText ()
    {
        return getString (mEchoText, sizeof (mEchoText));
    }

    bool setEchoText (const string& val)
    {
        return setString (mEchoText, val, sizeof (mEchoText));
    }
    
    uint64_t getStopPx ()
    {
        return mStopPx;
    }

    bool setStopPx (uint64_t val)
    {
        mStopPx = val;
        return true;
    }
    
    string getRoutingInst ()
    {
        return getString (mRoutingInst, sizeof (mRoutingInst));
    }

    bool setRoutingInst (const string& val)
    {
        return setString (mRoutingInst, val, sizeof (mRoutingInst));
    }
    
    string getRoutStrategy ()
    {
        return getString (mRoutStrategy, sizeof (mRoutStrategy));
    }

    bool setRoutStrategy (const string& val)
    {
        return setString (mRoutStrategy, val, sizeof (mRoutStrategy));
    }
    
    string getExDestination ()
    {
        return getString (mExDestination, sizeof (mExDestination));
    }

    bool setExDestination (const string& val)
    {
        return setString (mExDestination, val, sizeof (mExDestination));
    }
    
    string getTradeReportRefID ()
    {
        return getString (mTradeReportRefID, sizeof (mTradeReportRefID));
    }

    bool setTradeReportRefID (const string& val)
    {
        return setString (mTradeReportRefID, val, sizeof (mTradeReportRefID));
    }
    
    string getMarketingFeeCode ()
    {
        return getString (mMarketingFeeCode, sizeof (mMarketingFeeCode));
    }

    bool setMarketingFeeCode (const string& val)
    {
        return setString (mMarketingFeeCode, val, sizeof (mMarketingFeeCode));
    }
    
    string getTargetPartyID ()
    {
        return getString (mTargetPartyID, sizeof (mTargetPartyID));
    }

    bool setTargetPartyID (const string& val)
    {
        return setString (mTargetPartyID, val, sizeof (mTargetPartyID));
    }
    
    uint64_t getAuctionID ()
    {
        return mAuctionID;
    }

    bool setAuctionID (uint64_t val)
    {
        mAuctionID = val;
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
    
    string getLiquidityProvision ()
    {
        return getString (mLiquidityProvision, sizeof (mLiquidityProvision));
    }

    bool setLiquidityProvision (const string& val)
    {
        return setString (mLiquidityProvision, val, sizeof (mLiquidityProvision));
    }
    
    uint32_t getCmtaNumber ()
    {
        return mCmtaNumber;
    }

    bool setCmtaNumber (uint32_t val)
    {
        mCmtaNumber = val;
        return true;
    }
    
    string getCrossType ()
    {
        return getString (mCrossType, sizeof (mCrossType));
    }

    bool setCrossType (const string& val)
    {
        return setString (mCrossType, val, sizeof (mCrossType));
    }
    
    string getCrossPrioritization ()
    {
        return getString (mCrossPrioritization, sizeof (mCrossPrioritization));
    }

    bool setCrossPrioritization (const string& val)
    {
        return setString (mCrossPrioritization, val, sizeof (mCrossPrioritization));
    }
    
    uint32_t getAllocQty ()
    {
        return mAllocQty;
    }

    bool setAllocQty (uint32_t val)
    {
        mAllocQty = val;
        return true;
    }
    
    string getRoutingFirmID ()
    {
        return getString (mRoutingFirmID, sizeof (mRoutingFirmID));
    }

    bool setRoutingFirmID (const string& val)
    {
        return setString (mRoutingFirmID, val, sizeof (mRoutingFirmID));
    }
    
    string getWaiverType ()
    {
        return getString (mWaiverType, sizeof (mWaiverType));
    }

    bool setWaiverType (const string& val)
    {
        return setString (mWaiverType, val, sizeof (mWaiverType));
    }
    
    string getCrossExclusionIndicator ()
    {
        return getString (mCrossExclusionIndicator, sizeof (mCrossExclusionIndicator));
    }

    bool setCrossExclusionIndicator (const string& val)
    {
        return setString (mCrossExclusionIndicator, val, sizeof (mCrossExclusionIndicator));
    }
    
    string getPriceFormation ()
    {
        return getString (mPriceFormation, sizeof (mPriceFormation));
    }

    bool setPriceFormation (const string& val)
    {
        return setString (mPriceFormation, val, sizeof (mPriceFormation));
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
    
    string getAlgorithmicIndicator ()
    {
        return getString (mAlgorithmicIndicator, sizeof (mAlgorithmicIndicator));
    }

    bool setAlgorithmicIndicator (const string& val)
    {
        return setString (mAlgorithmicIndicator, val, sizeof (mAlgorithmicIndicator));
    }
    
    string getDeferralReason ()
    {
        return getString (mDeferralReason, sizeof (mDeferralReason));
    }

    bool setDeferralReason (const string& val)
    {
        return setString (mDeferralReason, val, sizeof (mDeferralReason));
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
    
    string getCtiCode ()
    {
        return getString (mCtiCode, sizeof (mCtiCode));
    }

    bool setCtiCode (const string& val)
    {
        return setString (mCtiCode, val, sizeof (mCtiCode));
    }
    
    uint8_t getManualOrderIndicator ()
    {
        return mManualOrderIndicator;
    }

    bool setManualOrderIndicator (uint8_t val)
    {
        mManualOrderIndicator = val;
        return true;
    }
    
    uint64_t getTradeDate ()
    {
        return mTradeDate;
    }

    bool setTradeDate (uint64_t val)
    {
        mTradeDate = val;
        return true;
    }
    
    uint8_t getVariancePrice ()
    {
        return mVariancePrice;
    }

    bool setVariancePrice (uint8_t val)
    {
        mVariancePrice = val;
        return true;
    }
    
    uint64_t getVarianceSize ()
    {
        return mVarianceSize;
    }

    bool setVarianceSize (uint64_t val)
    {
        mVarianceSize = val;
        return true;
    }
    
    uint8_t getOrigTASPrice ()
    {
        return mOrigTASPrice;
    }

    bool setOrigTASPrice (uint8_t val)
    {
        mOrigTASPrice = val;
        return true;
    }
    
    uint8_t getCumQty ()
    {
        return mCumQty;
    }

    bool setCumQty (uint8_t val)
    {
        mCumQty = val;
        return true;
    }
    
    uint8_t getDayOrderQty ()
    {
        return mDayOrderQty;
    }

    bool setDayOrderQty (uint8_t val)
    {
        mDayOrderQty = val;
        return true;
    }
    
    uint8_t getDayCumQty ()
    {
        return mDayCumQty;
    }

    bool setDayCumQty (uint8_t val)
    {
        mDayCumQty = val;
        return true;
    }
    
    uint64_t getAvgPx ()
    {
        return mAvgPx;
    }

    bool setAvgPx (uint64_t val)
    {
        mAvgPx = val;
        return true;
    }
    
    uint64_t getDayAvgPx ()
    {
        return mDayAvgPx;
    }

    bool setDayAvgPx (uint64_t val)
    {
        mDayAvgPx = val;
        return true;
    }
    
    string getLegCFICode ()
    {
        return getString (mLegCFICode, sizeof (mLegCFICode));
    }

    bool setLegCFICode (const string& val)
    {
        return setString (mLegCFICode, val, sizeof (mLegCFICode));
    }
    
    uint64_t getLegMaturityDate ()
    {
        return mLegMaturityDate;
    }

    bool setLegMaturityDate (uint64_t val)
    {
        mLegMaturityDate = val;
        return true;
    }
    
    uint8_t getLegStrikePrice ()
    {
        return mLegStrikePrice;
    }

    bool setLegStrikePrice (uint8_t val)
    {
        mLegStrikePrice = val;
        return true;
    }
    
    uint32_t getSecondaryExecID ()
    {
        return mSecondaryExecID;
    }

    bool setSecondaryExecID (uint32_t val)
    {
        mSecondaryExecID = val;
        return true;
    }
    
    string getUsername ()
    {
        return getString (mUsername, sizeof (mUsername));
    }

    bool setUsername (const string& val)
    {
        return setString (mUsername, val, sizeof (mUsername));
    }
    
    string getTradeReportingIndicator ()
    {
        return getString (mTradeReportingIndicator, sizeof (mTradeReportingIndicator));
    }

    bool setTradeReportingIndicator (const string& val)
    {
        return setString (mTradeReportingIndicator, val, sizeof (mTradeReportingIndicator));
    }
    
    string getTradePublishIndicator ()
    {
        return getString (mTradePublishIndicator, sizeof (mTradePublishIndicator));
    }

    bool setTradePublishIndicator (const string& val)
    {
        return setString (mTradePublishIndicator, val, sizeof (mTradePublishIndicator));
    }
    
    uint8_t getReportTime ()
    {
        return mReportTime;
    }

    bool setReportTime (uint8_t val)
    {
        mReportTime = val;
        return true;
    }
});

} // namespace neueda

#endif
