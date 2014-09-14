#include "unpack_p.h"
#include <QDebug>

MsgPackPrivate::type_parser_f MsgPackPrivate::unpackers[32] = {
    unpack_nil,
    unpack_never_used,
    unpack_false, unpack_true,
    unpack_bin8, unpack_bin16, unpack_bin32,
    unpack_ext8, unpack_ext16, unpack_ext32,
    unpack_float32, unpack_float64,
    unpack_uint8, unpack_uint16, unpack_uint32, unpack_uint64,
    unpack_int8, unpack_int16, unpack_int32, unpack_int64,
    unpack_fixext1, unpack_fixext2, unpack_fixext4, unpack_fixext8, unpack_fixext16,
    unpack_str8, unpack_str16, unpack_str32,
    unpack_array16, unpack_array32,
    unpack_map16, unpack_map32
};

QVariant MsgPackPrivate::unpack(quint8 *p, quint8 *end)
{
    QVariantList d;
    quint32 typesz = 0;

    while (p <= end) {
        typesz = 0;
        d.append(unpack_type(p, typesz));
        p += typesz;
    }

    if (p - end > 1)
        return QVariant();

    if (d.length() == 1)
        return d[0];
    return d;
}

QVariant MsgPackPrivate::unpack_type(quint8 *p, quint32 &sz)
{
    QVariant d;

    if (*p <= 127) { // positive fixint 0x00 - 0x7f
        d = unpack_positive_fixint(p, sz);
    } else if (*p >= 0xe0) { // negative fixint 0xe0 - 0xff
        d = unpack_negative_fixint(p, sz);
    } else if (*p >= 0x80 && *p <= 0x8f) { // fixmap 1000xxxx 0x80 - 0x8f
        d = unpack_fixmap(p, sz);
    } else if (*p >= 0x90 && *p <= 0x9f) { // fixarray 1001xxxx 0x90 - 0x9f
        d = unpack_fixarray(p, sz);
    } else if (*p >= 0xa0 && *p <= 0xbf) { // fixstr 101xxxxx 0xa0 - 0xbf
        d = unpack_fixstr(p, sz);
    } else { // all other types
        d = (unpackers[*p - 0xc0])(p, sz);
    }

    //qDebug() << "unpack type res:" << d << sz;
    return d;
}

QVariant MsgPackPrivate::unpack_positive_fixint(quint8 *p, quint32 &sz)
{
    sz += 1;
    quint32 val = (quint32)*p;
    return val;
}

QVariant MsgPackPrivate::unpack_negative_fixint(quint8 *p, quint32 &sz)
{
    sz += 1;
    quint8 val8 = (quint8)*p;
    val8 &= ~((1 << 7) | (1 << 6) | (1 << 5));
    qint32 val = 0xffffffff;
    val &= (0xffffffe0 | val8);
    return val;
}

QVariant MsgPackPrivate::unpack_map_len(quint8 *p, quint32 &sz, quint32 len)
{
    quint32 elemsz = 0;
    QMap<QString, QVariant> map;

    for (quint32 i = 0; i < len; ++i) {
        elemsz = 0;
        QVariant key = unpack_type(p, elemsz);
        p += elemsz; sz += elemsz;
        elemsz = 0;

        QVariant val = unpack_type(p, elemsz);
        p += elemsz; sz += elemsz;
        map.insert(key.toString(), val);
    }
    return map;
}

QVariant MsgPackPrivate::unpack_fixmap(quint8 *p, quint32 &sz)
{
    sz += 1;
    quint32 len = (*p++) & 0x0f; // 0b00001111
    return unpack_map_len(p, sz ,len);
}

QVariant MsgPackPrivate::unpack_array_len(quint8 *p, quint32 &sz, quint32 len)
{
    quint32 elemsz = 0;
    QVariantList arr;

    for (quint32 i = 0; i < len; ++i) {
        elemsz = 0;
        arr.append(unpack_type(p, elemsz));
        p += elemsz;
        sz += elemsz;
    }
    return arr;
}

