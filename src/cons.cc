#include <cassert>
#include <cstddef>
#include <cstring>

#include "alloc.h"
#include "cons.h"

namespace
{
using namespace mclisp;

const std::string kPname = "PNAME";

void SetCons(ConsCell** c, const char* s)
{
  strncpy(reinterpret_cast<char *>(c), s, sizeof c);
}

std::string Slurp(ConsCell** c, const std::string& s)
{
  SetCons(c, s.c_str());
  return s.length() > sizeof c ? s.substr(sizeof c, std::string::npos) : "";
}

ConsCell* MakePnameList(const std::string& name)
{
  ConsCell *head, *prev, *curr, *data;
  std::string rest(name);

  for (head = prev = curr = Alloc::Allocate();
       rest.length() > 0;
       curr = Alloc::Allocate())
  {
    data = Alloc::Allocate();
    rest = Slurp(&data->car_, rest);
    rest = Slurp(&data->cdr_, rest);

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

  SetCons(&pname->car_, kPname.c_str());
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
  c->car_ = const_cast<ConsCell*>(Alloc::AtomMagic());
  c->cdr_ = MakeAssociationList(name);
  return c;
}

const std::string SymbolName(const ConsCell* symbol)
{
  // TODO
  return "NIL";
}

} // namespace mclisp
