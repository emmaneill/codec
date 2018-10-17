/*
 * Copyright 2014-2018 Neueda
 */

#ifndef CANCEL_ORDER_BITS_H
#define CANCEL_ORDER_BITS_H

        struct CancelOrderBits
        {
            //First byte
            unsigned int hasClearingFirm : 1;
            unsigned int hasMassCancelLockout : 1;
            unsigned int hasMassCancel : 1;
            unsigned int hasOsiRoot : 1;
            unsigned int hasMassCancelId : 1;
            unsigned int hasRoutingFirmID : 1;
            unsigned int hasManualOrderIndicator : 1;
            unsigned int hasOperatorId : 1;
            //Second byte
            unsigned int hasMassCancelInst : 1;
            unsigned int hasReserved : 1;
            unsigned int hasReserved2 : 1;
            unsigned int hasReserved3 : 1;
            unsigned int hasReserved4 : 1;
            unsigned int hasReserved5 : 1;
            unsigned int hasReserved6 : 1;
            unsigned int hasReserved7 : 1;
        };

#endif /* CANCEL_ORDER_BITS_H */

