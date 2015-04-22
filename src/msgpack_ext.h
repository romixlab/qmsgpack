#ifndef MSGPACK_EXT_H
#define MSGPACK_EXT_H
#include <QtGlobal>

namespace MsgPack
{
    namespace Ext {
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
#endif // MSGPACK_EXT_H
