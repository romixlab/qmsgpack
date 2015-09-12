#ifndef QMSGPACK_STREAM_LOCATION_H
#define QMSGPACK_STREAM_LOCATION_H

#ifdef QT_LOCATION_LIB

#include "../msgpack_export.h"
#include "../msgpackstream.h"

#include <QGeoCoordinate>

MSGPACK_EXPORT MsgPackStream& operator>>(MsgPackStream& s, QGeoCoordinate &coordinate);
MSGPACK_EXPORT MsgPackStream &operator<<(MsgPackStream& s, const QGeoCoordinate &coordinate);

#endif // QT_LOCATION_LIB
#endif // QMSGPACK_STREAM_LOCATION_H
