@require(Venue, venue, generation_date, messages, msg_data, types, get_field, sets, set_field)
/*
 * Copyright 2014-2018 Neueda Ltd.
 *
 * Generated @{generation_date}
 */
#include "@{venue}Codec.h"
#include "@{Venue}Packets.h"
#include "fields.h"
#include "codecHelpers.h"
#include <sstream>


namespace neueda
{

extern "C" codec*
getCodec ()
{
    return new @{venue}Codec ();
}

@for msg in messages:
codecState
@{venue}Codec::get@{msg['name']} (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "@{msg['name']}");
    size_t offset = sizeof (@{Venue}HeaderPacket);
@if len(msg_data[msg['name']]) > 0:
    @{venue}@{msg['name']}Packet* packet = (@{venue}@{msg['name']}Packet*)((char*)buf + offset);
@end
@for field in msg_data[msg['name']]:
@get_field(field)
@end
@if 'Login' in msg['name']:
@if 'LoginResponse' in msg['name']:
    //hardcode memory location as packets set incorrectly
    uint8_t NumParamGroups = *(uint8_t*)(buf + offset);
    d.setInteger (NumberOfParamGroups, (NumParamGroups));
    offset += sizeof (uint8_t);
    @else:
    int NumParamGroups = packet->getNumberOfParamGroups ();
@end
    cdrArray ParamGroupsArray;
    for (int i = 0; i < NumParamGroups; i++)
    {
        cdr item;
        BoeParamGroupsPacket* paramGroupsElement = (BoeParamGroupsPacket*) ((char*)buf + offset);

        item.setInteger(ParamGroupLength, paramGroupsElement->getParamGroupLength ());
        offset += sizeof (uint16_t);

        item.setInteger(ParamGroupType, paramGroupsElement->getParamGroupType ());
        offset += sizeof (uint8_t);

        if(paramGroupsElement->getParamGroupType () == 0x80)
        {
            BoeUnitSequencePacket* unitSequenceElement = (BoeUnitSequencePacket*) ((char*)buf + offset);
            offset += sizeof (BoeUnitSequencePacket);

            cdrArray UnitSequenceArray;
            item.setInteger(NoUnspecifiedUnitReplay, unitSequenceElement->getNoUnspecifiedUnitReplay ());

            int NumUnits = unitSequenceElement->getNumOfUnits ();
            item.setInteger(NumOfUnits, unitSequenceElement->getNumOfUnits ());

            cdrArray UnitGroupArray;
            for (int i = 0; i < NumUnits; i++)
            {
                cdr unitGroupItem;
                BoeUnitGroupPacket* unitGroupElement = (BoeUnitGroupPacket*) ((char*)buf + offset);

                unitGroupItem.setInteger (UnitNumber, unitGroupElement->getUnitNumber ());
                offset += sizeof (uint8_t);

                unitGroupItem.setInteger (UnitSequence, unitGroupElement->getUnitSequence ());
                offset += sizeof (uint32_t);

                UnitGroupArray.push_back (unitGroupItem);
            }
            item.setArray (UnitGroupSection, UnitGroupArray);
        }
        else if(paramGroupsElement->getParamGroupType () == 0x81)
        {
            BoeReturnBitfieldPacket* returnBitfieldElement = (BoeReturnBitfieldPacket*) ((char*)buf + offset);

            item.setInteger(MessageType, returnBitfieldElement->getMessageType ());
            offset += sizeof (uint8_t);

            int NumReturnBitfields = returnBitfieldElement->getNumOfReturnBitfield ();
            item.setInteger(NumOfReturnBitfield, returnBitfieldElement->getNumOfReturnBitfield ());
            offset += sizeof (uint8_t);

            //copying bitfields to new temporary buffer
            char* tmpbuf = new char[NumReturnBitfields];
            memcpy ((void*)tmpbuf, (void*)buf+offset, NumReturnBitfields);

            cdrArray ReturnBitfieldArray;
            for (int i = 0; i < NumReturnBitfields; i++)
            {
                cdr bitItem;
                BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
                bitItem.setInteger (Bitfield, bitfieldElement->getBitfield ());
                offset += sizeof (BoeBitfieldPacket);

                ReturnBitfieldArray.push_back(bitItem);
            }
            item.setArray(BitfieldSection, ReturnBitfieldArray);
            
            if (NumReturnBitfields ==0)
            {
               used += offset;
               return GW_CODEC_SUCCESS;
            }
            
            switch (MessageType) {
            case 0x25:
                memset (tmpbuf+NumReturnBitfields, 0, (ORDERACKNOWLEDGEMENT_BITFIELDS_SIZE - NumReturnBitfields));
                mOrderAcknowledgementBits = (OrderAcknowledgementBits*) tmpbuf;
                if (mOrderAcknowledgementBits == NULL)
                {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
                }
                break;
            case 0x26:
                memset (tmpbuf+NumReturnBitfields, 0, (ORDERREJECTED_BITFIELDS_SIZE - NumReturnBitfields));
                mOrderRejectedBits = (OrderRejectedBits*) tmpbuf;
                if (mOrderRejectedBits == NULL)
                {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
                }
                break;
            case 0x27:
                memset (tmpbuf+NumReturnBitfields, 0, (ORDERMODIFIED_BITFIELDS_SIZE - NumReturnBitfields));
                mOrderModifiedBits = (OrderModifiedBits*) tmpbuf;
                if (mOrderModifiedBits == NULL)
                {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
                }
                break;
            case 0x28:
                memset (tmpbuf+NumReturnBitfields, 0, (ORDERRESTATED_BITFIELDS_SIZE - NumReturnBitfields));
                mOrderRestatedBits = (OrderRestatedBits*) tmpbuf;
                if (mOrderRestatedBits == NULL)
                {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
                }
                break;
            case 0x29:
                memset (tmpbuf+NumReturnBitfields, 0, (USERMODIFYREJECTED_BITFIELDS_SIZE - NumReturnBitfields));
                mUserModifyRejectedBits = (UserModifyRejectedBits*) tmpbuf;
                if (mUserModifyRejectedBits == NULL)
                {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
                }
                break;
            case 0x2A:
                memset (tmpbuf+NumReturnBitfields, 0, (ORDERCANCELLED_BITFIELDS_SIZE - NumReturnBitfields));
                mOrderCancelledBits = (OrderCancelledBits*) tmpbuf;
                if (mOrderCancelledBits == NULL)
                {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
                }
                break;
            case 0x2B:
                memset (tmpbuf+NumReturnBitfields, 0, (CANCELREJECTED_BITFIELDS_SIZE - NumReturnBitfields));
                mCancelRejectedBits = (CancelRejectedBits*) tmpbuf;
                if (mCancelRejectedBits == NULL)
                {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
                }
                break;
            case 0x2C:
                memset (tmpbuf+NumReturnBitfields, 0, (ORDEREXECUTION_BITFIELDS_SIZE - NumReturnBitfields));
                mOrderExecutionBits = (OrderExecutionBits*) tmpbuf;
                if (mOrderExecutionBits == NULL)
                {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
                }
                break;
            case 0x2D:
                memset (tmpbuf+NumReturnBitfields, 0, (TRADECANCELCORRECT_BITFIELDS_SIZE - NumReturnBitfields));
                mTradeCancelCorrectBits = (TradeCancelCorrectBits*) tmpbuf;
                if (mTradeCancelCorrectBits == NULL)
                {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
                }
                break;
            case 0x30:
                memset (tmpbuf+NumReturnBitfields, 0, (TRADECAPTUREREPORTACKNOWLEDGEMENT_BITFIELDS_SIZE - NumReturnBitfields));
                mTradeCaptureReportAcknowledgementBits = (TradeCaptureReportAcknowledgementBits*) tmpbuf;
                if (mTradeCaptureReportAcknowledgementBits == NULL)
                {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
                }
                break;
            case 0x31:
                memset (tmpbuf+NumReturnBitfields, 0, (TRADECAPTUREREPORTREJECT_BITFIELDS_SIZE - NumReturnBitfields));
                mTradeCaptureReportRejectBits = (TradeCaptureReportRejectBits*) tmpbuf;
                if (mTradeCaptureReportRejectBits == NULL)
                {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
                }
                break;
            case 0x32:
                memset (tmpbuf+NumReturnBitfields, 0, (TRADECAPTURECONFIRM_BITFIELDS_SIZE - NumReturnBitfields));
                mTradeCaptureConfirmBits = (TradeCaptureConfirmBits*) tmpbuf;
                if (mTradeCaptureConfirmBits == NULL)
                {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
                }
                break;
            case 0x33:
                memset (tmpbuf+NumReturnBitfields, 0, (TRADECAPTUREREPORTDECLINE_BITFIELDS_SIZE - NumReturnBitfields));
                mTradeCaptureReportDeclineBits = (TradeCaptureReportDeclineBits*) tmpbuf;
                if (mTradeCaptureReportDeclineBits == NULL)
                {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
                }
                break;
            }
        }
        ParamGroupsArray.push_back(item);
    }
    d.setArray(ParamGroupSection, ParamGroupsArray);
    @end
    used += offset;
    return GW_CODEC_SUCCESS;
}
@end


