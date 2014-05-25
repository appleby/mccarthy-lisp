#include "objects.h"

namespace mclisp
{
const Symbol kT("T");
const Symbol kNil("NIL");

Sexp::operator bool() const
{
  return this != &kNil;
}

} // namespace mclisp
