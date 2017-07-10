// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "msgpack.h"
#include "private/unpack_p.h"
#include "private/pack_p.h"
#include "private/qt_types_p.h"

#include <QVector>

QVariant MsgPack::unpack(const QByteArray &data)
{
    quint8 *p = (quint8 *)data.data();
    quint8 *end = p + data.size() - 1;

    return MsgPackPrivate::unpack(p, end);
}

QByteArray MsgPack::pack(const QVariant &variant)
{
    QVector<QByteArray> user_data;
    // first run, calculate size
    ptrdiff_t size = MsgPackPrivate::pack(variant, nullptr, false, user_data) -
        static_cast<quint8 *>(nullptr);
    QByteArray arr;
    arr.resize(size);

    // second run, pack it
    MsgPackPrivate::pack(variant, (quint8 *)arr.data(), true, user_data);

    return arr;
}

bool MsgPack::registerPacker(int qType, qint8 msgpackType, MsgPack::pack_user_f packer)
{
    return MsgPackPrivate::register_packer(qType, msgpackType, packer);
}

bool MsgPack::registerUnpacker(qint8 msgpackType, MsgPack::unpack_user_f unpacker)
{
    return MsgPackPrivate::register_unpacker(msgpackType, unpacker);
}

qint8 MsgPack::msgpackType(int qType)
{
    return MsgPackPrivate::msgpack_type((QMetaType::Type)qType);
}

bool MsgPack::registerType(QMetaType::Type qType, quint8 msgpackType)
{
    return MsgPackPrivate::register_qtype(qType, msgpackType);
}

void MsgPack::setCompatibilityModeEnabled(bool enabled)
{
    MsgPackPrivate::compatibilityMode = enabled;
}
