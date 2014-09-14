#include "pack_p.h"



quint8 *MsgPackPrivate::pack(const QVariant &v, quint8 *p, bool sp)
{
    QMetaType::Type t = (QMetaType::Type)v.type();
    if (t == QMetaType::Int)
        p = pack_int(v, p, sp);
    else if (t == QMetaType::Bool)
        p = pack_bool(v, p, sp);
    else if (t == QMetaType::QVariantList)
        p = pack_list(v, p, sp);
    return p;
}

quint8 *MsgPackPrivate::pack_int(const QVariant &v, quint8 *p, bool sp)
{

}


quint8 *MsgPackPrivate::pack_bool(const QVariant &v, quint8 *p, bool sp)
{
    if (!sp)
        *p = v.toBool() ? 0xc3 : 0xc2;
    return p + 1;
}


quint8 *MsgPackPrivate::pack_list(const QVariant &v, quint8 *p, bool sp)
{
    QVariantList list = v.toList();
    //pack_int(...)
    if (!sp)
        *p = 0x92; // 2el
    p++;
    foreach (QVariant item, v.toList())
        p = pack(item, p, sp);
    return p;
}
