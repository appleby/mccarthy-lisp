#include <cassert>
#include <cstddef>
#include <cstring>

#include "alloc.h"
#include "cons.h"

namespace
{
using namespace mclisp;

const std::string kPname = "PNAME";

ConsCell* AtomMagic()
{
  return const_cast<ConsCell*>(Alloc::AtomMagic());
}

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


ConsCell* Get(std::string key, const ConsCell* alist)
{
  std::string thiskey;
  const ConsCell* next = alist;
  while (thiskey != key && next != kNil)
  {
    thiskey = Barf(&next->car);
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
    curr = Alloc::Allocate();
    data = Alloc::Allocate();
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
  ConsCell* pname = Alloc::Allocate();
  ConsCell* link = Alloc::Allocate();

  Slurp(&pname->car, kPname);
  pname->cdr = link;

  link->car = MakePnameList(name);
  link->cdr = kNil;

  return pname;
}

} // namespace

namespace mclisp
{
ConsCell* kT = nullptr;
ConsCell* kNil = nullptr;

void HackToFixNil()
{
  // TODO Get rid of HackToFixNil.
  // The association list of kNil should have only one item, it's pname. The
  // structure of kNil's alist should look like this:
  //          +----+----+     +-----+----+      +----+----+
  // kNil --->|ATOM|    |---->|PNAME|    |----->|    |    |-----> nullptr
  //          +----+----+     +-----+----+      +----+----+
  //                                              |
  //                                        +-----+                   
  //                                        |   +----+----+
  //                                        +-->|    |    |-----> nullptr
  //                                            +----+----+
  //                                              |
  //                                        +-----+
  //                                        |   +----+----+
  //                                        +-->|NIL |    |
  //                                            +----+----+
  //
  // The nullptr's need to be converted now to Nil's. The reason for this hack
  // is that Nil is symbol that is created and interned in Reader::Init. But
  // like all symbols, Nil's alist references Nil as the list terminator. At the
  // time when Nil is being created, kNil is still pointing to nullptr, and thus
  // we must go back an replace the nullptr's with pointers to kNil after kNil
  // has been initialized. The other option is to just nullptr as the list
  // terminator for symbol alist structure. That would avoid the need for this
  // hack, but would mean the symbol alist is not a proper list.
  assert (kNil->cdr->cdr->cdr == nullptr);
  assert (kNil->cdr->cdr->car->cdr == nullptr);
  kNil->cdr->cdr->cdr = kNil;
  kNil->cdr->cdr->car->cdr = kNil;
}

ConsCell* MakeCons(ConsCell* car, ConsCell* cdr)
{
  ConsCell* c = Alloc::Allocate();
  c->car = car;
  c->cdr = cdr;
  return c;
}

ConsCell* MakeSymbol(const std::string& name)
{
  assert(name.length());
  ConsCell* c = Alloc::Allocate();
  c->car = AtomMagic();
  c->cdr = MakeAssociationList(name);
  return c;
}

inline bool Symbolp(const ConsCell* c)
{
  return c->car == AtomMagic();
}

inline bool Consp(const ConsCell* c)
{
  return !Symbolp(c);
}

const std::string SymbolName(const ConsCell* symbol)
{
  if (!Symbolp(symbol))
    // TODO Throw better error.
    throw std::logic_error("Attempt to call SymbolName on a non-symbol.");
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

  if (!cons.car || !cons.cdr)
    // TODO error handling.
    throw std::logic_error("Attempted to print ConsCell with null car/cdr.");

  // TODO Pretty-print proper lists.
  // TODO Handle circular list structure.
  // TODO Figure out what C++ templating hell causes stream formatting to fail
  // when specifying a literal const char*, rather than a std::string.
  // return os << "(" << *cons.car << " . " << *cons.cdr << ")";
  static const std::string open("(");
  static const std::string dot(" . ");
  static const std::string close(")");
  return os << open << *cons.car << dot << *cons.cdr << close;
}

} // namespace mclisp
