#ifndef PACK_P_H
#define PACK_P_H
#include <QVariant>

namespace MsgPackPrivate {
/* if sp (size probe) == true, packer just moves pointer forward
 *
 */


quint8 * pack(const QVariant &v, quint8 *p, bool sp);

quint8 * pack_int(const QVariant &v, quint8 *p, bool sp);
quint8 * pack_bool(const QVariant &v, quint8 *p, bool sp);
quint8 * pack_list(const QVariant &v, quint8 *p, bool sp);

}

#endif // PACK_P_H
