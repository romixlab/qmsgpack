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

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)
    //QCoreApplication a(argc, argv);

    MsgPack::registerPacker(QMetaType::QPoint, 7, packQPoint);

    QVariantList l;

    l << QPoint(1, 2);
    QByteArray arr = MsgPack::pack(l);
    qDebug() << arr.toBase64();


    return 0;
    //return a.exec();
}
