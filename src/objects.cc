#include "objects.h"

namespace mclisp
{
const Symbol koT("T");
const Symbol koNil("NIL");

Sexp::operator bool() const
{
  return this != &koNil;
}

// Logical and relational operators (==, !=, <, >, <=, >=)
#define MCLISP_DEF_DERIVED_BOOL_OPS(arg_type) \
bool operator!=(arg_type lhs, arg_type rhs){return !operator==(lhs,rhs);} \
bool operator> (arg_type lhs, arg_type rhs){return  operator< (rhs,lhs);} \
bool operator<=(arg_type lhs, arg_type rhs){return !operator> (lhs,rhs);} \
bool operator>=(arg_type lhs, arg_type rhs){return !operator< (lhs,rhs);}

// Sexp operators attempt to cast both args to the same concrete subtype and
// defer to the operator overloaded on that type.
#define MCLISP_DEF_SEXP_BOOL_OP(op) \
bool op(const Sexp& lhs, const Sexp& rhs) { \
  try { \
    const Symbol& sym_lhs = dynamic_cast<const Symbol&>(lhs); \
    const Symbol& sym_rhs = dynamic_cast<const Symbol&>(rhs); \
    return op(sym_lhs, sym_rhs); \
  } catch (std::bad_cast) {} \
  try { \
    const Cons& cons_lhs = dynamic_cast<const Cons&>(lhs); \
    const Cons& cons_rhs = dynamic_cast<const Cons&>(rhs); \
    return op(cons_lhs, cons_rhs); \
  } catch (std::bad_cast) {} \
  return false; \
}

MCLISP_DEF_SEXP_BOOL_OP(operator==);
MCLISP_DEF_SEXP_BOOL_OP(operator<);
MCLISP_DEF_DERIVED_BOOL_OPS(const Sexp&);

bool operator==(const Symbol& lhs, const Symbol& rhs)
{ 
  return lhs.name() == rhs.name();
}

bool operator< (const Symbol& lhs, const Symbol& rhs)
{ 
  return lhs.name() < rhs.name();
}
MCLISP_DEF_DERIVED_BOOL_OPS(const Symbol&);

bool operator==(const Cons& lhs, const Cons& rhs)
{
  return lhs.car() == rhs.car() && lhs.cdr() == rhs.cdr();
}

bool operator< (const Cons& lhs, const Cons& rhs)
{
  return lhs.car() < rhs.car() || (lhs.car() == rhs.car() && lhs.cdr() < rhs.cdr());
}
MCLISP_DEF_DERIVED_BOOL_OPS(const Cons&);


// ostream operators (<<)
std::ostream& operator<<(std::ostream& os, const Sexp& sexp)
{
  try {
    const Symbol& symbol = dynamic_cast<const Symbol&>(sexp);
    return os << symbol;
  } catch (std::bad_cast) {}

  try {
    const Cons& cons = dynamic_cast<const Cons&>(sexp);
    return os << cons;
  } catch (std::bad_cast) {}

  throw std::logic_error("operator<<: Argument Sexp is of unknown type.");
}

std::ostream& operator<<(std::ostream& os, const Symbol& symbol)
{
  return os << symbol.name();
}

std::ostream& operator<<(std::ostream& os, const Cons& cons)
{
  return os << "(" << cons.car() << " . " << cons.cdr() << ")";
}

} // namespace mclisp
