#include <algorithm>
#include <cstring>
#include "error.h"

#define MAYBE_COPY(buf, space, str) \
  if (space) { \
    const char *s = (str); \
    size_t n = std::min(strlen(s), space); \
    strncpy(buf, s, n); \
    buf += n; \
    space -= n; \
    *buf = '\0'; \
  }

namespace mclisp
{
const char* Error::what() const noexcept
{
  static char buffer[256];
  char *buf = buffer;
  size_t space = sizeof buffer - 1;

  MAYBE_COPY(buf, space, prefix());
  MAYBE_COPY(buf, space, " ");
  MAYBE_COPY(buf, space, std::logic_error::what());
  return buffer;
}

const char* TypeError::what() const noexcept
{
  // TODO don't call function that might throw.
  static char buffer[256];
  char *buf = buffer;
  size_t space = sizeof buffer - 1;

  std::string objstr = ToString(object_);

  MAYBE_COPY(buf, space, prefix());
  MAYBE_COPY(buf, space, " '");
  // TODO truncate objstr?
  MAYBE_COPY(buf, space, objstr.c_str());
  MAYBE_COPY(buf, space, "' does not satisfy ");
  MAYBE_COPY(buf, space, should_satisfy_.c_str());
  MAYBE_COPY(buf, space, ".");
  return buffer;
}
} // mclisp
