qmsgpack
========

MessagePack for Qt

Installation
------------
Clone repository:
~~~
git clone https://github.com/romixlab/msgpack-qt.git
~~~

Run cmake:
~~~
cd msgpack-qt
mkdir build
cd build
cmake ..
~~~

Compile:
~~~
make
~~~

Run tests and install
~~~
make tests install
~~~

Sample usage
------------
Packing
~~~
QVariantList list;
list << 1 << 2 << 3;
QByteArray array = MsgPack::pack(list);
~~~

Unpacking:
~~~
QVariantList unpacked = MsgPack::unpack(array).toList();
~~~

By default these Qt types are supported: Int, UInt, LongLong, ULongLong, Double, QByteArray, Bool, QString, QStringList, QVariantList, QVariantMap.

Also you can provide packer and unpacker functions for any other type Qt (even for user type defined via  Q_DECLARE_METATYPE) or MsgPack type.
