#include "qt_types_p.h"
#include "pack_p.h"
#include "unpack_p.h"

#include <QDebug>

#ifdef QT_GUI_LIB
#include <QColor>
#endif
#define NO_QTGUI_WARNING "Library built without QtGui, hence some types are not available"

bool MsgPackPrivate::register_qtype(QMetaType::Type q_type, quint8 msgpack_type)
{
	if (q_type == QMetaType::QColor) {
		#ifdef QT_GUI_LIB
		qRegisterMetaType<QColor>("QColor");
    	MsgPackPrivate::register_packer((QMetaType::Type)qMetaTypeId<QColor>(), msgpack_type, pack_qcolor);
    	MsgPackPrivate::register_unpacker(msgpack_type, unpack_qcolor);
    	return true;
    	#else
    	qWarning() << NO_QTGUI_WARNING;
    	return false;
    	#endif //QT_GUI_LIB
	}
}

#ifdef QT_GUI_LIB
quint32 MsgPackPrivate::pack_qcolor(const QVariant &variant, QByteArray &data, bool write)
{
    if (write) {
        QColor color = variant.value<QColor>();
        data.resize(4);
        data[0] = color.red();
        data[1] = color.green();
        data[2] = color.blue();
        data[3] = color.alpha();
    }
    return 4; // 4 bytes: r,g,b,a
}

QVariant MsgPackPrivate::unpack_qcolor(const QByteArray &data)
{
    return QColor((quint8)data[0], (quint8)data[1],
                  (quint8)data[2], (quint8)data[3]);
}
#endif //MsgPackPrivate
