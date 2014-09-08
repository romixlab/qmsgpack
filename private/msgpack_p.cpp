#include "msgpack_p.h"
#include <QDebug>

MsgPackPrivate::type_parser_f MsgPackPrivate::parsers[32] = {
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
    int typesz = 0;

    while (p <= end) {
        typesz = 0;
        d.append(unpack_type(p, end, typesz));
        p += typesz;
        //qDebug() << "unpack typesz:" << typesz;
    }

    if (p - end > 1)
        return QVariant();

    if (d.length() == 1)
        return d[0];
    return d;
}

QVariant MsgPackPrivate::unpack_type(quint8 *p, quint8 *end, int &sz)
{
    QVariant d;

    if (*p <= 127) { // positive fixint 0x00 - 0x7f
        d = unpack_positive_fixint(p, end, sz);
    } else if (*p >= 0xe0) { // negative fixint 0xe0 - 0xff
        d = unpack_negative_fixint(p, end, sz);
    } else if (*p >= 0x80 && *p <= 0x8f) { // fixmap 1000xxxx 0x80 - 0x8f
        d = unpack_fixmap(p, end, sz);
    } else if (*p >= 0x90 && *p <= 0x9f) { // fixarray 1001xxxx 0x90 - 0x9f
        d = unpack_fixarray(p, end, sz);
    } else if (*p >= 0xa0 && *p <= 0xbf) { // fixstr 101xxxxx 0xa0 - 0xbf
        d = unpack_fixstr(p, end, sz);
    } else { // all other types
        d = (parsers[*p - 0xc0])(p, end, sz);
    }

    //qDebug() << "unpack type res:" << d << sz;
    return d;
}

QVariant MsgPackPrivate::unpack_positive_fixint(quint8 *p, quint8 *end, int &sz)
{
    Q_UNUSED(end)
    quint32 val = (quint32)*p;
    sz = 1;
    return QVariant(val);
}

QVariant MsgPackPrivate::unpack_negative_fixint(quint8 *p, quint8 *end, int &sz)
{
    Q_UNUSED(end)
    quint8 val8 = (quint8)*p;
    val8 &= ~((1 << 7) | (1 << 6) | (1 << 5));
    qint32 val = 31 - val8;
    val ^= 0xffffffff;
    sz = 1;
    return QVariant(val);
}

QVariant MsgPackPrivate::unpack_fixmap(quint8 *p, quint8 *end, int &sz)
{
    Q_UNUSED(end)
}

QVariant MsgPackPrivate::unpack_fixarray(quint8 *p, quint8 *end, int &sz)
{
    int len = (*p++) & 0x0f; // 0b00001111
    sz++;

    int elemsz = 0;
    QVariantList arr;

    for (int i = 0; i < len; ++i) {
        elemsz = 0;
        arr.append(unpack_type(p, end, elemsz));
        p += elemsz;
        sz += elemsz;
    }
//qDebug() << "unpack_fixarray len:" << len << arr << sz;
    return arr;
}

QVariant MsgPackPrivate::unpack_fixstr(quint8 *p, quint8 *end, int &sz)
{
    Q_UNUSED(end)
    int len = (*p) & 0x1f; // 0b00011111
    QString str = QString::fromUtf8((char*)(p +  1), len);
    sz = len + 1;
    return QVariant(str);
}

QVariant MsgPackPrivate::unpack_nil(quint8 *p, quint8 *end, int &sz)
{
    Q_UNUSED(end)
    sz = 1;
    return QVariant(0);
}

QVariant MsgPackPrivate::unpack_never_used(quint8 *p, quint8 *end, int &sz)
{
    Q_UNUSED(end)
    sz = 1;
    return QVariant();
}

QVariant MsgPackPrivate::unpack_false(quint8 *p, quint8 *end, int &sz)
{
    sz = 1;
    return QVariant(false);
}

QVariant MsgPackPrivate::unpack_true(quint8 *p, quint8 *end, int &sz)
{
    sz = 1;
    return QVariant(true);
}

QVariant MsgPackPrivate::unpack_bin8(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_bin16(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_bin32(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_ext8(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_ext16(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_ext32(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_float32(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_float64(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_uint8(quint8 *p, quint8 *end, int &sz)
{
    quint32 val;
    val = *(++p);
    sz = 2;
    return val;
}

QVariant MsgPackPrivate::unpack_uint16(quint8 *p, quint8 *end, int &sz)
{
    quint32 val;
    val = *(++p);
    val |= *(++p) << 8;
    sz = 3;
    return val;
}

QVariant MsgPackPrivate::unpack_uint32(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_uint64(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_int8(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_int16(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_int32(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_int64(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_fixext1(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_fixext2(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_fixext4(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_fixext8(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_fixext16(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_str8(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_str16(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_str32(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_array16(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_array32(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_map16(quint8 *p, quint8 *end, int &sz)
{

}

QVariant MsgPackPrivate::unpack_map32(quint8 *p, quint8 *end, int &sz)
{

}



