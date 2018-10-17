/*
 * Copyright 2014-2018 Neueda
 */

#ifndef MODIFY_ORDER_BITS_H
#define MODIFY_ORDER_BITS_H

        struct ModifyOrderBits
        {
            //First byte
            unsigned int hasClearingFirm : 1;
            unsigned int hasReserved : 1;
            unsigned int hasOrderQty : 1;
            unsigned int hasPrice : 1;
            unsigned int hasOrdType : 1;
            unsigned int hasCancelOrigOnReject : 1;
            unsigned int hasExecInst : 1;
            unsigned int hasSide : 1;
            //Second byte
            unsigned int hasMaxFloor : 1;
            unsigned int hasStopPx : 1;
            unsigned int hasRoutingFirmID : 1;
            unsigned int hasManualOrderIndicator : 1;
            unsigned int hasOperatorId : 1;
            unsigned int hasReserved2 : 1;
            unsigned int hasReserved3 : 1;
            unsigned int hasReserved4 : 1;
        };

#endif /* MODIFY_ORDER_BITS_H */

