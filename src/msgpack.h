#ifndef MSGPACK_H
#define MSGPACK_H

#include "msgpack_common.h"
#include "msgpack_export.h"

#include <QByteArray>
#include <QVariantList>

namespace MsgPack
{
    MSGPACK_EXPORT QVariant unpack(const QByteArray &data);
    MSGPACK_EXPORT bool registerUnpacker(qint8 msgpackType, unpack_user_f unpacker);

    MSGPACK_EXPORT QByteArray pack(const QVariant &variant);
    MSGPACK_EXPORT bool registerPacker(QMetaType::Type qType, qint8 msgpackType, pack_user_f packer);

    MSGPACK_EXPORT bool registerType(QMetaType::Type qType, quint8 msgpackType);

    MSGPACK_EXPORT void setCompatibilityModeEnabled(bool enabled);

    namespace ExtHelpers {
    quint8 * unpack_upto_quint8(quint8 *to, quint8 *from, bool *success);
    quint8 * unpack_upto_quint16(quint16 *to, quint8 *from, bool *success);
    quint8 * unpack_upto_quint32(quint32 *to, quint8 *from, bool *success);
    quint8 * unpack_upto_quint64(quint64 *to, quint8 *from, bool *success);
    quint8 * unpack_upto_qint8(qint8 *to, quint8 *from, bool *success);
    quint8 * unpack_upto_qint16(qint16 *to, quint8 *from, bool *success);
    quint8 * unpack_upto_qint32(qint32 *to, quint8 *from, bool *success);
    quint8 * unpack_upto_qint64(qint64 *to, quint8 *from, bool *success);
    quint8 * unpack_float(float *to, quint8 *from, bool *success);
    quint8 * unpack_double(double *to, quint8 *from, bool *success);
    } // ExtHelpers
} // MsgPack

#endif // MSGPACK_H
