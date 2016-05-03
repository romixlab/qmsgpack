qmsgpack
========
MessagePack for Qt

<p align="center">
    <a href="https://travis-ci.org/romixlab/qmsgpack">
        <img src="https://travis-ci.org/romixlab/qmsgpack.svg?branch=master"
             alt="Build Status">
    </a>
    <a href="http://qmsgpack.readthedocs.io/en/latest/">
        <img src="https://readthedocs.org/projects/qmsgpack/badge/?version=latest"
             alt="Doc Status">
    </a>
</p>



Documentation
-------------
Complete documentation is available here: [qmsgpack.readthedocs.io](http://qmsgpack.readthedocs.io/en/latest/)

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

Qt types and User types
-----------------------
There is packers and unpackers for QColor, QTime, QDate, QDateTime, QPoint, QSize, QRect. Also you can create your own packer/unpacker methods for Qt or your own types. See [docs](http://msgpack.marsworks.ru/) for details.

Thread-safety
-------------
At this time it is not safe to call `MsgPack::registerPacker`, `MsgPack::registerUnpacker` and `MsgPack::setCompatibilityModeEnabled` from different threads.
pack and unpack methods do not use any global data, so should be safe to use from multiple threads.
