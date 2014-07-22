#include <cassert>
#include <cstddef>
#include <cstring>
#include <sstream>

#include "alloc.h"
#include "cons.h"
#include "error.h"

namespace
{
using namespace mclisp;

ConsCell* AtomMagic = reinterpret_cast<ConsCell*>(0x2);
ConsCell TStruct = { AtomMagic, nullptr };
ConsCell NilStruct = { AtomMagic, nullptr };
ConsCell PnameStruct = { AtomMagic, nullptr };
ConsCell* kPname = &PnameStruct;

std::string Barf(ConsCell * const * c)
{
  // Barf sizeof c bytes into s. The length of the string may be less than
  // sizeof c bytes, in which case we truncate the string at the null byte.
  std::string s(reinterpret_cast<const char *>(c), sizeof c);
  return s.substr(0, s.find('\0'));
}

std::string Slurp(ConsCell** c, const std::string& s)
{
  strncpy(reinterpret_cast<char *>(c), s.c_str(), sizeof c);
  return s.length() > sizeof c ? s.substr(sizeof c, std::string::npos) : "";
}


ConsCell* Get(ConsCell* key, const ConsCell* alist)
{
  assert(Symbolp(key));

  ConsCell* thiskey = nullptr;
  const ConsCell* next = alist;

  while (thiskey != key && next != kNil)
  {
    thiskey = next->car;
    next = next->cdr;
  }
  return next == kNil ? kNil : next->car;
}

std::string StringFromPname(const ConsCell* c)
{
  std::string pname;
  const ConsCell* data;

  while (c != kNil)
  {
    data = c->car;
    pname += Barf(&data->car);
    pname += Barf(&data->cdr);
    c = c->cdr;
  }

  return pname;
}

ConsCell* MakePnameList(const std::string& name)
{
  std::string rest(name);
  ConsCell *head, *prev, *curr, *data;
  head = prev = curr = data = nullptr;

  while (rest.length() > 0)
  {
    curr = alloc::Allocate();
    data = alloc::Allocate();
    rest = Slurp(&data->car, rest);
    rest = Slurp(&data->cdr, rest);

    if (!head)
      head = curr;
    if (prev)
      prev->cdr = curr;

    curr->car = data;
    prev = curr;
  }

  curr->cdr = kNil;
  return head;
}

ConsCell* MakeAssociationList(const std::string& name)
{
  ConsCell* pname = alloc::Allocate();
  ConsCell* link = alloc::Allocate();

  pname->car = kPname;
  pname->cdr = link;

  link->car = MakePnameList(name);
  link->cdr = kNil;

  return pname;
}

std::ostream& FormatListOnStream(std::ostream& os, const ConsCell& cons,
                                 bool want_open_paren=true)
{
  if (cons.car == nullptr || cons.cdr == nullptr)
    ARGUMENT_ERROR("Attempted to print ConsCell with nullptr in car/cdr.");

  // TODO Handle circular list structure.
  if (want_open_paren)
    os << "(";

  os << *cons.car;

  if (Null(cons.cdr))
    // End of proper list.
    return os << ")";

  if (Atom(cons.cdr))
    // End of dotted list.
    return os << " . " << *cons.cdr << ")";

  os << ", ";
  return FormatListOnStream(os, *cons.cdr, false);
}
} // namespace

