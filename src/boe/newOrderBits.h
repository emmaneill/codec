/*
 * Copyright 2014-2018 Neueda
 */

#ifndef NEW_ORDER_BITS_H
#define NEW_ORDER_BITS_H

        struct NewOrderBits
        {
            //First byte
            unsigned int hasClearingFirm : 1;
            unsigned int hasClearingAccount : 1;
            unsigned int hasPrice : 1;
            unsigned int hasExecInst : 1;
            unsigned int hasOrdType : 1;
            unsigned int hasTimeInForce : 1;
            unsigned int hasMinQty : 1;
            unsigned int hasMaxFloor : 1;
            //Second byte
            unsigned int hasSymbol : 1;
            unsigned int hasSymbolSfx : 1;
            unsigned int hasCurrency : 1;
            unsigned int hasIDSource : 1;
            unsigned int hasSecurityID : 1;
            unsigned int hasSecurityExchange : 1;
            unsigned int hasCapacity : 1;
            unsigned int hasRoutingInst : 1;
            //Third byte
            unsigned int hasAccount : 1;
            unsigned int hasDisplayIndicator : 1;
            unsigned int hasMaxRemovePct : 1;
            unsigned int hasDiscretionAmount : 1;
            unsigned int hasPegDifference : 1;
            unsigned int hasPreventParticipantMatch : 1;
            unsigned int hasLocateRequired : 1;
            unsigned int hasExpireTime : 1;
            //Fourth byte
            unsigned int hasMaturityDate : 1;
            unsigned int hasStrikePrice : 1;
            unsigned int hasPutOrCall : 1;
            unsigned int hasRiskReset : 1;
            unsigned int hasOpenClose : 1;
            unsigned int hasCMTANumber : 1;
            unsigned int hasTargetPartyID : 1;
            unsigned int hasLiquidityProvision : 1;
            //Fifth byte
            unsigned int hasReserved : 1;
            unsigned int hasAttributedQuote : 1;
            unsigned int hasBookingType : 1;
            unsigned int hasExtExecInst : 1;
            unsigned int hasClientID : 1;
            unsigned int hasInvestorID : 1;
            unsigned int hasExecutorID : 1;
            unsigned int hasOrderOrigination : 1;
             //Sixth byte
            unsigned int hasDisplayRange : 1;
            unsigned int hasStopPx : 1;
            unsigned int hasRoutStrategy : 1;
            unsigned int hasRouteDeliveryMethod : 1;
            unsigned int hasExDestination : 1;
            unsigned int hasEchoText : 1;
            unsigned int hasAuctionId : 1;
            unsigned int hasRoutingFirmID : 1;
            //Seventh byte
            unsigned int hasAlgorithmicIndicator : 1;
            unsigned int hasCustomGroupId : 1;
            unsigned int hasClientQualifiedRole : 1;
            unsigned int hasInvestorQualifiedRole : 1;
            unsigned int hasExecutorQualifiedRole : 1;
            unsigned int hasCtiCode : 1;
            unsigned int hasManualOrderIndicator : 1;
            unsigned int hasOperatorId : 1;
            //Eighth byte
            unsigned int hasQuoteRoomId : 1;
            unsigned int hasSIIndicator : 1;
            unsigned int hasReserved2 : 1;
            unsigned int hasReserved3 : 1;
            unsigned int hasRserved4 : 1;
            unsigned int hasReserved5 : 1;
            unsigned int hasReserved6 : 1;
            unsigned int hasReserved7 : 1;
        };

#endif /* NEW_ORDER_BITS_H */

