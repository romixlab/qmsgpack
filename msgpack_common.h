#ifndef COMMON_H
#define COMMON_H

#include <QVariant>

namespace MsgPack {
typedef quint8 * (pack_user_f)(const QVariant &t, quint8 *p, bool wr);
}

#endif // COMMON_H
