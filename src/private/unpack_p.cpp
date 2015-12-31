#include "unpack_p.h"
#include "../endianhelper.h"

#include <QByteArray>
#include <QDebug>
#include <QMap>
#include <QReadLocker>
#include <QWriteLocker>

MsgPackPrivate::type_parser_f MsgPackPrivate::unpackers[32] = {
    MsgPackPrivate::unpack_nil,
    MsgPackPrivate::unpack_never_used,
    MsgPackPrivate::unpack_false,
    MsgPackPrivate::unpack_true,
    MsgPackPrivate::unpack_bin8,
    MsgPackPrivate::unpack_bin16,
    MsgPackPrivate::unpack_bin32,
    MsgPackPrivate::unpack_ext8,
    MsgPackPrivate::unpack_ext16,
    MsgPackPrivate::unpack_ext32,
    MsgPackPrivate::unpack_float32,
    MsgPackPrivate::unpack_float64,
    MsgPackPrivate::unpack_uint8,
    MsgPackPrivate::unpack_uint16,
    MsgPackPrivate::unpack_uint32,
    MsgPackPrivate::unpack_uint64,
    MsgPackPrivate::unpack_int8,
    MsgPackPrivate::unpack_int16,
    MsgPackPrivate::unpack_int32,
    MsgPackPrivate::unpack_int64,
    MsgPackPrivate::unpack_fixext1,
    MsgPackPrivate::unpack_fixext2,
    MsgPackPrivate::unpack_fixext4,
    MsgPackPrivate::unpack_fixext8,
    MsgPackPrivate::unpack_fixext16,
    MsgPackPrivate::unpack_str8,
    MsgPackPrivate::unpack_str16,
    MsgPackPrivate::unpack_str32,
    MsgPackPrivate::unpack_array16,
    MsgPackPrivate::unpack_array32,
    MsgPackPrivate::unpack_map16,
    MsgPackPrivate::unpack_map32
};

QHash<qint8, MsgPack::unpack_user_f> MsgPackPrivate::user_unpackers;
QReadWriteLock MsgPackPrivate::unpackers_lock;

QVariant MsgPackPrivate::unpack(quint8 *p, quint8 *end)
{
    QVariantList d;

    QVariant v;
    while (p <= end) {
        p = unpack_type(v, p);
        d.append(v);
    }

    if (p - end > 1)
        return QVariant();

    if (d.length() == 1)
        return d[0];
    return d;
}

quint8 *MsgPackPrivate::unpack_type(QVariant &v, quint8 *p)
{
    if (*p <= 127) { // positive fixint 0x00 - 0x7f
        p = unpack_positive_fixint(v, p);
    } else if (*p >= 0xe0) { // negative fixint 0xe0 - 0xff
        p = unpack_negative_fixint(v, p);
    } else if (*p >= 0x80 && *p <= 0x8f) { // fixmap 1000xxxx 0x80 - 0x8f
        p = unpack_fixmap(v, p);
    } else if (*p >= 0x90 && *p <= 0x9f) { // fixarray 1001xxxx 0x90 - 0x9f
        p = unpack_fixarray(v, p);
    } else if (*p >= 0xa0 && *p <= 0xbf) { // fixstr 101xxxxx 0xa0 - 0xbf
        p = unpack_fixstr(v, p);
    } else { // all other types
        p = (unpackers[*p - 0xc0])(v, p);
    }

    //qDebug() << "unpack type res:" << d << sz;
    return p;
}

quint8 * MsgPackPrivate::unpack_nil(QVariant &v, quint8 *p)
{
    Q_UNUSED(p)
    Q_UNUSED(v)
    v = QVariant();
    return p + 1;
}

quint8 * MsgPackPrivate::unpack_never_used(QVariant &v, quint8 *p)
{
    Q_UNUSED(p)
    Q_UNUSED(v)
    v = QVariant();
    return p + 1;
}

quint8 * MsgPackPrivate::unpack_false(QVariant &v, quint8 *p)
{
    Q_UNUSED(p)
    v = false;
    return p + 1;
}

quint8 * MsgPackPrivate::unpack_true(QVariant &v, quint8 *p)
{
    Q_UNUSED(p)
    v = true;
    return p + 1;
}

quint8 * MsgPackPrivate::unpack_positive_fixint(QVariant &v, quint8 *p)
{
    v = (quint32)*p;
    return p + 1;
}

quint8 * MsgPackPrivate::unpack_negative_fixint(QVariant &v, quint8 *p)
{
    v = (qint8)*p;
    return p + 1;
}

