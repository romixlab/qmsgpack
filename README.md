MessagePack for Qt
==================

Installation
------------
Clone repository:
~~~bash
git clone https://github.com/romixlab/qmsgpack.git
cd qmsgpack
mkdir build
cd build
cmake ..
make install
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

Streaming API:
~~~cpp
// packing
MsgPackStream stream(&ba, QIODevice::WriteOnly);
stream << 1 << 2.3 << "some string";

// unpacking
MsgPackStream stream(ba);
int a;
double b;
QSting s;
stream >> a >> b >> s;
~~~