@for msg in messages:
codecState
@{venue}Codec::put@{msg['name']} (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (@{Venue}HeaderPacket);
    @if len(msg_data[msg['name']]) > 0:
    @{venue}@{msg['name']}Packet* packet = (@{venue}@{msg['name']}Packet*)((char*)buf + offset);
    @end

    char *tmpBuf = (char *)buf;

    if (len < sizeof (@{venue}@{msg['name']}Packet))
        return GW_CODEC_SHORT;

    @for field in msg_data[msg['name']]:
    @set_field(field)
    @end
    @if 'Login' in msg['name']:
        @if 'LoginResponse' in msg['name']:
        //hardcoded due to memory assignement with packets
        uint8_t numberofparamgroups;
        if (!d.getInteger (NumberOfParamGroups, numberofparamgroups))
        {
            setLastError ("NumberOfParamGroups missing or not integer");
            return GW_CODEC_ERROR;
        }
        memcpy (tmpBuf + offset, &numberofparamgroups, sizeof(uint8_t));
        offset += sizeof (uint8_t);
        @end

    if (d.getArraySize (ParamGroupSection) > 0)
    {
        cdrArray* ParamGroupsArray = NULL;
        d.getArray (ParamGroupSection, (const cdrArray**)(&ParamGroupsArray));
        for (cdrArray::iterator it = ParamGroupsArray->begin(); it != ParamGroupsArray->end(); ++it)
        {
            cdr& item = *it;
            BoeParamGroupsPacket* paramGroupsElement = (BoeParamGroupsPacket*) ((char*)buf + offset);

            uint16_t paramGroupLength;
            if (!item.getInteger (ParamGroupLength, paramGroupLength))
            {
                setLastError ("ParamGroupLength missing or not integer");
                return GW_CODEC_ERROR;
            }
            paramGroupsElement->setParamGroupLength (paramGroupLength);
            offset += sizeof (uint16_t);

            uint8_t paramGroupType;
            if (!item.getInteger (ParamGroupType, paramGroupType))
            {
                setLastError ("ParamGroupType missing or not integer");
                return GW_CODEC_ERROR;
            }
            paramGroupsElement->setParamGroupType (paramGroupType);
            offset += sizeof (uint8_t);

            if(paramGroupType == 0x80)
            {
                BoeUnitSequencePacket* unitSequenceElement = (BoeUnitSequencePacket*) ((char*)buf + offset);
                cdrArray UnitSequenceParamGroupArray;

                uint8_t noUnspecifiedUnitReplay;
                if (!item.getInteger (NoUnspecifiedUnitReplay, noUnspecifiedUnitReplay))
                {
                    setLastError ("NoUnspecifiedUnitReplay missing or not integer");
                    return GW_CODEC_ERROR;
                }
                unitSequenceElement->setNoUnspecifiedUnitReplay (noUnspecifiedUnitReplay);
                offset += sizeof (uint8_t);

                uint8_t numOfUnits;
                if (!item.getInteger (NumOfUnits, numOfUnits))
                {
                    setLastError ("NumOfUnits missing or not integer");
                    return GW_CODEC_ERROR;
                }
                unitSequenceElement->setNumOfUnits (numOfUnits);
                offset += sizeof (uint8_t);

                cdrArray* UnitGroupArray = NULL;
                item.getArray (UnitGroupSection, (const cdrArray**)(&UnitGroupArray));
                if(item.getArraySize(UnitGroupSection) > 0)
                {
                    for (cdrArray::iterator it = UnitGroupArray->begin(); it != UnitGroupArray->end(); ++it)
                    {
                        cdr& unitGroupItem = *it;
                        BoeUnitGroupPacket* unitGroupElement = (BoeUnitGroupPacket*) ((char*)buf + offset);

                        uint8_t unitNumber;
                        if (!unitGroupItem.getInteger (UnitNumber, unitNumber))
                        {
                            setLastError ("UnitNumber missing or not integer");
                            return GW_CODEC_ERROR;
                        }
                        unitGroupElement->setUnitNumber (unitNumber);
                        offset += sizeof (uint8_t);

                        uint64_t unitSequence;
                        if (!unitGroupItem.getInteger (UnitSequence, unitSequence))
                        {
                            setLastError ("UnitSequence missing or not integer");
                            return GW_CODEC_ERROR;
                        }
                        unitGroupElement->setUnitSequence (unitSequence);
                        offset += sizeof (uint32_t);
                    }
                }
            }
            else if(paramGroupType == 0x81)
            {
                BoeReturnBitfieldPacket* returnBitfieldElement = (BoeReturnBitfieldPacket*) ((char*)buf + offset);

                cdrArray ReturnBitfieldArray;

                uint8_t messageType;
                if (!item.getInteger (MessageType, messageType))
                {
                    setLastError ("MessageType missing or not integer");
                    return GW_CODEC_ERROR;
                }
                returnBitfieldElement->setMessageType (messageType);
                offset += sizeof (uint8_t);

                uint8_t numReturnBitfields;
                if (!item.getInteger (NumOfReturnBitfield, numReturnBitfields))
                {
                    setLastError ("NumReturnBitfields missing or not integer");
                    return GW_CODEC_ERROR;
                }
                returnBitfieldElement->setNumOfReturnBitfield (numReturnBitfields);
                offset += sizeof (uint8_t);

                cdrArray* BitfieldArray = NULL;
                item.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
                if(item.getArraySize(BitfieldSection) > 0)
                {
                    for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
                    {
                        cdr& bitItem = *it;
                        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
                        uint8_t returnBitfield;
                        if (!bitItem.getInteger (Bitfield, returnBitfield))
                        {
                            setLastError ("Bitfield missing or not integer");
                            return GW_CODEC_ERROR;
                        }
                        bitfieldElement->setBitfield (returnBitfield);
                        offset += sizeof (uint8_t);
                    }
                }
                //TODO - set Optional fields
            }
        }
    }

    @end
    used += offset;

    return GW_CODEC_SUCCESS;
}

