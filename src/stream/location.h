#ifndef QMSGPACK_STREAM_LOCATION_H
#define QMSGPACK_STREAM_LOCATION_H

#include "../msgpackstream.h"

#include <QGeoCoordinate>

MsgPackStream& operator>>(MsgPackStream& s, QGeoCoordinate &coordinate)
{
    double x;
    s >> x;
    coordinate.setLatitude(x);
    s >> x;
    coordinate.setLongitude(x);
    return s;
}

MsgPackStream &operator<<(MsgPackStream& s, const QGeoCoordinate &coordinate)
{
    s << coordinate.latitude() << coordinate.longitude();
    return s;
}

#endif // QMSGPACK_STREAM_LOCATION_H