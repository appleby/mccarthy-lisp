#include <cstring>

#include "alloc.h"
#include "cons.h"

namespace mclisp
{
const ConsCell* kT = nullptr;
const ConsCell* kNil = nullptr;

//static constexpr char* kPname = "PNAME";

static ConsCell* MakeAssociationList(const std::string& name)
{
  // TODO
  ConsCell* c = Alloc::Allocate();
  return c;
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
