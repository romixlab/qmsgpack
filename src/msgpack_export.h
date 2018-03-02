#ifndef MSGPACK_EXPORT_H
#define MSGPACK_EXPORT_H

#include <QtCore/qglobal.h>

#ifndef MSGPACK_STATIC
#  if defined(MSGPACK_MAKE_LIB)
#    define MSGPACK_EXPORT Q_DECL_EXPORT
#  else
#    define MSGPACK_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define MSGPACK_EXPORT
#endif

#endif // MSGPACK_EXPORT_H
