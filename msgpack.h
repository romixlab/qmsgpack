#ifndef MSGPACK_H
#define MSGPACK_H
#include <QByteArray>
#include <QVariantList>
#include "msgpack_common.h"

namespace MsgPack
{
    QVariant unpack(const QByteArray &data);
    bool registerUnpacker(qint8 msgpackType, unpack_user_f unpacker);

    QByteArray pack(const QVariant &variant);
    bool registerPacker(QMetaType::Type qType, qint8 msgpackType, pack_user_f packer);
}

#endif // MSGPACK_H
