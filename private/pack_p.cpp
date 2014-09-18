#include "pack_p.h"
#include "private/sysdep.h"

quint8 *MsgPackPrivate::pack(const QVariant &v, quint8 *p, bool wr)
{
    QMetaType::Type t = (QMetaType::Type)v.type();
    if (t == QMetaType::Int)
        p = pack_int(v.toInt(), p, wr);
    else if (t == QMetaType::UInt)
        p = pack_uint(v.toInt(), p, wr);
    else if (t == QMetaType::Bool)
        p = pack_bool(v, p, wr);
    else if (t == QMetaType::QString)
        p = pack_string(v.toString(), p, wr);
    else if (t == QMetaType::QVariantList)
        p = pack_list(v.toList(), p, wr);
    else if (t == QMetaType::LongLong)
        p = pack_longlong(v.toLongLong(), p, wr);
    else if (t == QMetaType::ULongLong)
        p = pack_ulonglong(v.toULongLong(), p, wr);
    else if (t == QMetaType::Double)
        p = pack_double(v.toDouble(), p, wr);
    else if (t == QMetaType::QByteArray)
        p = pack_array(v.toByteArray(), p, wr);
    else if (t == QMetaType::QVariantMap)
        p = pack_map(v.toMap(), p, wr);

    return p;
}

quint8 *MsgPackPrivate::pack_int(qint32 i, quint8 *p, bool wr)
{
    if (i >= -32 && i <= 127) {
        qint32 val = _msgpack_be32(i);
        if (wr) *p = *( (quint8 *)&val + 3 );
        p++;
    } else if (i >= -128 && i <= 255) {
        if (wr) *p = i > 0 ? 0xcc : 0xd0;
        p++;
        if (wr) *p = i;
        p++;
    } else if (i >= -32768 && i <= 65535) {
        if (wr) *p = i > 0 ? 0xcd : 0xd1;
        p++;
        if (wr) _msgpack_store16(p, i);
        p += 2;
    } else {
        if (wr) *p = i > 0 ? 0xce : 0xd2;
        p++;
        if (wr) _msgpack_store32(p, i);
        p += 4;
    }

    return p;
}

quint8 *MsgPackPrivate::pack_uint(quint32 i, quint8 *p, bool wr)
{
    if (i <= 127) {
        qint32 val = _msgpack_be32(i);
        if (wr) *p = *( (quint8 *)&val + 3 );
        p++;
    } else if (i <= 255) {
        if (wr) *p = 0xcc;
        p++;
        if (wr) *p = i;
        p++;
    } else if (i <= 65535) {
        if (wr) *p = 0xcd;
        p++;
        if (wr) _msgpack_store16(p, i);
        p += 2;
    } else {
        if (wr) *p = 0xce;
        p++;
        if (wr) _msgpack_store32(p, i);
        p += 4;
    }

    return p;
}

quint8 *MsgPackPrivate::pack_longlong(qint64 i, quint8 *p, bool wr)
{
    if (i >= -2147483648 && i <= 2147483647)
        return p = pack_int(i, p, wr);
    if (wr) *p = 0xd3;
    p++;
    if (wr) _msgpack_store64(p, i);
    return p += 8;
}

quint8 *MsgPackPrivate::pack_ulonglong(quint64 i, quint8 *p, bool wr)
{
    if (i <= 4294967295)
        return pack_uint(i, p, wr);
    if (wr) *p = 0xcf;
    p++;
    if (wr) _msgpack_store64(p, i);
    return p += 8;
}

quint8 *MsgPackPrivate::pack_bool(const QVariant &v, quint8 *p, bool wr)
{
    if (wr)
        *p = v.toBool() ? 0xc3 : 0xc2;
    return p + 1;
}

quint8 *MsgPackPrivate::pack_list(const QVariantList &list, quint8 *p, bool wr)
{
    int len = list.length();
    if (len <= 15) {
        if (wr) *p = 0x90 | len;
        p++;
    } else {
        if (len <= 65535) {
            if (wr) *p = 0xdc;
            p++;
            _msgpack_store16(p, len);
            p += 2;
        } else {
            if (wr) *p = 0xdd;
            p++;
            _msgpack_store32(p, len);
            p += 4;
        }
    }
    foreach (QVariant item, list)
        p = pack(item, p, wr);
    return p;
}

quint8 *MsgPackPrivate::pack_string(const QString &str, quint8 *p, bool wr)
{
    int len = str.length();
    if (len <= 31) {
        if (wr) *p = 0xa0 | len;
        p++;
    } else if (len <= 255) {
        if (wr) *p = 0xd9;
        p++;
        if (wr) *p = len;
        p++;
    } else if (len <= 65535) {
        if (wr) *p = 0xda;
        p++;
        if (wr) _msgpack_store16(p, len);
        p += 2;
    } else {
        if (wr) *p = 0xdb;
        p++;
        if (wr) _msgpack_store32(p, len);
        p += 4;
    }
    if (wr) memcpy(p, str.toUtf8().data(), len);
    p += len;

    return p;
}

quint8 *MsgPackPrivate::pack_double(double i, quint8 *p, bool wr)
{
    if (wr) *p = 0xcb;
    p++;
    if (wr) {
        quint8 *d = (quint8 *)&i;
#ifdef __LITTLE_ENDIAN__
        for (int i = 0; i < 8; ++i)
            *(p + 7 - i) = *(d + i);
#else
        for (int i = 0; i < 8; ++i)
            *(p + i) = *(d + i);
#endif
    }
    return p += 8;
}

quint8 *MsgPackPrivate::pack_array(const QByteArray &arr, quint8 *p, bool wr)
{
    int len = arr.length();
    if (len <= 255) {
        if (wr) *p = 0xc4;
        p++;
        if (wr) *p = len;
        p++;
    } else if (len <= 65535) {
        if (wr) *p = 0xc5;
        p++;
        if (wr) _msgpack_store16(p, len);
        p += 2;
    } else {
        if (wr) *p = 0xc6;
        p++;
        if (wr) _msgpack_store32(p, len);
        p += 4;
    }
    if (wr) memcpy(p, arr.data(), len);
    p += len;

    return p;
}


quint8 *MsgPackPrivate::pack_map(const QVariantMap &map, quint8 *p, bool wr)
{
    QMapIterator<QString, QVariant> it(map);
    int len = 0;
    while (it.hasNext()) {
        it.next();
        len++;
    }
    if (len <= 15) {
        if (wr) *p = 0x80 | len;
        p++;
    } else if (len <= 65535) {
        if (wr) *p = 0xde;
        p++;
        if (wr) _msgpack_store16(p, len);
        p += 2;
    } else {
        if (wr) *p = 0xdf;
        p++;
        if (wr) _msgpack_store32(p, len);
        p += 4;
    }

    it.toFront();
    while (it.hasNext()) {
        it.next();
        p = pack(it.key(), p, wr);
        p = pack(it.value(), p, wr);
    }
    return p;
}
