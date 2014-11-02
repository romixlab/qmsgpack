#ifndef PACK_P_H
#define PACK_P_H
#include <QVariant>
#include "../msgpack_common.h"

namespace MsgPackPrivate {
/* if wr (write) == false, packer just moves pointer forward
 *
 */
typedef struct {
    MsgPack::pack_user_f packer;
    qint8 type;
} packer_t;
bool register_packer(QMetaType::Type q_type, qint8 msgpack_type, MsgPack::pack_user_f packer);
extern QHash<QMetaType::Type, packer_t> user_packers;
extern bool compatibilityMode;

quint8 * pack(const QVariant &v, quint8 *p, bool wr);

quint8 * pack_int(qint32 i, quint8 *p, bool wr);
quint8 * pack_uint(quint32 i, quint8 *p, bool wr);
quint8 * pack_longlong(qint64 i, quint8 *p, bool wr);
quint8 * pack_ulonglong(quint64 i, quint8 *p, bool wr);

quint8 * pack_bool(const QVariant &v, quint8 *p, bool wr);

quint8 * pack_arraylen(quint32 len, quint8 *p, bool wr);
quint8 * pack_array(const QVariantList &list, quint8 *p, bool wr);
quint8 * pack_stringlist(const QStringList &list, quint8 *p, bool wr);

quint8 * pack_string(const QString &str, quint8 *p, bool wr);
quint8 * pack_double(double i, quint8 *p, bool wr);
quint8 * pack_bin(const QByteArray &arr, quint8 *p, bool wr);
quint8 * pack_map(const QVariantMap &map, quint8 *p, bool wr);
quint8 * pack_user(const QVariant &v, quint8 *p, bool wr);
}

#endif // PACK_P_H
