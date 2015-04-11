qmsgpack [![Build Status](https://travis-ci.org/romixlab/qmsgpack.svg?branch=qt_types)](https://travis-ci.org/romixlab/qmsgpack)
========

MessagePack for Qt

Installation
------------
Clone repository:
~~~bash
git clone https://github.com/romixlab/qmsgpack.git
~~~

Run cmake:
~~~bash
cd qmsgpack
mkdir build
cd build
cmake ..
~~~

Compile:
~~~bash
make
~~~

Run tests and install
~~~bash
make tests install
~~~

Sample usage
------------
Packing
~~~cpp
QVariantList list;
list << 1 << 2 << 3;
QByteArray array = MsgPack::pack(list);
~~~

Unpacking:
~~~cpp
QVariantList unpacked = MsgPack::unpack(array).toList();
~~~

By default these Qt types are supported: `Int, UInt, LongLong, ULongLong, Double, QByteArray, Bool, QString, QStringList, QVariantList, QVariantMap`.

Also you can provide packer and unpacker functions for any other type (even for user type defined via `Q_DECLARE_METATYPE`) or MsgPack type.

Thread-safety
-------------
At this time it is not safe to call `MsgPack::registerPacker`, `MsgPack::registerUnpacker` and `MsgPack::setCompatibilityModeEnabled` from different threads.
pack and unpack methods do not use any global data, so should be safe to use from multiple threads.
