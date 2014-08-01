#include <algorithm>
#include <cstring>
#include <sstream>

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

const std::string ParityError::ConstructWhat(ConsCell *formals,
                                             ConsCell *actuals)
{
  std::ostringstream oss;

  auto fmt_args = [](ConsCell* list)
  {
    size_t len = Length(list);
    return (std::to_string(len) + " argument" + (len == 1 ? " " : "s ")
            + (len ? ToString(list) : ""));
  };

  oss << "Function expects " << fmt_args(formals) << " but was passed "
      << fmt_args(actuals) << ".";

  return oss.str();
}

const std::string TypeError::ConstructWhat(const ConsCell *object,
                                           const std::string& should_satisfy)
{
  std::ostringstream oss;
  oss << "'" << *object << "' does not satisfy " << should_satisfy << ".";
  return oss.str();
}
} // mclisp
