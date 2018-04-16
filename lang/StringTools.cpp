#include "StringTools.h"

namespace lang {

std::size_t
strlen_save (register const char *s,
			 std::size_t maxlen)
{
    register const char *e;
    std::size_t n;

    for (e = s, n = 0; *e && n < maxlen; e++, n++);

    return n;
}

} // namespace lang