quint8 * MsgPackPrivate::unpack_uint8(QVariant &v, quint8 *p)
{
    v = (quint8)*(++p);
    return p + 1;
}

quint8 * MsgPackPrivate::unpack_uint16(QVariant &v, quint8 *p)
{
    p++;
    v = _msgpack_load16(quint16, p);
    return p + 2;
}

quint8 * MsgPackPrivate::unpack_uint32(QVariant &v, quint8 *p)
{
    p++;
    v = _msgpack_load32(quint32, p);
    return p + 4;
}

quint8 * MsgPackPrivate::unpack_uint64(QVariant &v, quint8 *p)
{
    p++;
    v = _msgpack_load64(quint64, p);
    return p + 8;
}

quint8 * MsgPackPrivate::unpack_int8(QVariant &v, quint8 *p)
{
    v = (qint8)*(++p);
    return p + 1;
}

quint8 * MsgPackPrivate::unpack_int16(QVariant &v, quint8 *p)
{
    p++;
    v = _msgpack_load16(qint16, p);
    return p + 2;
}

quint8 * MsgPackPrivate::unpack_int32(QVariant &v, quint8 *p)
{
    p++;
    v = _msgpack_load32(qint32, p);
    return p + 4;
}

quint8 * MsgPackPrivate::unpack_int64(QVariant &v, quint8 *p)
{
    p++;
    v = _msgpack_load64(qint64, p);
    return p + 8;
}

quint8 * MsgPackPrivate::unpack_float32(QVariant &v, quint8 *p)
{
    float f;
    quint8 *fp = (quint8 *)&f;
    p++;
#ifdef __LITTLE_ENDIAN__
    for (int i = 0; i < 4; ++i)
        *(fp + 3 - i) = *(p + i);
#else
    for (int i = 0; i < 4; ++i)
        *(fp + i) = *(p + i);
#endif
    v = f;
    return p + 4;
}

quint8 * MsgPackPrivate::unpack_float64(QVariant &v, quint8 *p)
{
    double d;
    quint8 *fd = (quint8 *)&d;
    p++;
#ifdef __LITTLE_ENDIAN__
    for (int i = 0; i < 8; ++i)
        *(fd + 7 - i) = *(p + i);
#else
    for (int i = 0; i < 8; ++i)
        *(fp + i) = *(p + i);
#endif
    v = d;
    return p + 8;
}

quint8 * MsgPackPrivate::unpack_fixstr(QVariant &v, quint8 *p)
{
    int len = (*p) & 0x1f; // 0b00011111
    p++;
    v = QString::fromUtf8((char*)p, len);
    return p + len;
}

quint8 * MsgPackPrivate::unpack_str8(QVariant &v, quint8 *p)
{
    int len = *(++p);
    v = QString::fromUtf8((char*)(++p), len);
    return p + len;
}

quint8 * MsgPackPrivate::unpack_str16(QVariant &v, quint8 *p)
{
    p++;
    int len = _msgpack_load16(int, p);
    p += 2;
    v = QString::fromUtf8((char*)p, len);
    return p + len;
}

quint8 * MsgPackPrivate::unpack_str32(QVariant &v, quint8 *p)
{
    p++;
    int len = _msgpack_load32(int, p);
    p += 4;
    v = QString::fromUtf8((char*)p, len);
    return p + len;
}

quint8 * MsgPackPrivate::unpack_bin8(QVariant &v, quint8 *p)
{
    int len = *(++p);
    v = QByteArray((char*)(++p), len);
    return p + len;
}

quint8 * MsgPackPrivate::unpack_bin16(QVariant &v, quint8 *p)
{
    p++;
    int len = _msgpack_load16(int, p);
    p += 2;
    v = QByteArray((char*)p, len);
    return p + len;
}

quint8 * MsgPackPrivate::unpack_bin32(QVariant &v, quint8 *p)
{
    p++;
    int len = _msgpack_load32(int, p);
    p += 4;
    v = QByteArray((char*)p, len);
    return p + len;
}

quint8 * MsgPackPrivate::unpack_array_len(QVariant &v, quint8 *p, quint32 len)
{
    QVariantList arr;

    QVariant vu;
    for (quint32 i = 0; i < len; ++i) {
        p = unpack_type(vu, p);
        arr.append(vu);
    }
    v = arr;
    return p;
}

