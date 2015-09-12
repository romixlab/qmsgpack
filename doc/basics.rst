Basics
------

.. contents::
   :depth:  4

Standard types
==============

Below are Qt equivalents to MessagePack types listed in `spec <https://github.com/msgpack/msgpack/blob/master/spec.md>`_:

================ ==============
MessagePack type Qt or C++ type
================ ==============
positive fixint  quint8
fixmap, map      QMap, QHash
fixarray,array   QList
fixstr, str      QString
nil              QVariant()
false, true      bool
bin              QByteArray
float 32         float
float 64         double
uint 8           quint8
uint 16          quint16
uint 32          quint32
uint 64          quint64
int 8            qint8
int 16           qint16
int 32           qint32, int
int 64           qint64
negative fixint  qint8
================ ==============

You can pack and unpack any of those types right away:

Packing
^^^^^^^

Pass QVariant to ``MsgPack::pack()`` function:

.. code-block:: cpp
    
    QVariant v = 123;
    QByteArray packed = MsgPack::pack(v);
    qDebug() << packed.toHex();

Of course QVariant can contain a QVarianList or a QVariantMap:

.. code-block:: cpp
    
    QList<QVariant> list;
    list << 123 << 4.56 << true;
    QByteArray packed = MsgPack::pack(list);
    qDebug() << packed.toHex();

.. note::
    If you want to pack QList<int> for example see: REF TO MsgPackStream

Unpacking
^^^^^^^^^

Unpacking is handled by ``MsgPack::unpack()`` function:

.. code-block:: cpp
    
    QByteArray packed = MsgPack::pack("qwerty");
    QVariant unpacked = MsgPack::unpack(packed);
    qDebug() << unpacked.toString();

.. tip::
    
    If packed data contains only one msgpack type (fixstr of fixmap for example), unpack will return it as ``QVariant(QString())`` and ``QVariant(QMap())`` respectively.
    But if there are several values packed, ``QVariant(QList())`` will be returned (consider this 5 bool values packed without msgpack's list: [0xc3, 0xc3, 0xc3, 0xc3, 0xc3])

More types
==========

There are built in packers and unpackers (basic and stream ones) for following types:
``QPoint, QSize, QRect, QTime, QDate, QDateTime, QColor, QGeoCoordinate``.
But since there is no such types in msgpack spec, ext type is used.

Example:

.. code-block:: cpp

    MsgPack::registerType(QMetaType::QPoint, 37); // 37 is msgpack user type id
    QByteArray ba = MsgPack::pack(QPoint(12, 34));
    qDebug() << MsgPack::unpack(ba).toPoint();

Note, that QColor and QGeoCoordinate is enabled by default only in qmake project.

Thread safety
=============

All methods are thread safe, except ``MsgPack::setCompatibilityModeEnabled`` which is not.

``pack()`` and ``unpack()`` do not use any global variables except user packers and unpackers, access to them is controlled via QReadLocker (and QWriteLocker when registering a new one), so readers do not block each other.

.. warning::

    User packers and unpackers can break thread-safety! But in most cases they are so simple, so this is not a problem.

Compatibility mode
==================

You can enable compatibility mode this way: ``MsgPack::setCompatibilityModeEnabled(true)``, after that there will be no str8, and QByteArray will be packed to str.