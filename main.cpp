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

    MsgPack::registerPacker(QMetaType::QPoint, 7, packQPoint);
    MsgPack::registerUnpacker(7, unpackQPoint);

    QVariantList l;

    l << QPoint(12, 13);
    QByteArray arr = MsgPack::pack(l);
    qDebug() << arr.toBase64();

    qDebug() << MsgPack::unpack(arr);


    return 0;
    //return a.exec();
}