namespace mclisp
{

// Nil is a self-referential symbol. The association list of Nil, being a list,
// is terminated with a Nil. If we attempt to set kNil to nullptr here and call
// MakeSymbol to initialize it a run-time, then Nil's association list will
// actually be terminated with a nullptr instead of Nil. To get around this, we
// statically allocate the first ConsCell of kNil, and set the cdr to the result
// of calling MakeAssociationList in cons::Init.  kT could be initialized to
// nullptr here, but it's handled the same as kNil for symmetry, and to allow
// FromBool to work even before kT has been initialized. The kPname symbol is
// also self-referential and therefore handled in the same way, but its scope is
// limited to this file, so it's declared in the anonymous namespace, above.
ConsCell* kT = &TStruct;
ConsCell* kNil = &NilStruct;
std::map<std::string, ConsCell *> g_builtin_symbols;

namespace cons
{

void Init()
{
  static bool initialized = false;

  if (initialized)
    return;

  kT->cdr = MakeAssociationList("T");
  g_builtin_symbols.emplace("T", kT);

  kNil->cdr = MakeAssociationList("NIL");
  g_builtin_symbols.emplace("NIL", kNil);

  kPname->cdr = MakeAssociationList("PNAME");
  g_builtin_symbols.emplace("PNAME", kPname);

  std::vector<const char *> builtin_names =
  { "ATOM", "CAR", "CDR", "COND", "CONS", "DEFUN", "EOF", "EQ", "EVAL", "LABEL",
    "LAMBDA", "LOAD", "PRINT", "QUOTE" };

  for (auto it : builtin_names)
    g_builtin_symbols.emplace(it, MakeSymbol(it));

  initialized = true;
}

} // namespace cons

ConsCell::operator bool() const
{
  return this != kNil;
}

ConsCell* FromBool(bool pred)
{
  return pred ? kT : kNil;
}

ConsCell* Cons(ConsCell* car, ConsCell* cdr)
{
  ConsCell* c = alloc::Allocate();
  c->car = car;
  c->cdr = cdr;
  return c;
}

ConsCell* Acons(ConsCell* key, ConsCell* value, ConsCell* alist)
{
  // In McCarthy's paper, unlike in Common Lisp, assoc expects the sublists of
  // the alist to be proper lists, not dotted lists.
  return Cons(List(key, value), alist);
}

ConsCell* MakeSymbol(const std::string& name)
{
  assert(name.length());
  ConsCell* c = alloc::Allocate();
  c->car = AtomMagic;
  c->cdr = MakeAssociationList(name);
  return c;
}

bool Atom(const ConsCell* c)
{
  return c->car == AtomMagic;
}

bool Symbolp(const ConsCell* c)
{
  return Atom(c);
}

bool Consp(const ConsCell* c)
{
  return !Atom(c);
}

bool Listp(const ConsCell* c)
{
  return Null(c) || Consp(c);
}

bool Eq(const ConsCell* a, const ConsCell* b)
{
  return a == b;
}

bool Null(const ConsCell* c)
{
  return Eq(c, kNil);
}

ConsCell* Car(const ConsCell* c)
{
  TYPECHECK(c, Listp);
  return c == kNil ? kNil : c->car;
}

ConsCell* Cdr(const ConsCell* c)
{
  TYPECHECK(c, Listp);
  return c == kNil ? kNil : c->cdr;
}

ConsCell* Caar(const ConsCell* c)
{
  return Car(Car(c));
}

ConsCell* Cadr(const ConsCell* c)
{
  return Car(Cdr(c));
}

ConsCell* Cadar(const ConsCell* c)
{
  return Car(Cdr(Car(c)));
}

ConsCell* Caddr(const ConsCell* c)
{
  return Car(Cdr(Cdr(c)));
}

ConsCell* Cdddr(const ConsCell* c)
{
  return Cdr(Cdr(Cdr(c)));
}

ConsCell* Caddar(const ConsCell* c)
{
  return Car(Cdr(Cdr(Car(c))));
}

// 4. assoc [x;y]. If y is a list of the form  ((u_1, v_1), ..., (u_n, v_n)) and
// x is one of the u's, then assoc [x; y] is the corresponding v.
ConsCell* Assoc(const ConsCell* k, const ConsCell* alist)
{
  TYPECHECK(alist, Listp);

  if (Null(alist))
    // Because McCarthy's version of Assoc just returns the corresponding v, and
    // not the (u, v) pair as in Common Lisp, we need a way to distinguish "not
    // found" from "v = NIL". Hence, we return nullptr here instead of kNil to
    // indicate lookup failure.
    return nullptr;

  TYPECHECK(Car(alist), Consp);

  if (Eq(k, Caar(alist)))
    // McCarthy's assoc expects the sublists to be proper lists, not dotted
    // lists as in Common Lisp; hence Cadar here instead of Cdar.
    return Cadar(alist);

  return Assoc(k, Cdr(alist));
}

ConsCell* CopyAlist(const ConsCell* alist)
{
  TYPECHECK(alist, Listp);

  if (Null(alist))
    return kNil;

  TYPECHECK(Car(alist), Consp);

  return Cons(CopyList(Car(alist)), CopyAlist(Cdr(alist)));
}

ConsCell* CopyList(const ConsCell* list)
{
  TYPECHECK(list, Listp);

  if (Null(list))
    return kNil;

  if (!Consp(Cdr(list)))
    return Cons(Car(list), Cdr(list));

  return Cons(Car(list), CopyList(Cdr(list)));
}

ConsCell* List_(std::vector<ConsCell *> values)
{
  ConsCell* L = kNil;

  for (auto it = values.rbegin(); it != values.rend(); ++it)
    L = Cons(*it, L);

  return L;
}

size_t Length(const ConsCell *list)
{
  TYPECHECK(list, Listp);

  if (Null(list))
    return 0;

  return 1 + Length(Cdr(list));
}

ConsCell* Append(const ConsCell* L1, ConsCell* L2)
{
  TYPECHECK(L1, Listp);
  TYPECHECK(L2, Listp);

  if (Null(L1))
    return L2;

  return Cons(Car(L1), Append(Cdr(L1), L2));
}

ConsCell* Pair(const ConsCell *L1, const ConsCell *L2)
{
  TYPECHECK(L1, Listp);
  TYPECHECK(L2, Listp);

  if (Null(L1) && Null(L2))
    return kNil;

  if (Null(L1) || Null(L2))
    ARGUMENT_ERROR("The lists L1 and L2 must be the same length.");

  return Acons(Car(L1), Car(L2), Pair(Cdr(L1), Cdr(L2)));
}

const std::string SymbolName(const ConsCell* symbol)
{
  TYPECHECK(symbol, Symbolp);
  return StringFromPname(Get(kPname, symbol->cdr));
}

// Logical and relational operators (==, !=, <, >, <=, >=)
bool operator==(const ConsCell& lhs, const ConsCell& rhs)
{
  if (Symbolp(&lhs) && Symbolp(&rhs))
    return &lhs == &rhs;

  if (Consp(&lhs) && Consp(&rhs))
    return *lhs.car == *rhs.car && *lhs.cdr == *rhs.cdr;

  return false;
}

bool operator< (const ConsCell& lhs, const ConsCell& rhs)
{
  if (Symbolp(&lhs) && Symbolp(&rhs))
    return SymbolName(&lhs) < SymbolName(&rhs);

  if (Consp(&lhs) && Consp(&rhs))
    return (*lhs.car < *rhs.car ||
            (*lhs.car == *rhs.car && *lhs.cdr < *rhs.cdr));

  return false;
}
bool operator!=(const ConsCell& lhs, const ConsCell& rhs){return !operator==(lhs,rhs);}
bool operator> (const ConsCell& lhs, const ConsCell& rhs){return  operator< (rhs,lhs);}
bool operator<=(const ConsCell& lhs, const ConsCell& rhs){return !operator> (lhs,rhs);}
bool operator>=(const ConsCell& lhs, const ConsCell& rhs){return !operator< (lhs,rhs);}

std::ostream& operator<<(std::ostream& os, const ConsCell& cons)
{
  if (Symbolp(&cons))
    return os << SymbolName(&cons);

  return FormatListOnStream(os, cons);
}

// Mostly useful as a way to print a ConsCell in GDB.
std::string ToString(const ConsCell* cons)
{
  std::ostringstream oss;
  oss << *cons;
  return oss.str();
}

} // namespace mclisp
