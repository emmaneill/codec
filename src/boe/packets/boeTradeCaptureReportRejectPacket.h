/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 15:00:46 26/10/18
 */
#ifndef _BOE_TRADECAPTUREREPORTREJECT_PACKET_H_
#define _BOE_TRADECAPTUREREPORTREJECT_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>

#include "boeConstants.h"
#include "boePacketUtils.h"
#include "boeMessageHeaderPacket.h"


namespace neueda
{

PACKED(class boeTradeCaptureReportRejectPacket
{
public:
    uint16_t mFrameLength;
    optiqMessageHeaderPacket mHeader;
        uint64_t mTransactTime;
        char mTradeReportID[20];
        ReasonCodes_enum mTradeCaptureRejectReason;
        char mTradeCaptureRejectText[60];
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
        uint8_t mNoSides;

    boeTradeCaptureReportRejectPacket ()
    {
        mFrameLength = sizeof (boeTradeCaptureReportRejectPacket) - sizeof (uint16_t);
        mHeader.setBlockLength (sizeof (boeTradeCaptureReportRejectPacket) -
                                sizeof (optiqMessageHeaderPacket) -
                                sizeof (mFrameLength));
        mHeader.setTemplateId (BoeTradeCaptureReportRejectTemplateId);
        mHeader.setSchemaId (0);
        mHeader.setVersion (102);
        mTransactTime = 0;
        memset (mTradeReportID, 0, 20);
        mTradeCaptureRejectReason = 0;
        memset (mTradeCaptureRejectText, 0, 60);
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
        mNoSides = 0;
    }

    
    uint64_t getTransactTime () const
    {
        return mTransactTime;
    }

    bool setTransactTime (uint64_t val)
    {
        mTransactTime = val;
        return true;
    }
    
    string getTradeReportID () const
    {
        return getString (mTradeReportID, sizeof (mTradeReportID));
    }

    bool setTradeReportID (const string& val)
    {
        return setString (mTradeReportID, val, sizeof (mTradeReportID));
    }
    //TODO unhandled type ReasonCodes_enum
    
    string getTradeCaptureRejectText () const
    {
        return getString (mTradeCaptureRejectText, sizeof (mTradeCaptureRejectText));
    }

    bool setTradeCaptureRejectText (const string& val)
    {
        return setString (mTradeCaptureRejectText, val, sizeof (mTradeCaptureRejectText));
    }
    
    uint8_t getNumberOfBitfields () const
    {
        return mNumberOfBitfields;
    }

    bool setNumberOfBitfields (uint8_t val)
    {
        mNumberOfBitfields = val;
        return true;
    }
    
    string getSide () const
    {
        return getString (mSide, sizeof (mSide));
    }

    bool setSide (const string& val)
    {
        return setString (mSide, val, sizeof (mSide));
    }
    
    uint64_t getPegDifference () const
    {
        return mPegDifference;
    }

    bool setPegDifference (uint64_t val)
    {
        mPegDifference = val;
        return true;
    }
    
    uint64_t getPrice () const
    {
        return mPrice;
    }

    bool setPrice (uint64_t val)
    {
        mPrice = val;
        return true;
    }
    
    string getExecInst () const
    {
        return getString (mExecInst, sizeof (mExecInst));
    }

    bool setExecInst (const string& val)
    {
        return setString (mExecInst, val, sizeof (mExecInst));
    }
    
    string getOrdType () const
    {
        return getString (mOrdType, sizeof (mOrdType));
    }

    bool setOrdType (const string& val)
    {
        return setString (mOrdType, val, sizeof (mOrdType));
    }
    
    string getTimeInForce () const
    {
        return getString (mTimeInForce, sizeof (mTimeInForce));
    }

    bool setTimeInForce (const string& val)
    {
        return setString (mTimeInForce, val, sizeof (mTimeInForce));
    }
    
    uint32_t getMinQty () const
    {
        return mMinQty;
    }

    bool setMinQty (uint32_t val)
    {
        mMinQty = val;
        return true;
    }
    
    string getSymbol () const
    {
        return getString (mSymbol, sizeof (mSymbol));
    }

    bool setSymbol (const string& val)
    {
        return setString (mSymbol, val, sizeof (mSymbol));
    }
    
    uint8_t getSymbolSfx () const
    {
        return mSymbolSfx;
    }

    bool setSymbolSfx (uint8_t val)
    {
        mSymbolSfx = val;
        return true;
    }
    
    string getCurrency () const
    {
        return getString (mCurrency, sizeof (mCurrency));
    }

    bool setCurrency (const string& val)
    {
        return setString (mCurrency, val, sizeof (mCurrency));
    }
    
    string getIDSource () const
    {
        return getString (mIDSource, sizeof (mIDSource));
    }

    bool setIDSource (const string& val)
    {
        return setString (mIDSource, val, sizeof (mIDSource));
    }
    
    string getSecurityID () const
    {
        return getString (mSecurityID, sizeof (mSecurityID));
    }

    bool setSecurityID (const string& val)
    {
        return setString (mSecurityID, val, sizeof (mSecurityID));
    }
    
    string getSecurityExchange () const
    {
        return getString (mSecurityExchange, sizeof (mSecurityExchange));
    }

    bool setSecurityExchange (const string& val)
    {
        return setString (mSecurityExchange, val, sizeof (mSecurityExchange));
    }
    
    string getCapacity () const
    {
        return getString (mCapacity, sizeof (mCapacity));
    }

    bool setCapacity (const string& val)
    {
        return setString (mCapacity, val, sizeof (mCapacity));
    }
    
    string getAccount () const
    {
        return getString (mAccount, sizeof (mAccount));
    }

    bool setAccount (const string& val)
    {
        return setString (mAccount, val, sizeof (mAccount));
    }
    
    string getClearingFirm () const
    {
        return getString (mClearingFirm, sizeof (mClearingFirm));
    }

    bool setClearingFirm (const string& val)
    {
        return setString (mClearingFirm, val, sizeof (mClearingFirm));
    }
    
    string getClearingAccount () const
    {
        return getString (mClearingAccount, sizeof (mClearingAccount));
    }

    bool setClearingAccount (const string& val)
    {
        return setString (mClearingAccount, val, sizeof (mClearingAccount));
    }
    
    string getDisplayIndicator () const
    {
        return getString (mDisplayIndicator, sizeof (mDisplayIndicator));
    }

    bool setDisplayIndicator (const string& val)
    {
        return setString (mDisplayIndicator, val, sizeof (mDisplayIndicator));
    }
    
    uint32_t getMaxFloor () const
    {
        return mMaxFloor;
    }

    bool setMaxFloor (uint32_t val)
    {
        mMaxFloor = val;
        return true;
    }
    
    uint32_t getOrderQty () const
    {
        return mOrderQty;
    }

    bool setOrderQty (uint32_t val)
    {
        mOrderQty = val;
        return true;
    }
    
    string getPreventParticipantMatch () const
    {
        return getString (mPreventParticipantMatch, sizeof (mPreventParticipantMatch));
    }

    bool setPreventParticipantMatch (const string& val)
    {
        return setString (mPreventParticipantMatch, val, sizeof (mPreventParticipantMatch));
    }
    
    uint32_t getCorrectedSize () const
    {
        return mCorrectedSize;
    }

    bool setCorrectedSize (uint32_t val)
    {
        mCorrectedSize = val;
        return true;
    }
    
    string getOrigClOrdID () const
    {
        return getString (mOrigClOrdID, sizeof (mOrigClOrdID));
    }

    bool setOrigClOrdID (const string& val)
    {
        return setString (mOrigClOrdID, val, sizeof (mOrigClOrdID));
    }
    
    uint32_t getLeavesQty () const
    {
        return mLeavesQty;
    }

    bool setLeavesQty (uint32_t val)
    {
        mLeavesQty = val;
        return true;
    }
    
    uint32_t getLastShares () const
    {
        return mLastShares;
    }

    bool setLastShares (uint32_t val)
    {
        mLastShares = val;
        return true;
    }
    
    uint64_t getDisplayPrice () const
    {
        return mDisplayPrice;
    }

    bool setDisplayPrice (uint64_t val)
    {
        mDisplayPrice = val;
        return true;
    }
    
    uint64_t getWorkingPrice () const
    {
        return mWorkingPrice;
    }

    bool setWorkingPrice (uint64_t val)
    {
        mWorkingPrice = val;
        return true;
    }
    
    string getBaseLiquidityIndicator () const
    {
        return getString (mBaseLiquidityIndicator, sizeof (mBaseLiquidityIndicator));
    }

    bool setBaseLiquidityIndicator (const string& val)
    {
        return setString (mBaseLiquidityIndicator, val, sizeof (mBaseLiquidityIndicator));
    }
    
    uint64_t getSecondaryOrderID () const
    {
        return mSecondaryOrderID;
    }

    bool setSecondaryOrderID (uint64_t val)
    {
        mSecondaryOrderID = val;
        return true;
    }
    
    string getContraCapacity () const
    {
        return getString (mContraCapacity, sizeof (mContraCapacity));
    }

    bool setContraCapacity (const string& val)
    {
        return setString (mContraCapacity, val, sizeof (mContraCapacity));
    }
    
    uint8_t getAttributedQuote () const
    {
        return mAttributedQuote;
    }

    bool setAttributedQuote (uint8_t val)
    {
        mAttributedQuote = val;
        return true;
    }
    
    uint64_t getBulkOrderIDs () const
    {
        return mBulkOrderIDs;
    }

    bool setBulkOrderIDs (uint64_t val)
    {
        mBulkOrderIDs = val;
        return true;
    }
    
    string getBulkRejectReasons () const
    {
        return getString (mBulkRejectReasons, sizeof (mBulkRejectReasons));
    }

    bool setBulkRejectReasons (const string& val)
    {
        return setString (mBulkRejectReasons, val, sizeof (mBulkRejectReasons));
    }
    
    string getPartyRole () const
    {
        return getString (mPartyRole, sizeof (mPartyRole));
    }

    bool setPartyRole (const string& val)
    {
        return setString (mPartyRole, val, sizeof (mPartyRole));
    }
    
    string getSubLiquidityIndicator () const
    {
        return getString (mSubLiquidityIndicator, sizeof (mSubLiquidityIndicator));
    }

    bool setSubLiquidityIndicator (const string& val)
    {
        return setString (mSubLiquidityIndicator, val, sizeof (mSubLiquidityIndicator));
    }
    
    uint16_t getTradeReportTypeReturn () const
    {
        return mTradeReportTypeReturn;
    }

    bool setTradeReportTypeReturn (uint16_t val)
    {
        mTradeReportTypeReturn = val;
        return true;
    }
    
    uint8_t getTradePublishIndReturn () const
    {
        return mTradePublishIndReturn;
    }

    bool setTradePublishIndReturn (uint8_t val)
    {
        mTradePublishIndReturn = val;
        return true;
    }
    
    string getText () const
    {
        return getString (mText, sizeof (mText));
    }

    bool setText (const string& val)
    {
        return setString (mText, val, sizeof (mText));
    }
    
    uint64_t getBidPx () const
    {
        return mBidPx;
    }

    bool setBidPx (uint64_t val)
    {
        mBidPx = val;
        return true;
    }
    
    uint64_t getLargeSize () const
    {
        return mLargeSize;
    }

    bool setLargeSize (uint64_t val)
    {
        mLargeSize = val;
        return true;
    }
    
    string getLastMkt () const
    {
        return getString (mLastMkt, sizeof (mLastMkt));
    }

    bool setLastMkt (const string& val)
    {
        return setString (mLastMkt, val, sizeof (mLastMkt));
    }
    
    string getFeeCode () const
    {
        return getString (mFeeCode, sizeof (mFeeCode));
    }

    bool setFeeCode (const string& val)
    {
        return setString (mFeeCode, val, sizeof (mFeeCode));
    }
    
    string getEchoText () const
    {
        return getString (mEchoText, sizeof (mEchoText));
    }

    bool setEchoText (const string& val)
    {
        return setString (mEchoText, val, sizeof (mEchoText));
    }
    
    uint64_t getStopPx () const
    {
        return mStopPx;
    }

    bool setStopPx (uint64_t val)
    {
        mStopPx = val;
        return true;
    }
    
    string getRoutingInst () const
    {
        return getString (mRoutingInst, sizeof (mRoutingInst));
    }

    bool setRoutingInst (const string& val)
    {
        return setString (mRoutingInst, val, sizeof (mRoutingInst));
    }
    
    string getRoutStrategy () const
    {
        return getString (mRoutStrategy, sizeof (mRoutStrategy));
    }

    bool setRoutStrategy (const string& val)
    {
        return setString (mRoutStrategy, val, sizeof (mRoutStrategy));
    }
    
    string getExDestination () const
    {
        return getString (mExDestination, sizeof (mExDestination));
    }

    bool setExDestination (const string& val)
    {
        return setString (mExDestination, val, sizeof (mExDestination));
    }
    
    string getTradeReportRefID () const
    {
        return getString (mTradeReportRefID, sizeof (mTradeReportRefID));
    }

    bool setTradeReportRefID (const string& val)
    {
        return setString (mTradeReportRefID, val, sizeof (mTradeReportRefID));
    }
    
    string getMarketingFeeCode () const
    {
        return getString (mMarketingFeeCode, sizeof (mMarketingFeeCode));
    }

    bool setMarketingFeeCode (const string& val)
    {
        return setString (mMarketingFeeCode, val, sizeof (mMarketingFeeCode));
    }
    
    string getTargetPartyID () const
    {
        return getString (mTargetPartyID, sizeof (mTargetPartyID));
    }

    bool setTargetPartyID (const string& val)
    {
        return setString (mTargetPartyID, val, sizeof (mTargetPartyID));
    }
    
    uint64_t getAuctionID () const
    {
        return mAuctionID;
    }

    bool setAuctionID (uint64_t val)
    {
        mAuctionID = val;
        return true;
    }
    
    uint8_t getOrderCategory () const
    {
        return mOrderCategory;
    }

    bool setOrderCategory (uint8_t val)
    {
        mOrderCategory = val;
        return true;
    }
    
    string getLiquidityProvision () const
    {
        return getString (mLiquidityProvision, sizeof (mLiquidityProvision));
    }

    bool setLiquidityProvision (const string& val)
    {
        return setString (mLiquidityProvision, val, sizeof (mLiquidityProvision));
    }
    
    uint32_t getCmtaNumber () const
    {
        return mCmtaNumber;
    }

    bool setCmtaNumber (uint32_t val)
    {
        mCmtaNumber = val;
        return true;
    }
    
    string getCrossType () const
    {
        return getString (mCrossType, sizeof (mCrossType));
    }

    bool setCrossType (const string& val)
    {
        return setString (mCrossType, val, sizeof (mCrossType));
    }
    
    string getCrossPrioritization () const
    {
        return getString (mCrossPrioritization, sizeof (mCrossPrioritization));
    }

    bool setCrossPrioritization (const string& val)
    {
        return setString (mCrossPrioritization, val, sizeof (mCrossPrioritization));
    }
    
    uint32_t getAllocQty () const
    {
        return mAllocQty;
    }

    bool setAllocQty (uint32_t val)
    {
        mAllocQty = val;
        return true;
    }
    
    string getRoutingFirmID () const
    {
        return getString (mRoutingFirmID, sizeof (mRoutingFirmID));
    }

    bool setRoutingFirmID (const string& val)
    {
        return setString (mRoutingFirmID, val, sizeof (mRoutingFirmID));
    }
    
    string getWaiverType () const
    {
        return getString (mWaiverType, sizeof (mWaiverType));
    }

    bool setWaiverType (const string& val)
    {
        return setString (mWaiverType, val, sizeof (mWaiverType));
    }
    
    string getCrossExclusionIndicator () const
    {
        return getString (mCrossExclusionIndicator, sizeof (mCrossExclusionIndicator));
    }

    bool setCrossExclusionIndicator (const string& val)
    {
        return setString (mCrossExclusionIndicator, val, sizeof (mCrossExclusionIndicator));
    }
    
    string getPriceFormation () const
    {
        return getString (mPriceFormation, sizeof (mPriceFormation));
    }

    bool setPriceFormation (const string& val)
    {
        return setString (mPriceFormation, val, sizeof (mPriceFormation));
    }
    
    uint8_t getClientQualifiedRole () const
    {
        return mClientQualifiedRole;
    }

    bool setClientQualifiedRole (uint8_t val)
    {
        mClientQualifiedRole = val;
        return true;
    }
    
    uint32_t getClientID () const
    {
        return mClientID;
    }

    bool setClientID (uint32_t val)
    {
        mClientID = val;
        return true;
    }
    
    uint32_t getInvestorID () const
    {
        return mInvestorID;
    }

    bool setInvestorID (uint32_t val)
    {
        mInvestorID = val;
        return true;
    }
    
    uint32_t getExecutorID () const
    {
        return mExecutorID;
    }

    bool setExecutorID (uint32_t val)
    {
        mExecutorID = val;
        return true;
    }
    
    string getOrderOrigination () const
    {
        return getString (mOrderOrigination, sizeof (mOrderOrigination));
    }

    bool setOrderOrigination (const string& val)
    {
        return setString (mOrderOrigination, val, sizeof (mOrderOrigination));
    }
    
    string getAlgorithmicIndicator () const
    {
        return getString (mAlgorithmicIndicator, sizeof (mAlgorithmicIndicator));
    }

    bool setAlgorithmicIndicator (const string& val)
    {
        return setString (mAlgorithmicIndicator, val, sizeof (mAlgorithmicIndicator));
    }
    
    string getDeferralReason () const
    {
        return getString (mDeferralReason, sizeof (mDeferralReason));
    }

    bool setDeferralReason (const string& val)
    {
        return setString (mDeferralReason, val, sizeof (mDeferralReason));
    }
    
    uint8_t getInvestorQualifiedRole () const
    {
        return mInvestorQualifiedRole;
    }

    bool setInvestorQualifiedRole (uint8_t val)
    {
        mInvestorQualifiedRole = val;
        return true;
    }
    
    uint8_t getExecutorQualifiedRole () const
    {
        return mExecutorQualifiedRole;
    }

    bool setExecutorQualifiedRole (uint8_t val)
    {
        mExecutorQualifiedRole = val;
        return true;
    }
    
    string getCtiCode () const
    {
        return getString (mCtiCode, sizeof (mCtiCode));
    }

    bool setCtiCode (const string& val)
    {
        return setString (mCtiCode, val, sizeof (mCtiCode));
    }
    
    uint8_t getManualOrderIndicator () const
    {
        return mManualOrderIndicator;
    }

    bool setManualOrderIndicator (uint8_t val)
    {
        mManualOrderIndicator = val;
        return true;
    }
    
    uint64_t getTradeDate () const
    {
        return mTradeDate;
    }

    bool setTradeDate (uint64_t val)
    {
        mTradeDate = val;
        return true;
    }
    
    uint8_t getVariancePrice () const
    {
        return mVariancePrice;
    }

    bool setVariancePrice (uint8_t val)
    {
        mVariancePrice = val;
        return true;
    }
    
    uint64_t getVarianceSize () const
    {
        return mVarianceSize;
    }

    bool setVarianceSize (uint64_t val)
    {
        mVarianceSize = val;
        return true;
    }
    
    uint8_t getOrigTASPrice () const
    {
        return mOrigTASPrice;
    }

    bool setOrigTASPrice (uint8_t val)
    {
        mOrigTASPrice = val;
        return true;
    }
    
    uint8_t getCumQty () const
    {
        return mCumQty;
    }

    bool setCumQty (uint8_t val)
    {
        mCumQty = val;
        return true;
    }
    
    uint8_t getDayOrderQty () const
    {
        return mDayOrderQty;
    }

    bool setDayOrderQty (uint8_t val)
    {
        mDayOrderQty = val;
        return true;
    }
    
    uint8_t getDayCumQty () const
    {
        return mDayCumQty;
    }

    bool setDayCumQty (uint8_t val)
    {
        mDayCumQty = val;
        return true;
    }
    
    uint64_t getAvgPx () const
    {
        return mAvgPx;
    }

    bool setAvgPx (uint64_t val)
    {
        mAvgPx = val;
        return true;
    }
    
    uint64_t getDayAvgPx () const
    {
        return mDayAvgPx;
    }

    bool setDayAvgPx (uint64_t val)
    {
        mDayAvgPx = val;
        return true;
    }
    
    string getLegCFICode () const
    {
        return getString (mLegCFICode, sizeof (mLegCFICode));
    }

    bool setLegCFICode (const string& val)
    {
        return setString (mLegCFICode, val, sizeof (mLegCFICode));
    }
    
    uint64_t getLegMaturityDate () const
    {
        return mLegMaturityDate;
    }

    bool setLegMaturityDate (uint64_t val)
    {
        mLegMaturityDate = val;
        return true;
    }
    
    uint8_t getLegStrikePrice () const
    {
        return mLegStrikePrice;
    }

    bool setLegStrikePrice (uint8_t val)
    {
        mLegStrikePrice = val;
        return true;
    }
    
    uint32_t getSecondaryExecID () const
    {
        return mSecondaryExecID;
    }

    bool setSecondaryExecID (uint32_t val)
    {
        mSecondaryExecID = val;
        return true;
    }
    
    string getUsername () const
    {
        return getString (mUsername, sizeof (mUsername));
    }

    bool setUsername (const string& val)
    {
        return setString (mUsername, val, sizeof (mUsername));
    }
    
    string getTradeReportingIndicator () const
    {
        return getString (mTradeReportingIndicator, sizeof (mTradeReportingIndicator));
    }

    bool setTradeReportingIndicator (const string& val)
    {
        return setString (mTradeReportingIndicator, val, sizeof (mTradeReportingIndicator));
    }
    
    string getTradePublishIndicator () const
    {
        return getString (mTradePublishIndicator, sizeof (mTradePublishIndicator));
    }

    bool setTradePublishIndicator (const string& val)
    {
        return setString (mTradePublishIndicator, val, sizeof (mTradePublishIndicator));
    }
    
    uint8_t getReportTime () const
    {
        return mReportTime;
    }

    bool setReportTime (uint8_t val)
    {
        mReportTime = val;
        return true;
    }
    
    uint8_t getNoSides () const
    {
        return mNoSides;
    }

    bool setNoSides (uint8_t val)
    {
        mNoSides = val;
        return true;
    }
});

} // namespace neueda

#endif
