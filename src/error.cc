#include <cstring>
#include "error.h"

namespace mclisp
{

const char* Error::what() const noexcept
{
  static char buf[256];
  // TODO strncpy / strncat
  strcpy(buf, prefix());
  strcat(buf, " ");
  strcat(buf, std::logic_error::what());
  return buf;
}
} // mclisp
