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

const char* TypeError::what() const noexcept
{
  // TODO strncpy / strncat
  // TODO don't call function that might throw.
  static char buf[256];
  std::string objstr = ToString(object_);

  strcpy(buf, prefix());
  strcat(buf, " '");
  strncat(buf, objstr.c_str(), 32);

  if (objstr.length() > 32)
    strcat(buf, "...");

  strcat(buf, "' does not satisfy ");
  strcat(buf, should_satisfy_.c_str());
  strcat(buf, ".");
  return buf;
}
} // mclisp
