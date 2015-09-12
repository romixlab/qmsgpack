Custom types
------------

.. contents::
   :depth:  4

Custom QVariant
===============

You can provide two functions for packing QVariant with custom type inside to QByteArray and vise versa.
For example here is how QColor packer and unpacker looks like:

.. code-block:: cpp

	QByteArray pack_qcolor(const QVariant &variant)
	{
	    QColor color = variant.value<QColor>();
	    if (!color.isValid())
	        return QByteArray(1, 0);
	    QByteArray data;
	    data.resize(4);
	    quint8 *p = (quint8 *)data.data();
	    p[0] = color.red();
	    p[1] = color.green();
	    p[2] = color.blue();
	    p[3] = color.alpha();
	    return data;
	}

	QVariant unpack_qcolor(const QByteArray &data)
	{
	    if (data.length() == 1)
	        return QColor();
	    quint8 *p = (quint8 *)data.data();
	    return QColor(p[0], p[1], p[2], p[3]);
	}

And that's it!
Now register this two functions:

.. code-block:: cpp

	MsgPack::registerPacker(QMetaType::QColor, 3, pack_qcolor); // 3 is msgpack ext type id
	MsgPack::registerUnpacker(3, unpack_qcolor);

After that ``MsgPack::pack(QColor(127, 127, 127))`` will start to work!

Custom stream
=============

You can provide stream operators for any other type you might want to work with. But there are some pitfalls to consider if you want to unpack something with other MsgPack implementations.

Example:

.. code-block:: cpp

    class SomeType
    {
        public:
            double x() const { return m_x; }
            void setX(double x) { m_x = x;}

            double y() const { return m_y; }
            void setY(double y) { m_y = y;}

            double z() const { return m_z; }
            void setZ(double z) { m_z = z;}

        private:
            double m_x, m_y, m_z;
    };

    MsgPackStream &operator<<(MsgPackStream &s, const SomeType &t)
    {
        s.writeExtHeader(27, <msgpack user type id>); // size of packed double is 9 * 3 = 27
        s << t.x() << t.y() << t.z();
        return s;
    }

    MsgPackStream &operator>>(MsgPackStream &s, SomeType &t)
    {
        quint32 len;
        s.readExtHeader(len);
        if (len != 27) {
            s.setStatus(MsgPackStream::ReadCorruptData);
            return s;
        }
        double x, y, z;
        s >> x >> y >> z;
        t.setX(x);
        t.setY(y);
        t.setZ(z);
        return s;
    }

In this case size of data is known id advance, if this is not the case, then you can use QByteArray. Here is how QPoint operators are implemented:

.. code-block:: cpp

    MsgPackStream &operator<<(MsgPackStream &s, const QPoint &point)
    {
        // we need to know user type id, that was registered with MsgPack::registerType
        qint8 msgpackType = MsgPack::msgpackType(QMetaType::QPoint);
        if (msgpackType == -1) {
            s.setStatus(MsgPackStream::WriteFailed);
            return s;
        }
        QByteArray ba;
        MsgPackStream out(&ba, QIODevice::WriteOnly); // stream inside stream ;)
        if (point.isNull()) { // save some bytes if point is invalid
            quint8 p[1] = {0};
            out.writeBytes((const char *)p, 1);
        } else {
            out << point.x() << point.y();
        }
        s.writeExtHeader(ba.length(), msgpackType); // only now write msgpack ext field
        s.writeBytes(ba.data(), ba.length()); // and variable length data
        return s;
    }

    MsgPackStream &operator>>(MsgPackStream &s, QPoint &point)
    {
        quint32 len;
        s.readExtHeader(len); // read msgpack ext field
        if (len == 1) { // handle invalid QPoint
            point = QPoint();
            return s;
        }
        QByteArray ba;
        ba.resize(len);
        s.readBytes(ba.data(), len); // read len bytes to byte array
        MsgPackStream in(ba);
        int x, y;
        in >> x >> y;
        point = QPoint(x, y);
        return s;
    }

.. tip::

    Of course you can just stream out everything without any ext header and user type id's, like this: ``s << point.x() << point.y(); return s;`` but in that case you will not be able to unpack anything useful with MsgPack::unpack() or in other MsgPack implementations.