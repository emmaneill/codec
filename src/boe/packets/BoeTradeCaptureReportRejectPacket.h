/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 15:21:30 26/10/18
 */
#ifndef _BOE_TRADECAPTUREREPORTREJECT_PACKET_H_
#define _BOE_TRADECAPTUREREPORTREJECT_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>
#include "boePacketUtils.h"
#include "boeHeaderPacket.h"


namespace neueda
{

PACKED(class boeTradeCaptureReportRejectPacket
{
public:
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
        mTransactTime = 0;
        memset (mTradeReportID, 0, 20);
        mTradeCaptureRejectReason = 0;
        memset (mTradeCaptureRejectText, 0, 60);
        mNumberOfBitfields = 0;
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
});

} // namespace neueda

#endif
