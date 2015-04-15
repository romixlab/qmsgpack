#include "msgpack.h"
#include "msgpack_common.h"
#include "private/unpack_p.h"
#include "private/pack_p.h"
#include "private/qt_types_p.h"
#include "private/sysdep.h"

QVariant MsgPack::unpack(const QByteArray &data)
{
    quint8 *p = (quint8 *)data.data();
    quint8 *end = p + data.size() - 1;

    return MsgPackPrivate::unpack(p, end);
}

QByteArray MsgPack::pack(const QVariant &variant)
{
    quint8 *p = 0;
    quint8 *end = MsgPackPrivate::pack(variant, p, false);
    quint32 size = end - p;
    //qDebug() << "size probe:" << size;

    QByteArray arr;
    arr.resize(size);
    end = MsgPackPrivate::pack(variant, (quint8 *)arr.data(), true);

    return arr;
}

bool MsgPack::registerPacker(QMetaType::Type qType, qint8 msgpackType, MsgPack::pack_user_f packer)
{
    return MsgPackPrivate::register_packer(qType, msgpackType, packer);
}

bool MsgPack::registerUnpacker(qint8 msgpackType, MsgPack::unpack_user_f unpacker)
{
    return MsgPackPrivate::register_unpacker(msgpackType, unpacker);
}

bool MsgPack::registerType(QMetaType::Type qType, quint8 msgpackType)
{
    return MsgPackPrivate::register_qtype(qType, msgpackType);
}

void MsgPack::setCompatibilityModeEnabled(bool enabled)
{
    MsgPackPrivate::compatibilityMode = enabled;
}

quint8 * MsgPack::ExtHelpers::unpack_upto_quint8(quint8 *to, quint8 *p, bool *success)
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

quint8 *MsgPack::ExtHelpers::unpack_upto_quint16(quint16 *to, quint8 *p, bool *success)
{
    if (*p == MsgPack::FirstByte::UINT16) {
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

quint8 *MsgPack::ExtHelpers::unpack_upto_quint32(quint32 *to, quint8 *p, bool *success)
{
    if (*p == MsgPack::FirstByte::UINT32) {
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

quint8 *MsgPack::ExtHelpers::unpack_upto_quint64(quint64 *to, quint8 *p, bool *success)
{
    if (*p == MsgPack::FirstByte::UINT64) {
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

quint8 *MsgPack::ExtHelpers::unpack_upto_qint8(qint8 *to, quint8 *p, bool *success)
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

quint8 *MsgPack::ExtHelpers::unpack_upto_qint16(qint16 *to, quint8 *p, bool *success)
{
    if (*p == MsgPack::FirstByte::INT16) {
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

quint8 *MsgPack::ExtHelpers::unpack_upto_qint32(qint32 *to, quint8 *p, bool *success)
{
    if(*p == MsgPack::FirstByte::INT32) {
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

quint8 *MsgPack::ExtHelpers::unpack_upto_qint64(qint64 *to, quint8 *p, bool *success)
{
    if(*p == MsgPack::FirstByte::INT64) {
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