quint32 MsgPackPrivate::unpack_uint8(quint8 *p)
{
    return *p;
}


quint32 MsgPackPrivate::unpack_uint16(quint8 *p)
{
    quint32 val = 0;
    quint8 *pv = (quint8 *)&val;
    pv[1] = *(p); pv[0] = *(++p);
    return val;
}


quint32 MsgPackPrivate::unpack_uint32(quint8 *p)
{
    quint32 val = 0;
    quint8 *pv = (quint8 *)&val;
    pv[3] = *(p); pv[2] = *(++p);
    pv[1] = *(++p); pv[0] = *(++p);
    return val;
}

QVariant MsgPackPrivate::unpack_fixarray(quint8 *p, quint32 &sz)
{
    sz += 1;
    quint32 len = (*p++) & 0x0f; // 0b00001111
    return unpack_array_len(p, sz, len);
}

QVariant MsgPackPrivate::unpack_fixstr(quint8 *p, quint32 &sz)
{
    sz += 1;
    quint32 len = (*p) & 0x1f; // 0b00011111
    QString str = QString::fromUtf8((char*)(p +  1), len);
    sz += len;
    return QVariant(str);
}

QVariant MsgPackPrivate::unpack_nil(quint8 *p, quint32 &sz)
{
    Q_UNUSED(p)
    sz += 1;
    return 0;
}

QVariant MsgPackPrivate::unpack_never_used(quint8 *p, quint32 &sz)
{
    Q_UNUSED(p)
    sz += 1;
    return QVariant();
}

QVariant MsgPackPrivate::unpack_false(quint8 *p, quint32 &sz)
{
    Q_UNUSED(p)
    sz += 1;
    return false;
}

QVariant MsgPackPrivate::unpack_true(quint8 *p, quint32 &sz)
{
    Q_UNUSED(p)
    sz += 1;
    return true;
}

QVariant MsgPackPrivate::unpack_bin8(quint8 *p, quint32 &sz)
{
    sz += 2;
    quint32 len = unpack_uint8(++p);
    QByteArray arr((const char *)(p + 1), len);
    sz += len;
    return arr;
}

QVariant MsgPackPrivate::unpack_bin16(quint8 *p, quint32 &sz)
{
    sz += 3;
    quint32 len = unpack_uint16(++p);
    QByteArray arr((const char *)(p + 2), len);
    sz += len;
    return arr;
}

QVariant MsgPackPrivate::unpack_bin32(quint8 *p, quint32 &sz)
{
    sz += 5;
    quint32 len = unpack_uint32(++p);
    QByteArray arr((const char *)(p + 4), len);
    sz = len;
    return arr;
}

QVariant MsgPackPrivate::unpack_ext8(quint8 *p, quint32 &sz)
{

}

QVariant MsgPackPrivate::unpack_ext16(quint8 *p, quint32 &sz)
{

}

QVariant MsgPackPrivate::unpack_ext32(quint8 *p, quint32 &sz)
{

}

QVariant MsgPackPrivate::unpack_float32(quint8 *p, quint32 &sz)
{
    sz += 5;
    float val = 0;
    quint8 *vp = ( (quint8 *)&val ) + 3;
    for (int i = 0; i < 4; ++i)
        *(vp--) = *(++p);
    return val;
}

QVariant MsgPackPrivate::unpack_float64(quint8 *p, quint32 &sz)
{
    sz += 9;
    double val = 0;
    quint8 *vp = ( (quint8 *)&val ) + 7;
    for (int i = 0; i < 8; ++i)
        *(vp--) = *(++p);
    return val;
}

QVariant MsgPackPrivate::unpack_uint8(quint8 *p, quint32 &sz)
{
    sz += 2;
    return unpack_uint8(++p);
}

