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
  return std::string(reinterpret_cast<const char *>(c), sizeof c);
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

  while (c != kNil && c != nullptr)
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

const std::string SymbolName(const ConsCell* symbol)
{
  if (!Symbolp(symbol))
    // TODO Throw better error.
    throw std::logic_error("Attempt to call SymbolName on a non-symbol.");
  return StringFromPname(Get(kPname, symbol->cdr_));
}

} // namespace mclisp
