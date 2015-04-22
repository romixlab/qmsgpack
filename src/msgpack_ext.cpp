#include "msgpack_ext.h"
#include "msgpack_common.h"
#include "private/sysdep.h"

quint8 * MsgPack::Ext::unpack_upto_quint8(quint8 *to, quint8 *p, bool *success)
{
    if (*p <= MsgPack::FirstByte::POSITIVE_FIXINT) {
        *to = *(p++);
    } else if (*p == MsgPack::FirstByte::UINT8) {
        *to = *(++p);
    } else {
        *success = false;
        return p;
    }
    *success = true;
    return p;
}

quint8 *MsgPack::Ext::unpack_upto_quint16(quint16 *to, quint8 *p, bool *success)
{
    if (*p == MsgPack::FirstByte::UINT16) {
        p++;
        *to = _msgpack_load16(quint16, p);
        *success = true;
        return p + 2;
    } else {
        quint8 u8;
        p = unpack_upto_quint8(&u8, p, success);
        *to = u8;
        return p;
    }
}

quint8 *MsgPack::Ext::unpack_upto_quint32(quint32 *to, quint8 *p, bool *success)
{
    if (*p == MsgPack::FirstByte::UINT32) {
        p++;
        *to = _msgpack_load32(quint32, p);
        *success = true;
        return p + 4;
    } else {
        quint16 u16;
        p = unpack_upto_quint16(&u16, p, success);
        *to = u16;
        return p;
    }
}

quint8 *MsgPack::Ext::unpack_upto_quint64(quint64 *to, quint8 *p, bool *success)
{
    if (*p == MsgPack::FirstByte::UINT64) {
        p++;
        *to = _msgpack_load64(quint64, p);
        *success = true;
        return p + 8;
    } else {
        quint32 u32;
        p = unpack_upto_quint32(&u32, p, success);
        *to = u32;
        return p;
    }
}

quint8 *MsgPack::Ext::unpack_upto_qint8(qint8 *to, quint8 *p, bool *success)
{
    if (*p >= MsgPack::FirstByte::NEGATIVE_FIXINT) {
        *to = *p;
        *success = true;
        return p + 1;
    } else if (*p == MsgPack::FirstByte::INT8) {
        *to = (qint8) *(++p);
        *success = true;
        return p + 1;
    } else {
        quint8 u8;
        p = unpack_upto_quint8(&u8, p, success);
        *to = u8;
        return p;
    }
}

quint8 *MsgPack::Ext::unpack_upto_qint16(qint16 *to, quint8 *p, bool *success)
{
    if (*p == MsgPack::FirstByte::INT16) {
        p++;
        *to = _msgpack_load16(qint16, p);
        *success = true;
        return p + 2;
    } else {
        qint8 i8;
        p = unpack_upto_qint8(&i8, p, success);
        *to = i8;
        if (*success) {
            return p;
        } else {
            quint16 u16;
            p = unpack_upto_quint16(&u16, p, success);
            *to = u16;
            return p;
        }
    }
}

quint8 *MsgPack::Ext::unpack_upto_qint32(qint32 *to, quint8 *p, bool *success)
{
    if(*p == MsgPack::FirstByte::INT32) {
        p++;
        *to = _msgpack_load32(qint32, p);
        *success = true;
        return p + 4;
    } else {
        qint16 i16;
        p = unpack_upto_qint16(&i16, p, success);
        *to = i16;
        if (*success) {
            return p;
        } else {
            quint32 u32;
            p = unpack_upto_quint32(&u32, p, success);
            *to = u32;
            return p;
        }
    }
}

quint8 *MsgPack::Ext::unpack_upto_qint64(qint64 *to, quint8 *p, bool *success)
{
    if(*p == MsgPack::FirstByte::INT64) {
        p++;
        *to = _msgpack_load64(qint64, p);
        *success = true;
        return p + 8;
    } else {
        qint32 i32;
        p = unpack_upto_qint32(&i32, p, success);
        *to = i32;
        if (*success) {
            return p;
        } else {
            quint64 u64;
            p = unpack_upto_quint64(&u64, p, success);
            *to = u64;
            return p;
        }
    }
}