QVariant MsgPackPrivate::unpack_uint16(quint8 *p, quint32 &sz)
{
    sz += 3;
    return unpack_uint16(++p);
}

QVariant MsgPackPrivate::unpack_uint32(quint8 *p, quint32 &sz)
{
    sz += 5;
    return unpack_uint32(++p);
}

QVariant MsgPackPrivate::unpack_uint64(quint8 *p, quint32 &sz)
{
    sz += 9;
    quint64 val = 0;
    for (int i = 56; i >= 0; i -= 8)
        val |= (quint64)(*(++p)) << i;
    return val;
}

QVariant MsgPackPrivate::unpack_int8(quint8 *p, quint32 &sz)
{
    sz += 2;
    qint32 val = 0xffffff00;
    val |= *(++p);
    return val;
}

QVariant MsgPackPrivate::unpack_int16(quint8 *p, quint32 &sz)
{
    sz += 3;
    qint32 val = 0xffff0000;
    quint8 *pv = (quint8 *)&val;
    pv[1] = *(++p); pv[0] = *(++p);
    return val;
}

QVariant MsgPackPrivate::unpack_int32(quint8 *p, quint32 &sz)
{
    sz += 5;
    qint32 val = 0;
    quint8 *pv = (quint8 *)&val;
    pv[3] = *(++p); pv[2] = *(++p);
    pv[1] = *(++p); pv[0] = *(++p);
    return val;
}

QVariant MsgPackPrivate::unpack_int64(quint8 *p, quint32 &sz)
{
    sz += 9;
    qint64 val = 0;
    for (int i = 56; i >= 0; i -= 8)
        val |= (quint64)(*(++p)) << i;
    return val;
}

QVariant MsgPackPrivate::unpack_fixext1(quint8 *p, quint32 &sz)
{

}

QVariant MsgPackPrivate::unpack_fixext2(quint8 *p, quint32 &sz)
{

}

QVariant MsgPackPrivate::unpack_fixext4(quint8 *p, quint32 &sz)
{

}

QVariant MsgPackPrivate::unpack_fixext8(quint8 *p, quint32 &sz)
{

}

QVariant MsgPackPrivate::unpack_fixext16(quint8 *p, quint32 &sz)
{

}

QVariant MsgPackPrivate::unpack_str8(quint8 *p, quint32 &sz)
{
    sz += 2;
    quint32 len = unpack_uint8(++p);
    QString str = QString::fromUtf8((char*)(p +  1), len);
    sz += len;
    return QVariant(str);
}

QVariant MsgPackPrivate::unpack_str16(quint8 *p, quint32 &sz)
{
    sz += 3;
    quint32 len = unpack_uint16(++p);
    QString str = QString::fromUtf8((char*)(p +  2), len);
    sz += len;
    return QVariant(str);
}

QVariant MsgPackPrivate::unpack_str32(quint8 *p, quint32 &sz)
{
    sz += 5;
    quint32 len = unpack_uint32(++p);
    QString str = QString::fromUtf8((char*)(p +  4), len);
    sz += len;
    return QVariant(str);
}

QVariant MsgPackPrivate::unpack_array16(quint8 *p, quint32 &sz)
{
    sz += 3;
    quint32 len = unpack_uint16(++p);
    return unpack_array_len(p + 2, sz, len);
}

QVariant MsgPackPrivate::unpack_array32(quint8 *p, quint32 &sz)
{
    sz += 5;
    quint32 len = unpack_uint32(++p);
    return unpack_array_len(p + 4, sz, len);
}

QVariant MsgPackPrivate::unpack_map16(quint8 *p, quint32 &sz)
{
    sz += 3;
    quint32 len = unpack_uint16(p);
    return unpack_map_len(p + 2, sz, len);
}

QVariant MsgPackPrivate::unpack_map32(quint8 *p, quint32 &sz)
{
    sz += 5;
    quint32 len = unpack_uint32(p);
    return unpack_map_len(p + 4, sz, len);
}




