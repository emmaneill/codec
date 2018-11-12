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
#include "orderMsgBits.h"
#include "newOrderBits.h"
#include "cancelOrderBits.h"
#include "modifyOrderBits.h"
#include "tradeCaptureBits.h"


#define LOGIN_RESPONSE 0x24
#define LOGOUT 0x08
#define SERVER_HEARTBEAT 0x09
#define REPLAY_COMPLETE 0x13
#define ORDER_ACK 0x25
#define ORDER_REJECTED 0x26
#define ORDER_MODIFIED 0x27
#define ORDER_RESTATED 0x28
#define USER_MODIFY_REJECT 0x29
#define ORDER_CANCELLED 0x2A
#define ORDER_CANCEL_REJECTED 0x2B
#define ORDER_EXECUTION 0x2C
#define TRADE_CANCEL_CORRECTED 0x2D

#define LOGIN_REQUEST 0x37
#define CLIENT_HEARTBEAT 0x03
#define NEW_ORDER 0x38

#define MIN_MSG_SIZE 10
#define ORDER_MSG_BITFIELDS_SIZE 15
#define NEW_ORDER_BITFIELDS_SIZE 8
#define CANCEL_ORDER_BITFIELDS_SIZE 2
#define MODIFY_ORDER_BITFIELDS_SIZE 2
#define TRADE_CAPTURE_BITFIELDS_SIZE 5

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


        OrderMsgBits* mOrderExecutionBits;
        OrderMsgBits* mOrderModifiedBits;
        OrderMsgBits* mOrderRejectedBits;
        OrderMsgBits* mOrderRestatedBits;
        OrderMsgBits* mOrderAcknowledgementBits;
        OrderMsgBits* mOrderCancelledBits;
        OrderMsgBits* mUserModifyRejectedBits;
        OrderMsgBits* mCancelRejectedBits;
        NewOrderBits* mNewOrderBits;
        CancelOrderBits* mCancelOrderBits;
        ModifyOrderBits* mModifyOrderBits;
        TradeCaptureBits* mTradeCaptureReportBits;
        OrderMsgBits* mTradeCancelCorrectBits;
        OrderMsgBits* mTradeCaptureReportRejectBits;
        OrderMsgBits* mTradeCaptureConfirmBits;
        OrderMsgBits* mTradeCaptureReportDeclineBits;
        OrderMsgBits* mTradeCaptureReportAcknowledgementBits;


};

} // namespace neueda

#endif /* @venue.upper()_CODEC_H */
