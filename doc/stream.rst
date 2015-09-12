Streams
-------

.. contents::
   :depth:  4

There are QDataStream analogue with almost identical API. Every basic type is supported as well as QList<T> and Qt types (QPoint, QSize, QRect, QTime, QDate, QDateTime, QColor, QGeoCoordinate). More types are on the way.

Packing
=======

You can use any QIODevice derived class or QByteArray.

.. code-block:: cpp
    
    QByteArray ba;
    MsgPackStream out(&ba, QIODevice::WriteOnly);
    out << 1 << true << "Hello";
    qDebug() << ba.toHex();

Of course you can unpack this byte array using ``MsgPack::unpack``:

.. code-block:: cpp

    qDebug() << MsgPack::unpack(ba);
    // output:
    // QVariant(QVariantList, (QVariant(uint, 1), QVariant(bool, true), QVariant(QString, "Hello"))) 

QList of any type are also supported:

.. code-block:: cpp

    QList<int> list;
    list << 1 << 2 << 3;
    QByteArray ba;
    MsgPackStream out(&ba, QIODevice::WriteOnly);
    out << list;

Unpacking
=========

To unpack QByteArray just pass it by value, or use QIODevice::ReadOnly for other devices:

.. code-block:: cpp

    MsgPackStream in(ba);
    QList<int> list2;
    in >> list2;
    qDebug() << list2; // (1, 2, 3)

More types
==========

Include ``<qmsgpack/stream/geometry.h`` (or location.h or time.h) for additional types.
And do not forget to register type, so that MsgPackStream will know which user type id to use.

.. code-block:: cpp

    #include <qmsgpack/stream/geometry.h>
    // ...

    MsgPack::registerType(QMetaType::QPoint, 3);
    QByteArray ba;
    MsgPackStream out(&ba, QIODevice::WriteOnly);
    out << QPoint(1, 2) << QPoint();
    qDebug() << MsgPack::unpack(ba);
    // output:
    // QVariant(QVariantList, (QVariant(QPoint, QPoint(1,2)), QVariant(QPoint, QPoint(0,0))))
