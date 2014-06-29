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


const ConsCell* Get(std::string key, const ConsCell* alist)
{
  std::string thiskey;
  const ConsCell* next = alist;
  while (thiskey != key && next != kNil)
  {
    thiskey = Barf(&next->car_);
    next = next->cdr_;
  }
  return next == kNil ? kNil : next->car_;
}

std::string StringFromPname(const ConsCell* c)
{
  std::string pname;
  const ConsCell* data;

  while (c != kNil)
  {
    data = c->car_;
    pname += Barf(&data->car_);
    pname += Barf(&data->cdr_);
    c = c->cdr_;
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
    rest = Slurp(&data->car_, rest);
    rest = Slurp(&data->cdr_, rest);

    if (!head)
      head = curr;
    if (prev)
      prev->cdr_ = curr;

    curr->car_ = data;
    prev = curr;
  }

  curr->cdr_ = const_cast<ConsCell*>(kNil);
  return head;
}

ConsCell* MakeAssociationList(const std::string& name)
{
  ConsCell* pname = Alloc::Allocate();
  ConsCell* link = Alloc::Allocate();

  Slurp(&pname->car_, kPname);
  pname->cdr_ = link;

  link->car_ = MakePnameList(name);
  link->cdr_ = const_cast<ConsCell*>(kNil);

  return pname;
}

} // namespace

namespace mclisp
{
const ConsCell* kT = nullptr;
const ConsCell* kNil = nullptr;

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
  assert (kNil->cdr_->cdr_->cdr_ == nullptr);
  assert (kNil->cdr_->cdr_->car_->cdr_ == nullptr);
  kNil->cdr_->cdr_->cdr_ = const_cast<ConsCell *>(kNil);
  kNil->cdr_->cdr_->car_->cdr_ = const_cast<ConsCell *>(kNil);
}

const ConsCell* MakeCons(const ConsCell* car, const ConsCell* cdr)
{
  ConsCell* c = Alloc::Allocate();
  c->car_ = const_cast<ConsCell*>(car);
  c->cdr_ = const_cast<ConsCell*>(cdr);
  return c;
}

const ConsCell* MakeSymbol(const std::string& name)
{
  assert(name.length());
  ConsCell* c = Alloc::Allocate();
  c->car_ = AtomMagic();
  c->cdr_ = MakeAssociationList(name);
  return c;
}

inline bool Symbolp(const ConsCell* c)
{
  return c->car_ == AtomMagic();
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
  return StringFromPname(Get(kPname, symbol->cdr_));
}

// Logical and relational operators (==, !=, <, >, <=, >=)
bool operator==(const ConsCell& lhs, const ConsCell& rhs)
{
  if (Symbolp(&lhs) && Symbolp(&rhs))
    return &lhs == &rhs;

  if (Consp(&lhs) && Consp(&rhs))
    return *lhs.car_ == *rhs.car_ && *lhs.cdr_ == *rhs.cdr_;

  return false;
}

bool operator< (const ConsCell& lhs, const ConsCell& rhs)
{
  if (Symbolp(&lhs) && Symbolp(&rhs))
    return SymbolName(&lhs) < SymbolName(&rhs);

  if (Consp(&lhs) && Consp(&rhs))
    return (*lhs.car_ < *rhs.car_ ||
            (*lhs.car_ == *rhs.car_ && *lhs.cdr_ < *rhs.cdr_));

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

  // FIXME
  //return os << "(" << *cons.car_ << " . " << *cons.cdr_ << ")";
  return os;
}

} // namespace mclisp
