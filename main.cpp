#include <QCoreApplication>
#include <msgpack.h>
#include <QDebug>

#include <vector>

//int is_big_endian(void)
//{
//    union {
//        quint32 i;
//        char c[4];
//    } bint = {0x01020304};

//    return bint.c[0] == 1;
//}

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

//    qDebug() << "MsgPack";
//    qDebug() << MsgPack::deserialize(QByteArray::fromHex("dc 00 33 c2 c3 c0 00 00 00 00 00 00 00 00 00 ff ff ff ff ff 7f 7f cc ff cd ff ff ce ff ff ff ff e0 e0 d1 ff 80 d2 ff ff 80 00 d3 ff ff ff ff 80 00 00 00 00 00 01 ff a1 61 a1 61 a1 61 a0 a0 a0 91 00 91 00 91 00 90 90 90 80 80 80 81 a1 61 61 81 a1 61 61 81 a1 61 61 91 90 91 91 a1 61"));



    QVariantList l;
    l << "abcdefhijkabcdefhijkabcdefhijk12";
    QByteArray arr = MsgPack::pack(l);
    qDebug() << arr.toBase64();

    return 0;
    //return a.exec();
}