@end

codecState
@{venue}Codec::decode (cdr& d, const void* buf, size_t len, size_t& used)
{

    clearLastError ();
    used = 0;
    d.clear ();

    if (len < sizeof(@{Venue}HeaderPacket))
        return GW_CODEC_SHORT;

    @{Venue}HeaderPacket* hdr = (@{Venue}HeaderPacket*)((unsigned char*)buf);

    if (len < sizeof hdr)
        return GW_CODEC_SHORT;

    d.setInteger (MessageType,  hdr->getMessageType ());
    d.setInteger (MessageLength, hdr->getMessageLength ());
    d.setInteger (MatchingUnit, hdr->getMatchingUnit ());
    d.setInteger (SequenceNumber, hdr->getSequenceNumber ());

    switch (hdr->mMessageType) {
@for message in messages:
        case @{message['id']}:
            return get@{message['name']} (d, buf, used);
@end
        default:
            setLastError ("unknown message type");
            return GW_CODEC_ERROR;
    }
    return GW_CODEC_ERROR;
}

codecState
@{venue}Codec::encode (const cdr& d, void* buf, size_t len, size_t& used)
{
    clearLastError ();
    used = 0;

    @{Venue}HeaderPacket* hdr = (@{Venue}HeaderPacket*)((char*)buf);

    @{Venue}HeaderPacket base;
    memcpy (buf, &base, sizeof(base));

    if (len < sizeof(@{Venue}HeaderPacket))
        return GW_CODEC_SHORT;

    if (!d.isType (MessageType, CDR_INTEGER, 1))
    {
        setLastError ("MessageType missing or not string");
        return GW_CODEC_ERROR;
    }
    uint8_t type;
    d.getInteger (MessageType, type);
    hdr->setMessageType (type);

    uint8_t matchingUnit;
    if(!d.getInteger (MatchingUnit, matchingUnit))
    {
        setLastError("MatchingUnit is missing or not integer");
        return GW_CODEC_ERROR;
    }
    hdr->setMatchingUnit (matchingUnit);

    uint32_t sequenceNumber;
    if(!d.getInteger (SequenceNumber, sequenceNumber))
    {
        setLastError("SequenceNumber is missing or not integer");
        return GW_CODEC_ERROR;
    }
    hdr->setSequenceNumber (sequenceNumber);

    codecState state = GW_CODEC_ERROR;
    switch (type) {
@for message in messages:
        case @{message['id']}:
            state = put@{message['name']} (d, buf, len, used);
            break;
@end
        default:
            setLastError ("unknown message type");
            return GW_CODEC_ERROR;
    }
    //Message Length does not include first two bytes of message
    hdr->setMessageLength (used - 2);
    return state;
}

} // namespace neueda
