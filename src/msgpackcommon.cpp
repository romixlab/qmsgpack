#include "msgpackcommon.h"

QString MsgPack::version()
{
	return QString("%1.%2.%3")
            .arg(MSGPACK_MAJOR)
            .arg(MSGPACK_MINOR)
            .arg(MSGPACK_VERSION);
}
