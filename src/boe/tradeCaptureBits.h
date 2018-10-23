/*
 * Copyright 2014-2018 Neueda
 */

#ifndef TRADE_CAPTURE_BITS_H
#define TRADE_CAPTURE_BITS_H

        struct TradeCaptureBits
        {
            //First byte
            unsigned int hasSymbol : 1;
            unsigned int hasReserved : 1;
            unsigned int hasCurrency : 1;
            unsigned int hasIDSource : 1;
            unsigned int hasSecurityID : 1;
            unsigned int hasSecurityExchange: 1;
            unsigned int hasExecInst : 1;
            unsigned int hasReserved2: 1;
            //Second byte
            unsigned int hasCapacity : 1;
            unsigned int hasAccount : 1;
            unsigned int hasTransactionCategory : 1;
            unsigned int hasTradeTime : 1;
            unsigned int hasPartyRole : 1;
            unsigned int hasTradeReportTransType : 1;
            unsigned int hasTradeID : 1;
            unsigned int hasVenueType : 1;
            //Third byte
            unsigned int hasTradingSessionSubID : 1;
            unsigned int hasMatchType : 1;
            unsigned int hasTrdSubType : 1;
            unsigned int hasSecondaryTrdType : 1;
            unsigned int hasTradePriceCondition : 1;
            unsigned int hasTradePublishIndicator : 1;
            unsigned int hasLargeSize : 1;
            unsigned int hasExecutionMethod : 1;
            //Fourth byte
            unsigned int hasTradeReportType : 1;
            unsigned int hasTradeHandlingInstruction : 1;
            unsigned int hasTradeLinkID : 1;
            unsigned int hasTradeReportRefID : 1;
            unsigned int hasGrossTradeAmt : 1;
            unsigned int hasTolerance : 1;
            unsigned int hasOrderCategory : 1;
            unsigned int hasSettlementPrice : 1;
            //Fifth byte
            unsigned int hasSettlementDate : 1;
            unsigned int hasPriceFormation : 1;
            unsigned int hasAlgorithmicIndicator : 1;
            unsigned int hasWaiverType : 1;
            unsigned int hasDeferralReason : 1;
            unsigned int hasSettlementCurrency : 1;
            unsigned int hasSettlementLocation: 1;
            unsigned int hasReserved3 : 1;
        };

#endif /* TRADE_CAPTURE_BITS_H */

