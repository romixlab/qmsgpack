#include <QCoreApplication>
#include <msgpack.h>
#include <QDebug>

void test(quint8 *p) {
    p++;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "MsgPack";
    qDebug() << MsgPack::deserialize(QByteArray::fromHex("92cd0100cd0200"));

    return a.exec();
}
