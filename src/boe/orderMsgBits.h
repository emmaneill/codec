/*
 * Copyright 2014-2018 Neueda
 */

#ifndef ORDER_MSG_BITS_H
#define ORDER_MSG_BITS_H

        //For use with  Order Execution, Acknowledgement, Rejected, Modified, Restated, UserModify Rejected, Cancelled
        struct OrderMsgBits
        {
            //First byte
            unsigned int hasSide : 1;
            unsigned int hasPegDifference : 1;
            unsigned int hasPrice : 1;
            unsigned int hasExecInst : 1;
            unsigned int hasOrdType : 1;
            unsigned int hasTimeInForce : 1;
            unsigned int hasMinQty : 1;
            unsigned int hasMaxRemovePct : 1;
            //Second byte
            unsigned int hasSymbol : 1;
            unsigned int hasSymbolSfx : 1;
            unsigned int hasCurrency : 1;
            unsigned int hasIDSource : 1;
            unsigned int hasSecurityID : 1;
            unsigned int hasSecurityExchange : 1;
            unsigned int hasCapacity : 1;
            unsigned int hasReserved : 1;
            //Third byte
            unsigned int hasAccount : 1;
            unsigned int hasClearingFirm : 1;
            unsigned int hasClearingAccount : 1;
            unsigned int hasDisplayIndicator : 1;
            unsigned int hasMaxFloor : 1;
            unsigned int hasDiscretionAmount : 1;
            unsigned int hasOrderQty : 1;
            unsigned int hasPreventParticipantMatch : 1;
            //Fourth byte
            unsigned int hasMaturityDate : 1;
            unsigned int hasStrikePrice : 1;
            unsigned int hasPutOrCall : 1;
            unsigned int hasOpenClose : 1;
            unsigned int hasClOrdIDBatch : 1;
            unsigned int hasCorrectedSize : 1;
            unsigned int hasPartyID : 1;
            unsigned int hasAccessFee : 1;
            //Fifth byte
            unsigned int hasOrigClOrdID : 1;
            unsigned int hasLeavesQty : 1;
            unsigned int hasLastShares : 1;
            unsigned int hasLastPrice : 1;
            unsigned int hasDisplayPrice : 1;
            unsigned int hasWorkingPrice : 1;
            unsigned int hasBaseLiquidityIndicator : 1;
            unsigned int hasExpireTime : 1;
            //Sixth byte
            unsigned int hasSecondaryOrderID : 1;
            unsigned int hasCcp : 1;
            unsigned int hasContraCapacity : 1;
            unsigned int hasAttributedQuote : 1;
            unsigned int hasExtExecInst : 1;
            unsigned int hasBulkOrderIDs : 1;
            unsigned int hasBulkRejectReasons : 1;
            unsigned int hasPartyRole : 1;
            //Seventh bytE
            unsigned int hasSubLiquidityIndicator : 1;
            unsigned int hasTradeReportTypeReturn : 1;
            unsigned int hasTradePublishIndReturn : 1;
            unsigned int hasText : 1;
            unsigned int hasBidPx : 1;
            unsigned int hasOffer : 1;
            unsigned int hasLargeSize : 1;
            unsigned int hasLastMkt : 1;
           //Eighth byte
            unsigned int hasFeeCode : 1;
            unsigned int hasEchoText : 1;
            unsigned int hasStopPx : 1;
            unsigned int hasRoutingInst : 1;
            unsigned int hasRoutStrategy : 1;
            unsigned int hasRouteDeliveryMethod : 1;
            unsigned int hasExDestination : 1;
            unsigned int hasTradeReportRefID : 1;
           //Ninth byte
            unsigned int hasMarketingFeeCode : 1;
            unsigned int hasTargetPartyID : 1;
            unsigned int hasAuctionID : 1;
            unsigned int hasOrderCategory : 1;
            unsigned int hasLiquidityProvision : 1;
            unsigned int hasCmtaNumber : 1;
            unsigned int hasCrossType : 1;
            unsigned int hasCrossPrioritization : 1;
            //Tenth byte
            unsigned int hasCrossID : 1;
            unsigned int hasAllocQty : 1;
            unsigned int hasGiveUpFirmID : 1;
            unsigned int hasRoutingFirmID : 1;
            unsigned int hasWaiverType : 1;
            unsigned int hasCrossExclusionIndicator : 1;
            unsigned int hasPriceFormation : 1;
            unsigned int hasClientQualifiedRole : 1;
            //Eleventh byTe
            unsigned int hasClientID : 1;
            unsigned int hasInvestorID : 1;
            unsigned int hasExecutorID : 1;
            unsigned int hasOrderOrigination : 1;
            unsigned int hasAlgorithmicIndicator : 1;
            unsigned int hasDeferralReason : 1;
            unsigned int hasInvestorQualifiedRole : 1;
            unsigned int hasExecutorQualifiedRole : 1;
            //Twelfth bytE
            unsigned int hasCtiCode : 1;
            unsigned int hasManualOrderIndicator : 1;
            unsigned int hasOperatorID : 1;
            unsigned int hasTradeDate : 1;
            unsigned int hasVariancePrice : 1;
            unsigned int hasVarianceSize : 1;
            unsigned int hasOrigSymbolID : 1;
            unsigned int hasOrigTASPrice : 1;
            //Thirteenth Byte
            unsigned int hasCumQty : 1;
            unsigned int hasDayOrderQty : 1;
            unsigned int hasDayCumQty : 1;
            unsigned int hasAvgPx : 1;
            unsigned int hasDayAvgPx : 1;
            unsigned int hasPendingStatus : 1;
            unsigned int hasDrillThruProtection : 1;
            unsigned int hasMultilegReportingType : 1;
            //Fourteenth Byte
            unsigned int hasLegCFICode : 1;
            unsigned int hasLegMaturityDate : 1;
            unsigned int hasLegStrikePrice : 1;
            unsigned int hasQuoteRoomID : 1;
            unsigned int hasSecondaryExecID : 1;
            unsigned int hasUserRequestID : 1;
            unsigned int hasUsername : 1;
            unsigned int hasUserStatus : 1;
            //Fifteenth bYte
            unsigned int hasTradeReportingIndicator : 1;
            unsigned int hasReserved1  : 1;
            unsigned int hasReserved2 : 1;
            unsigned int hasReserved3 : 1;
            unsigned int hasTradePublishIndicator : 1;
            unsigned int hasReportTime :1;
            unsigned int hasReserved4 :1;
            unsigned int hasReserved5 : 1;
        };

#endif /* ORDER_MSG_BITS_H */

