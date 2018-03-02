// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "msgpackcommon.h"

QString MsgPack::version()
{
    return QString::fromLatin1("%1.%2.%3")
            .arg(MSGPACK_MAJOR)
            .arg(MSGPACK_MINOR)
            .arg(MSGPACK_VERSION);
}
