#include <QCoreApplication>
#include <msgpack.h>
#include <QDebug>
#include <QDataStream>
#include <QPoint>

quint32 packQPoint(const QVariant &variant, QByteArray &data, bool write)
{
    if (write) {
        data.resize(8);
        QDataStream out(&data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_3);
        out << variant.toPoint();
    }
    return 8;
}

QVariant unpackQPoint(QByteArray &data)
{
    QDataStream in(&data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);
    QPoint p;
    in >> p;
    return p;
}

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)
    //QCoreApplication a(argc, argv);

    qDebug() << "msgpack v" << MsgPack::version();

    QVariantMap map;
    map["one"] = 1;
    map["map"] = QVariantMap();
//    map["booleans"] = QVariantList() << false << true;
//    map["list2"] = QVariantList() << (QVariantList() << "abc" << "def")
//                                  << (QVariantList() << "qwe" << "rty");
    map["integers"] = QVariantList() << 0 << 127 << -31 << 128 << 777;
//    map["bytearray"] = QByteArray("bytes");

//    QVariantMap map2;
//    map2["one"] = 1;
//    map2["two"] = QVariantMap();
//    map["map2"] = map2;

    QByteArray arr = MsgPack::pack(map);

    QVariantMap um = MsgPack::unpack(arr).toMap();

    qDebug() << arr.toBase64();

    return 0;
    //return a.exec();
}
