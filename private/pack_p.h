#ifndef PACK_P_H
#define PACK_P_H
#include <QVariant>
#include "../msgpack_common.h"

namespace MsgPackPrivate {
/* if wr (write) == false, packer just moves pointer forward
 *
 */

QHash<int, MsgPack::pack_user_f> user_packers;


quint8 * pack(const QVariant &v, quint8 *p, bool wr);

quint8 * pack_int(qint32 i, quint8 *p, bool wr);
quint8 * pack_uint(quint32 i, quint8 *p, bool wr);
quint8 * pack_longlong(qint64 i, quint8 *p, bool wr);
quint8 * pack_ulonglong(quint64 i, quint8 *p, bool wr);

quint8 * pack_bool(const QVariant &v, quint8 *p, bool wr);
quint8 * pack_list(const QVariantList &list, quint8 *p, bool wr);
quint8 * pack_string(const QString &str, quint8 *p, bool wr);
quint8 * pack_double(double i, quint8 *p, bool wr);
quint8 * pack_array(const QByteArray &arr, quint8 *p, bool wr);
quint8 * pack_map(const QVariantMap &map, quint8 *p, bool wr);



}

#endif // PACK_P_H
