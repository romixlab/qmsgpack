#ifndef MSGPACK_H
#define MSGPACK_H

#include "msgpack_export.h"
#include "msgpackcommon.h"

#include <QByteArray>
#include <QVariantList>

namespace MsgPack
{
    MSGPACK_EXPORT QVariant unpack(const QByteArray &data);
    MSGPACK_EXPORT bool registerUnpacker(qint8 msgpackType, unpack_user_f unpacker);

    MSGPACK_EXPORT QByteArray pack(const QVariant &variant);
    MSGPACK_EXPORT bool registerPacker(QMetaType::Type qType, qint8 msgpackType, pack_user_f packer);
    MSGPACK_EXPORT qint8 msgpackType(int qType);

    MSGPACK_EXPORT bool registerType(QMetaType::Type qType, quint8 msgpackType);

    MSGPACK_EXPORT void setCompatibilityModeEnabled(bool enabled);
} // MsgPack

#endif // MSGPACK_H
