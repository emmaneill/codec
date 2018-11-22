@require(venue, generation_date, messages)
/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated @{generation_date}
 */
#ifndef @venue.upper()_CODEC_H
#define @venue.upper()_CODEC_H

#include <stdint.h>
#include "cdr.h"
#include "codec.h"
#include "codecHelpers.h"
#include "orderMsgBits.h"
#include "newOrderBits.h"
#include "cancelOrderBits.h"
#include "modifyOrderBits.h"
#include "tradeCaptureBits.h"
#include "packets/BoeHeaderPacket.h"
#include "packets/BoePackets.h"

#define MIN_MSG_SIZE 10
#define NEWORDER_BITFIELDS_SIZE 8
#define CANCELORDER_BITFIELDS_SIZE 2
#define MODIFYORDER_BITFIELDS_SIZE 2
#define TRADECAPTUREREPORT_BITFIELDS_SIZE 5
#define ORDERACKNOWLEDGEMENT_BITFIELDS_SIZE 15
#define ORDERREJECTED_BITFIELDS_SIZE 15
#define ORDERMODIFIED_BITFIELDS_SIZE 15
#define ORDERRESTATED_BITFIELDS_SIZE 15
#define USERMODIFYREJECTED_BITFIELDS_SIZE 15
#define ORDERCANCELLED_BITFIELDS_SIZE 15
#define CANCELREJECTED_BITFIELDS_SIZE 15
#define ORDEREXECUTION_BITFIELDS_SIZE 15
#define TRADECANCELCORRECT_BITFIELDS_SIZE 15
#define TRADECAPTUREREPORTACKNOWLEDGEMENT_BITFIELDS_SIZE 15
#define TRADECAPTUREREPORTREJECT_BITFIELDS_SIZE 15
#define TRADECAPTURECONFIRM_BITFIELDS_SIZE 15
#define TRADECAPTUREREPORTDECLINE_BITFIELDS_SIZE 15

namespace neueda
{

class @{venue}Codec: public codec
{
    public:
        @{venue}Codec()
        {
@for msg in messages:
            mMsgTypes.insert(std::make_pair("@{msg['id']}", "@{msg['name']}"));
@end
        }

        codecState decode (cdr& d,
                             const void* buf,
                             size_t len,
                             size_t& used);
        codecState encode (const cdr& d,
                             void* buf,
                             size_t len,
                             size_t& used);
        std::string getType () const { return "@{venue}"; }

        std::string getStringField(char* buf, int len, size_t& pos)
        {
            std::string c = "";
            c.assign((const char*)buf+pos, len);
            return c;
        }

    private:
@for msg in messages:
        codecState get@{msg['name']} (cdr& d, const void* buf, size_t& used);
@end

@for msg in messages:
        codecState put@{msg['name']} (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
@end


        OrderExecutionBits* mOrderExecutionBits;
        OrderModifiedBits* mOrderModifiedBits;
        OrderRejectedBits* mOrderRejectedBits;
        OrderRestatedBits* mOrderRestatedBits;
        OrderAcknowledgementBits* mOrderAcknowledgementBits;
        OrderCancelledBits* mOrderCancelledBits;
        UserModifyRejectedBits* mUserModifyRejectedBits;
        CancelRejectedBits* mCancelRejectedBits;
        TradeCancelCorrectBits* mTradeCancelCorrectBits;
        TradeCaptureReportRejectBits* mTradeCaptureReportRejectBits;
        TradeCaptureConfirmBits* mTradeCaptureConfirmBits;
        TradeCaptureReportDeclineBits* mTradeCaptureReportDeclineBits;
        TradeCaptureReportAcknowledgementBits* mTradeCaptureReportAcknowledgementBits;
        NewOrderBits* mNewOrderBits;
        CancelOrderBits* mCancelOrderBits;
        ModifyOrderBits* mModifyOrderBits;
        TradeCaptureReportBits* mTradeCaptureReportBits;


};

} // namespace neueda

#endif /* @venue.upper()_CODEC_H */