quint8 * MsgPackPrivate::unpack_fixarray(QVariant &v, quint8 *p)
{
    quint32 len = (*p++) & 0x0f; // 0b00001111
    return unpack_array_len(v, p, len);
}

quint8 * MsgPackPrivate::unpack_array16(QVariant &v, quint8 *p)
{
    p++;
    quint32 len = _msgpack_load16(quint32, p);
    return unpack_array_len(v, p + 2, len);
}

quint8 * MsgPackPrivate::unpack_array32(QVariant &v, quint8 *p)
{
    p++;
    quint32 len = _msgpack_load32(quint32, p);
    return unpack_array_len(v, p + 4, len);
}

quint8 * MsgPackPrivate::unpack_map_len(QVariant &v, quint8 *p, quint32 len)
{
    QMap<QString, QVariant> map;
    QVariant key, val;

    for (quint32 i = 0; i < len; ++i) {
        p = unpack_type(key, p);
        p = unpack_type(val, p);

        map.insert(key.toString(), val);
    }
    v = map;
    return p;
}

quint8 * MsgPackPrivate::unpack_fixmap(QVariant &v, quint8 *p)
{
    quint32 len = (*p++) & 0x0f; // 0b00001111
    return unpack_map_len(v, p ,len);
}

quint8 * MsgPackPrivate::unpack_map16(QVariant &v, quint8 *p)
{
    p++;
    quint32 len = _msgpack_load16(quint32, p);
    return unpack_map_len(v, p + 2, len);
}

quint8 * MsgPackPrivate::unpack_map32(QVariant &v, quint8 *p)
{
    p++;
    quint32 len = _msgpack_load32(quint32, p);
    return unpack_map_len(v, p + 4, len);
}

quint8 *MsgPackPrivate::unpack_ext(QVariant &v, quint8 *p, qint8 type, quint32 len)
{
    QReadLocker locker(&unpackers_lock);
    if (!user_unpackers.contains(type)) {
        qWarning() << "MsgPack::unpack() unpacker for type" << type << "doesn't exist";
        return p + len;
    }
    QByteArray data((char *)p, len);
    v = user_unpackers[type](data);
    return p + len;
}

quint8 * MsgPackPrivate::unpack_fixext1(QVariant &v, quint8 *p)
{
    qint8 type = *(++p);
    return unpack_ext(v, p + 1, type, 1);
}

quint8 * MsgPackPrivate::unpack_fixext2(QVariant &v, quint8 *p)
{
    qint8 type = *(++p);
    return unpack_ext(v, p + 1, type, 2);
}

quint8 * MsgPackPrivate::unpack_fixext4(QVariant &v, quint8 *p)
{
    qint8 type = *(++p);
    return unpack_ext(v, p + 1, type, 4);
}

quint8 * MsgPackPrivate::unpack_fixext8(QVariant &v, quint8 *p)
{
    qint8 type = *(++p);
    return unpack_ext(v, p + 1, type, 8);
}

quint8 * MsgPackPrivate::unpack_fixext16(QVariant &v, quint8 *p)
{
    qint8 type = *(++p);
    return unpack_ext(v, p + 1, type, 16);
}

quint8 * MsgPackPrivate::unpack_ext8(QVariant &v, quint8 *p)
{
    p++;
    quint32 len = *(p);
    p += 1;
    qint8 type = *(p);
    return unpack_ext(v, p + 1, type, len);
}

quint8 * MsgPackPrivate::unpack_ext16(QVariant &v, quint8 *p)
{
    p++;
    quint32 len = _msgpack_load16(quint32, p);
    p += 2;
    qint8 type = *(p);
    return unpack_ext(v, p + 1, type, len);
}

quint8 * MsgPackPrivate::unpack_ext32(QVariant &v, quint8 *p)
{
    p++;
    quint32 len = _msgpack_load32(quint32, p);
    p += 4;
    qint8 type = *(p);
    return unpack_ext(v, p + 1, type, len);
}

bool MsgPackPrivate::register_unpacker(qint8 msgpack_type, MsgPack::unpack_user_f unpacker)
{
    if (unpacker == 0) {
        qWarning() << "MsgPack::unpacker for type" << msgpack_type << "is invalid";
        return false;
    }
    QWriteLocker locker(&unpackers_lock);
    if (user_unpackers.contains(msgpack_type)) {
        qWarning() << "MsgPack::unpacker for type" << msgpack_type << "already exists";
        return false;
    }
    user_unpackers.insert(msgpack_type, unpacker);
    return true;
}
